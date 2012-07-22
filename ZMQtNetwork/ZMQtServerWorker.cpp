#include "ZMQtServerWorker.h"

#include "ZMQtServer_P.h"
#include <QtCore/QDebug>

namespace ZMQtNetwork
{
    ZMQtServerWorker::ZMQtServerWorker(ZMQtServer_P *server, QObject *parent)
        : QThread(parent)
        , m_iTimerID(0)
        , m_pServer(server)
    {
        m_pSocket = zmq_socket(m_context.getContext(), ZMQ_DEALER);
        if (m_pSocket)
        {
            int _linger=0;
            zmq_setsockopt(m_pSocket, ZMQ_LINGER, &_linger, sizeof(_linger));
            int _rt = zmq_connect(m_pSocket, "inproc://backend");
            if (_rt == 0)
            {
                qDebug() << "backend connect success!";
                start();
            }
            else
            {
                qCritical() << "backend connect error:" << zmq_strerror(zmq_errno());
            }

            m_iTimerID = startTimer(1);
        }
    }

    ZMQtServerWorker::~ZMQtServerWorker()
    {
        zmq_close(m_pSocket);
    }

    void ZMQtServerWorker::run()
    {
        connect(this, SIGNAL(requestSend(const QString &, const QByteArray &)),
                this, SLOT(sendSlot(const QString &, const QByteArray &)), Qt::QueuedConnection);

        exec();
    }

    void ZMQtServerWorker::timerEvent( QTimerEvent */*event*/ )
    {
        // 尝试获取消息地址——可能还未收到消息
        zmq_msg_t _msg;
        zmq_msg_init(&_msg);
        int _rc = zmq_recv (m_pSocket, &_msg, ZMQ_NOBLOCK);
        if (_rc != 0)
        {
            if (zmq_errno() != EAGAIN)
            {
                qCritical() << "message receive error:" << zmq_strerror(zmq_errno());
            }
            zmq_msg_close(&_msg);
            // 没有收到消息, 不继续
            return;
        }
        // 获取消息地址成功, 将这个地址存档
        QString _UID;
        QString _hex;
        zmq_msg_t *_address = new zmq_msg_t;
        zmq_msg_init(_address);
        zmq_msg_copy(_address, &_msg);
        char *_addressData = (char *)zmq_msg_data(_address);
        int _size = zmq_msg_size(_address);
        if (_addressData[0])
        {
            _UID = QString::fromLocal8Bit(_addressData, _size);
        }
        else
        {
            for(int i=1; i<_size; i++)
            {
                _hex = QString::number((unsigned char)_addressData[i], 16).toUpper();
                if (_hex.size() == 1)
                {
                    _hex.prepend("0");
                }
                _UID.append(_hex);
            }
        }
        // 关闭当前UID存档的老的地址消息
        zmq_msg_t *oldAddress = m_pServer->m_addressMap.value(_UID);
        if (oldAddress)
        {
            zmq_msg_close(oldAddress);
            delete oldAddress;
        }
        // 将新的消息地址存入map表
        m_pServer->m_addressMap.insert(_UID, _address);
        // 关闭收到的地址消息
        zmq_msg_close(&_msg);

        // 开始获取消息内容
        QByteArray _data;
        long long _more = 0;
        size_t _more_size = sizeof(_more);
        do
        {
            // 初始化信息
            zmq_msg_init(&_msg);
            zmq_recv (m_pSocket, &_msg, 0);
            // 保存收到的消息到字节数组
            _data.append((const char *)zmq_msg_data(&_msg), zmq_msg_size(&_msg));
            // 关闭收到的消息
            zmq_msg_close(&_msg);
            // 判读是否拥有额外的信息帧
            _rc = zmq_getsockopt(m_pSocket, ZMQ_RCVMORE, &_more, &_more_size);
        } while(_more);

        emit dataReceived(_UID, _data);
    }

    void ZMQtServerWorker::stop()
    {
        killTimer(m_iTimerID);
        terminate();

        disconnect(this, SIGNAL(requestSend(const QString &, const QByteArray &)),
                   this, SLOT(sendSlot(const QString &, const QByteArray &)));
    }

    void ZMQtServerWorker::send( const QString &UID, const QByteArray &data )
    {
        emit requestSend(UID, data);
    }

    void ZMQtServerWorker::sendSlot( const QString &UID, const QByteArray &data )
    {
        zmq_msg_t *_address = m_pServer->m_addressMap.value(UID);
        if (_address)
        {
            zmq_msg_t _msg;
            // 发送地址信息
            zmq_msg_init(&_msg);
            zmq_msg_copy(&_msg, _address);
            zmq_send(m_pSocket, &_msg, ZMQ_SNDMORE);
            zmq_msg_close(&_msg);
            // 发送一个空信息
            zmq_msg_init_size(&_msg, 0);
            memcpy(zmq_msg_data(&_msg), "", 0);
            zmq_send(m_pSocket, &_msg, ZMQ_SNDMORE);
            zmq_msg_close(&_msg);
            // 发送数据信息
            zmq_msg_init_size(&_msg, data.size());
            memcpy(zmq_msg_data(&_msg), data.data(), data.size());
            zmq_send(m_pSocket, &_msg, 0);
            zmq_msg_close(&_msg);
        }
    }
}

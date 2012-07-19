#include "ZMQtClient_P.h"

#include "ZMQtClient.h"
#include <QtCore/QDateTime>
#include <QtCore/QDataStream>
#include <QtCore/QDebug>

namespace ZMQtNetwork
{
    ZMQtClient_P::ZMQtClient_P( const QString &UID, int io_threads, QObject *parent)
        : QThread(parent)
        , INetworkClient(UID)
        , m_context(io_threads)
        , m_pSocket(NULL)
        , m_bConnected(false)
    {
        // 如果UID为空, 则会尝试用一个随机数和本机IP进行自动UID生成
        if(UID.isEmpty())
        {
            qsrand(QDateTime::currentMSecsSinceEpoch());
            m_UID = QString("%1@%2").arg(qrand(), 0, 16).arg(ZMQtNetwork::getIPList().value(0));
        }
        else
        {
            m_UID = UID;
        }
    }

    ZMQtClient_P::~ZMQtClient_P()
    {

    }

    int ZMQtClient_P::connect( const QString &IP/*="localhost"*/, quint32 port/*=17050*/ )
    {
        if (m_context.getContext())
        {
            close();
            m_pSocket = zmq_socket(m_context.getContext(), ZMQ_DEALER);

            if (m_pSocket)
            {
                int _linger=0;
                zmq_setsockopt(m_pSocket, ZMQ_LINGER, &_linger, sizeof(_linger));
                // 设置客户端UID
                zmq_setsockopt(m_pSocket, ZMQ_IDENTITY, m_UID.toLocal8Bit().data(), m_UID.toLocal8Bit().size());
                // 连接字符串
                QString _connectString= QString("tcp://%1:%2").arg(IP).arg(port);
                // 连接到服务器
                int _rt = zmq_connect(m_pSocket, _connectString.toAscii().data());
                if (_rt == 0)
                {
                    qDebug() << "socket connect success!";
                    qDebug() << "with setting:" << _connectString;

                    // 开始线程
                    m_iTimerID = startTimer(1);
                    start();
                    m_bConnected = true;
                }
                else
                {
                    qCritical() << "socket connect error:" << zmq_strerror(zmq_errno());
                    qCritical() << "with setting:" << _connectString;
                }
                return _rt;
            }
            else
            {
                qCritical() << "socket create error:" << zmq_strerror(zmq_errno());
            }
        }
        else
        {
            qCritical() << "context did not created!";
        }

        return INT_MIN;      
    }

    void ZMQtClient_P::close()
    {
        m_bConnected = false;
        // 结束线程
        terminate();
        // 关闭定时器
        killTimer(m_iTimerID);
        // 关闭socket
        if (m_pSocket)
        {
            int _rt = zmq_close(m_pSocket);
            if (_rt == 0)
            {
                qDebug() << "socket close success!";
            }
            else
            {
                qCritical() << "socket close error:" << zmq_strerror(zmq_errno());
            }
            m_pSocket = NULL;
        }

        QObject::disconnect(this, SIGNAL(requestSend(const QByteArray &)), 
            this, SLOT(sendSlot(const QByteArray &)));
    }

    void ZMQtClient_P::request( const QString &type, const QByteArray &data )
    {
        QByteArray _data;
        QDataStream _stream(&_data, QIODevice::WriteOnly);
        _stream.writeBytes(type.toUtf8().data(), type.toUtf8().size());
        _stream.writeBytes(data.data(), data.size());

        QByteArray _zipData = qCompress(_data);

        send(_zipData);
    }

    void ZMQtClient_P::send( const QByteArray &data )
    {
        emit requestSend(data); 
    }

    void ZMQtClient_P::run()
    {
        QObject::connect(this, SIGNAL(requestSend(const QByteArray &)), 
            this, SLOT(sendSlot(const QByteArray &)), Qt::QueuedConnection);

        exec();
    }

    void ZMQtClient_P::timerEvent( QTimerEvent */*event*/ )
    {
        if (m_pSocket)
        {
            QByteArray _data; 
            int _rc = 0;
            long long _more = 0;
            size_t _more_size = sizeof(_more);
            do
            {
                _more = 0;
                zmq_msg_t _msg;
                _rc = zmq_msg_init(&_msg);
                if (_rc != 0)
                {
                    qCritical() << "message init error:" << zmq_strerror(zmq_errno());
                    zmq_msg_close(&_msg);
                    return;
                }
                _rc = zmq_recv (m_pSocket, &_msg, ZMQ_NOBLOCK);
                if (_rc != 0)
                {
                    if (zmq_errno() != EAGAIN)
                    {
                        qCritical() << "message receive error:" << zmq_strerror(zmq_errno());
                    }
                    zmq_msg_close(&_msg);
                    return;
                }
                _data.append((const char *)zmq_msg_data(&_msg), zmq_msg_size(&_msg));
                _rc = zmq_getsockopt(m_pSocket, ZMQ_RCVMORE, &_more, &_more_size);
                zmq_msg_close(&_msg);
            } while(_more);

            replyDataReceived(_data);
        }
    }

    void ZMQtClient_P::sendSlot( const QByteArray &data )
    {
        if (m_pSocket)
        {
            zmq_msg_t _msg;
            int _rc = zmq_msg_init_size(&_msg, data.size());
            if (_rc != 0)
            {
                qCritical() << "message init error:" << zmq_strerror(zmq_errno());
                zmq_msg_close(&_msg);
                return;
            }
            memcpy(zmq_msg_data(&_msg), data.data(), data.size());

            _rc = zmq_send(m_pSocket, &_msg, 0);
            if (_rc != 0)
            {
                qCritical() << "message send error:" << zmq_strerror(zmq_errno());
            }
            zmq_msg_close(&_msg);
        }
    }

    void ZMQtClient_P::replyDataReceived( const QByteArray &data )
    {
        QByteArray _unzipData = qUncompress(data);
        QDataStream _stream(_unzipData);
        char *_buff = NULL;
        quint32 _size = 0;
        _stream.readBytes(_buff, _size);
        QString _type = QString::fromUtf8(QByteArray::fromRawData(_buff, _size));
        _stream.readBytes(_buff, _size);
        QByteArray _data;
        _data.append(QByteArray::fromRawData(_buff, _size));
        // 发送信号
        emit replyReceived(_type, _data);

        delete[] _buff;
    }

}

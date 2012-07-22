#include "ZMQtSubscriber_P.h"

#include <zmq.h>
#include <QtCore/QDebug>

namespace ZMQtNetwork
{
    ZMQtSubscriber_P::ZMQtSubscriber_P(int io_threads, QObject *parent)
        : QThread(parent)
        , m_context(io_threads)
        , m_pSocket(NULL)
    {

    }

    ZMQtSubscriber_P::~ZMQtSubscriber_P()
    {

    }

    int ZMQtSubscriber_P::connect( const QString &IP/*="localhost"*/, quint32 port/*=17051*/ )
    {
        if (m_context.getContext())
        {
            close();
            m_pSocket = zmq_socket(m_context.getContext(), ZMQ_SUB);

            if (m_pSocket)
            {
                int _linger=0;
                zmq_setsockopt(m_pSocket, ZMQ_LINGER, &_linger, sizeof(_linger));
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

    void ZMQtSubscriber_P::close()
    {
        // 结束线程
        terminate();
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
    }

    void ZMQtSubscriber_P::addTypeFliter( const QString &type )
    {
        if (m_pSocket)
        {
            zmq_setsockopt(m_pSocket, ZMQ_SUBSCRIBE, type.toUtf8().data(), type.toUtf8().size());
            m_typeFliterList << type;
        }
    }

    void ZMQtSubscriber_P::removeTypeFliter( const QString &type )
    {
        if (m_pSocket)
        {
            zmq_setsockopt(m_pSocket, ZMQ_UNSUBSCRIBE, type.toUtf8().data(), type.toUtf8().size());
            m_typeFliterList.removeOne(type);
        }
    }

    void ZMQtSubscriber_P::removeAllTypeFliter()
    {
        if (m_pSocket)
        {
            QString _type;
            QStringListIterator _it(m_typeFliterList);
            while(_it.hasNext())
            {
                _type = _it.next();
                zmq_setsockopt(m_pSocket, ZMQ_UNSUBSCRIBE, _type.toUtf8().data(), _type.toUtf8().size());
            }
            _type.clear();
            zmq_setsockopt(m_pSocket, ZMQ_UNSUBSCRIBE, _type.toUtf8().data(), _type.toUtf8().size());
            m_typeFliterList.clear();
        }
    }

    void ZMQtSubscriber_P::run()
    {
        exec();
    }

    void ZMQtSubscriber_P::timerEvent( QTimerEvent * /*event*/ )
    {
        if (m_pSocket)
        {
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

            // 储存消息类型
            QString _type = QString::fromUtf8((const char *)zmq_msg_data(&_msg), zmq_msg_size(&_msg));

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

            QByteArray _unzipedData;
            try
            {
                _unzipedData = qUncompress(_data);
                if (_unzipedData.isEmpty())
                {
                    _unzipedData = _data;
                }
            }
            catch (...)
            {
                _unzipedData = _data;
            }
            emit dataReceived(_type, _unzipedData);
        }
    }

}

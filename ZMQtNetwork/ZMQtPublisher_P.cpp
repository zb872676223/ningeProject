#include "ZMQtPublisher_P.h"

#include <zmq.h>
#include <QtCore/QDebug>

namespace ZMQtNetwork
{
    ZMQtPublisher_P::ZMQtPublisher_P(int io_threads, QObject *parent)
        : QThread(parent)
        , m_context(io_threads)
        , m_pSocket(NULL)
    {

    }

    ZMQtPublisher_P::~ZMQtPublisher_P()
    {
        close();
    }

    int ZMQtPublisher_P::bind( const QString &IP/*="*"*/, quint32 port/*=17051*/ )
    {
        if (m_context.getContext())
        {
            // 关闭现有连接
            close();

            m_pSocket = zmq_socket(m_context.getContext(), ZMQ_PUB);
            if (m_pSocket)
            {
                int _linger=0;
                zmq_setsockopt(m_pSocket, ZMQ_LINGER, &_linger, sizeof(_linger));
                QString _connectString = QString("tcp://%2:%3").arg(IP).arg(port);

                int _rt = zmq_bind(m_pSocket, _connectString.toAscii().data());
                if (_rt == 0)
                {
                    qDebug() << "socket bind success!";
                    qDebug() << "with setting:" << _connectString;
                }
                else
                {
                    qCritical() << "socket bind error:" << zmq_strerror(zmq_errno());
                    qCritical() << "with setting:" << _connectString;
                }

                //开始服务器主线程
                start();

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

    void ZMQtPublisher_P::publish( const QString &type, const QByteArray &data )
    {
        emit pub(type, data);
    }

    void ZMQtPublisher_P::close()
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

        disconnect(this, SIGNAL(pub(const QString &, const QByteArray &)),
                   this, SLOT(pubSlot(const QString &, const QByteArray &)));
    }

    void ZMQtPublisher_P::run()
    {
        connect(this, SIGNAL(pub(const QString &, const QByteArray &)),
                this, SLOT(pubSlot(const QString &, const QByteArray &)), Qt::QueuedConnection);
        exec();
    }

    void ZMQtPublisher_P::pubSlot(const QString &type, const QByteArray &data )
    {
        zmq_msg_t _msg;
        // 发送类型信息
        zmq_msg_init_size(&_msg, type.toUtf8().size());
        memcpy(zmq_msg_data(&_msg), type.toUtf8().data(), type.toUtf8().size());
        zmq_send(m_pSocket, &_msg, ZMQ_SNDMORE);
        zmq_msg_close(&_msg);
        // 发送数据信息
        QByteArray _zipedData = qCompress(data);
        zmq_msg_init_size(&_msg, _zipedData.size());
        memcpy(zmq_msg_data(&_msg), _zipedData.data(), _zipedData.size());
        zmq_send(m_pSocket, &_msg, 0);
        zmq_msg_close(&_msg);
    }
}

#include "ZMQtServer_P.h"
#include "ZMQtServerWorker.h"
#include <QtCore/QDebug>

namespace ZMQtNetwork
{
    ZMQtServer_P::ZMQtServer_P(int io_threads, QObject *parent)
        : QThread(parent)
        , INetworkServer()
        , m_context(io_threads)
        , m_pSocket(NULL)
        , m_pBackend(NULL)
        , m_pWorker(NULL)
    {
    }

    ZMQtServer_P::~ZMQtServer_P()
    {
        close();

        if (m_pWorker)
        {
            delete m_pWorker;
            m_pWorker = NULL;
        }

    }

    int ZMQtServer_P::bind( const QString &IP/*="*"*/, quint32 port/*=17050*/ )
    {
        if (m_context.getContext())
        {
            // 关闭现有连接
            close();

            // 绑定前端服务器端口
            m_pSocket = zmq_socket(m_context.getContext(), ZMQ_ROUTER);
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
            }
            else
            {
                qCritical() << "socket create error:" << zmq_strerror(zmq_errno());
            }

            // 绑定后端处理端口
            m_pBackend = zmq_socket(m_context.getContext(), ZMQ_DEALER);
            if (m_pBackend)
            {
                int _linger=0;
                zmq_setsockopt(m_pBackend, ZMQ_LINGER, &_linger, sizeof(_linger));
                QString _connectString("inproc://backend");
                int _rt = zmq_bind(m_pBackend, _connectString.toAscii().data());
                if (_rt == 0)
                {
                    qDebug() << "backend bind success!";
                    qDebug() << "with setting:" << _connectString;

                    // 创建一个默认的worker线程, 用于处理接收到的数据
                    if (m_pWorker)
                    {
                        delete m_pWorker;
                    }
                    m_pWorker = new ZMQtServerWorker(this);

                    connect(this, SIGNAL(stopWorker()), m_pWorker, SLOT(stop()), Qt::QueuedConnection);

                    connect(m_pWorker, SIGNAL(dataReceived(const QString &, const QByteArray &)),
                            this, SLOT(requestDataReceived(const QString &, const QByteArray &)), Qt::QueuedConnection);

                    // 后端也绑定完成, 于是开始服务器主线程
                    start();
                }
                else
                {
                    qCritical() << "socket connect error:" << zmq_strerror(zmq_errno());
                    qCritical() << "with setting:" << _connectString;
                }

                return _rt;
            }
        }
        else
        {
            qCritical() << "context did not created!";
        }

        return INT_MIN;

    }

    void ZMQtServer_P::reply( const QString &UID, const QString &type, const QByteArray &data )
    {
        QByteArray _data;
        QDataStream _stream(&_data, QIODevice::ReadWrite);
        //_stream << (quint32)type.toUtf8().size();
        _stream.writeBytes(type.toUtf8().data(), type.toUtf8().size());
        //_stream << (quint32)data.size();
        _stream.writeBytes(data.data(), data.size());

        QByteArray _zipData = qCompress(_data);

        send(UID, _zipData);
    }

    void ZMQtServer_P::close()
    {
        // 终结当前正在进行的线程
        terminate();
        // 停止工作线程
        emit stopWorker();
        // 关闭所有socket
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
        if (m_pBackend)
        {
            int _rt = zmq_close(m_pBackend);
            if (_rt == 0)
            {
                qDebug() << "backend close success!";
            }
            else
            {
                qCritical() << "backend close error:" << zmq_strerror(zmq_errno());
            }

            m_pBackend = NULL;
        }


        disconnect(this, SIGNAL(stopWorker()), m_pWorker, SLOT(stop()));

        disconnect(m_pWorker, SIGNAL(dataReceived(const QString &, const QByteArray &)),
                   this, SLOT(requestDataReceived(const QString &, const QByteArray &)));
    }

    void ZMQtServer_P::run()
    {
        // 调用ZMQ的设备绑定函数, 将接收到的消息交由worker线程处理
        zmq_device (ZMQ_QUEUE, m_pSocket, m_pBackend);
    }

    void ZMQtServer_P::send( const QString &UID, const QByteArray &data )
    {
        // 通过worker线程发送消息
        m_pWorker->send(UID, data);
    }

    void ZMQtServer_P::requestDataReceived( const QString &UID, const QByteArray &data )
    {
        QByteArray _unzipData = qUncompress(data);
        QDataStream _stream(_unzipData);
        quint32 _size = 0;
        char *_buff = 0;
        _stream.readBytes(_buff, _size);
        QString _type = QString::fromUtf8(QByteArray::fromRawData(_buff, _size));
        _stream.readBytes(_buff, _size);
        QByteArray _data;
        _data.append(QByteArray::fromRawData(_buff, _size));
        // 发送信号
        emit requestReceived(UID, _type, _data);

        delete[] _buff;
    }

}

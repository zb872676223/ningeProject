#include "ZMQtServer.h"

#include <QtCore/QDebug>
#include "ZMQtServer_P.h"
#include "ZMQtPublisher_P.h"

namespace ZMQtNetwork
{    
    /*****************************************************
    *
    * ZMQtServer类
    *
    *****************************************************/

    ZMQtServer::ZMQtServer(int io_threads, QObject *parent)
        : QObject(parent)
        , m_pZMQtServer(new ZMQtServer_P(io_threads))
        , m_pZMQtPublisher(new ZMQtPublisher_P(io_threads))
    {
        QObject::connect(m_pZMQtServer, SIGNAL(requestReceived(const QString &, const QString &, const QByteArray &)),
                         this, SIGNAL(requestReceived(const QString &, const QString &, const QByteArray &)), Qt::QueuedConnection);
    }

    ZMQtServer::~ZMQtServer()
    {
        delete m_pZMQtServer;
    }

    int ZMQtServer::bind( const QString &IP, quint32 port )
    {
        return m_pZMQtServer->bind(IP, port);
    }

    void ZMQtServer::close()
    {
        m_pZMQtServer->close();
    }

    void ZMQtServer::reply( const QString &UID, const QString &type, const QByteArray &data )
    {
        m_pZMQtServer->reply(UID, type, data);
    }

    const QList<QString> ZMQtServer::clientUIDList()
    {
        return m_pZMQtServer->clientUIDList();
    }

    int ZMQtServer::bindPublish( const QString &IP/*="*"*/, quint32 port/*=17051*/ )
    {
        return m_pZMQtPublisher->bind(IP, port);
    }

    void ZMQtServer::publish( const QString &type, const QByteArray &data )
    {
        m_pZMQtPublisher->publish(type, data);
    }

    void ZMQtServer::closePublish()
    {
        m_pZMQtPublisher->close();
    }

}

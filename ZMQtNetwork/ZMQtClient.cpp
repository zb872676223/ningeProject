#include "ZMQtClient.h"

#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QHostAddress>
#include "ZMQtClient_P.h"
#include "ZMQtSubscriber_P.h"

namespace ZMQtNetwork
{
    QMap<QString, ZMQtClient_P *> *ZMQtClient::m_pClientList = new QMap<QString, ZMQtClient_P *>();
    QMap<QString, ZMQtSubscriber_P *> *ZMQtClient::m_pSubscriberList = new QMap<QString, ZMQtSubscriber_P *>();

    QStringList getIPList()
    {
        QStringList _IPList;
        QList<QNetworkInterface> _interfaceList = QNetworkInterface::allInterfaces(); 
        for (int i=0; i<_interfaceList.size(); i++)
        {
            QNetworkInterface _interface = _interfaceList.value(i);
            if ( (_interface.flags() & QNetworkInterface::IsUp) &&
                !(_interface.flags() & QNetworkInterface::IsLoopBack)
                )
            {
                QList<QNetworkAddressEntry> _addressEntryList = _interface.addressEntries();
                for(int j=0; j<_addressEntryList.size(); j++)
                {
                    QHostAddress _address = _addressEntryList.value(j).ip();
                    if ( (_address.protocol() == QAbstractSocket::IPv4Protocol) &&
                        (_address != QHostAddress::LocalHost) &&
                        (_address != QHostAddress::Broadcast) )
                    {
                        _IPList << _address.toString();
                    }
                }
            }
        }
        qSort(_IPList);
        return _IPList;
    }

    ZMQtClient::ZMQtClient(const QString &UID, int io_threads, QObject *parent)
        : QObject(parent) 
    {
        if (m_pClientList->keys().contains(UID))
        {
            m_pZMQtClient = m_pClientList->value(UID);
            m_pZMQtSubcriber = m_pSubscriberList->value(UID);
        }
        else
        {
            m_pZMQtClient = new ZMQtClient_P(UID, io_threads);
            m_pZMQtSubcriber = new ZMQtSubscriber_P();
            m_pClientList->insert(m_pZMQtClient->UID(), m_pZMQtClient);
            m_pSubscriberList->insert(m_pZMQtClient->UID(), m_pZMQtSubcriber);
        }

        QObject::connect(m_pZMQtClient, SIGNAL(replyReceived(const QString &, const QByteArray &)),
            this, SIGNAL(replyReceived(const QString &, const QByteArray &)), Qt::QueuedConnection);

        QObject::connect(m_pZMQtSubcriber, SIGNAL(dataReceived(const QString &, const QByteArray &)),
            this, SIGNAL(replyReceived(const QString &, const QByteArray &)), Qt::QueuedConnection);
    }

    QString ZMQtClient::UID()
    {
        return m_pZMQtClient->UID();
    }

    ZMQtClient::~ZMQtClient()
    {
        //m_pClientList->remove(m_pZMQtClient->UID());
        //delete m_pZMQtClient;
    }

    void ZMQtClient::close()
    {
        m_pZMQtClient->close();
    }

    int ZMQtClient::connect( const QString &IP, quint32 port )
    {
        return m_pZMQtClient->connect(IP, port);
    }

    void ZMQtClient::request( const QString &type, const QByteArray &data )
    {
        m_pZMQtClient->request(type, data);
    }

    void ZMQtClient::setUID( const QString &UID )
    {
        m_pClientList->take(m_pZMQtClient->UID());
        m_pZMQtClient->setUID(UID);
        m_pClientList->insert(m_pZMQtClient->UID(), m_pZMQtClient);
    }

    bool ZMQtClient::isConnected()
    {
        return m_pZMQtClient->isConnected();
    }

    int ZMQtClient::connectToPublisher( const QString &IP/*="localhost"*/, quint32 port/*=17051*/ )
    {
        return m_pZMQtSubcriber->connect(IP, port);
    }

    void ZMQtClient::closeSubScriber()
    {
        m_pZMQtSubcriber->close();
    }

    void ZMQtClient::addTypeFliter( const QString &type )
    {
        m_pZMQtSubcriber->addTypeFliter(type);
    }

    void ZMQtClient::removeTypeFliter( const QString &type )
    {
        m_pZMQtSubcriber->removeTypeFliter(type);
    }

    void ZMQtClient::removeAllTypeFliter()
    {
        m_pZMQtSubcriber->removeAllTypeFliter();
    }

}
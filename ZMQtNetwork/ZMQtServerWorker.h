#ifndef ZMQTSERVERWORKER_H
#define ZMQTSERVERWORKER_H

#include "INetwork.h"
#include <zmq.h>
#include <QtCore/QThread>
#include "ZMQtContext.h"

namespace ZMQtNetwork
{
    class ZMQtServer_P;
    class ZMQtServerWorker : public QThread
    {
        Q_OBJECT

    public:
        ZMQtServerWorker(ZMQtServer_P *server, QObject *parent=0);
        ~ZMQtServerWorker();

        public slots:
            void stop();
            void send(const QString &UID, const QByteArray &data);

signals:    
            void requestSend(const QString &UID, const QByteArray &data);
            void dataReceived(const QString &UID, const QByteArray &data);

            private slots:
                void sendSlot(const QString &UID, const QByteArray &data);

    protected:
        void run();
        void timerEvent(QTimerEvent *event);

    private:
        int m_iTimerID;

        ZMQtContext m_context;
        void *m_pSocket;

        ZMQtServer_P *m_pServer;
    };
}
#endif // ZMQTSERVERWORKER_H

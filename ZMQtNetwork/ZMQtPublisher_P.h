#ifndef ZMQTSERVERBROADCAST_P_H
#define ZMQTSERVERBROADCAST_P_H

#include <QtCore/QThread>
#include "INetwork.h"
#include "ZMQtContext.h"

namespace ZMQtNetwork
{
    class ZMQtPublisher_P : public QThread, public INetworkPublisher
    {
        Q_OBJECT

    public:
        ZMQtPublisher_P(int io_threads = 1, QObject *parent=0);
        ~ZMQtPublisher_P();

    public:
        /*
        * 绑定到一个广播端口
        * IP 为本机IP,可以使用 * 代替全部可用的本机IP
        * port 为端口
        */
        virtual int bind(const QString &IP="*", quint32 port=17051);

        /*
        * 广播一条类型为type的数据
        */
        virtual void publish(const QString &type, const QByteArray &data);

        /*
        * 结束绑定
        */
        virtual void close();

signals:
        void pub(const QString &type, const QByteArray &data);

    private slots:
        void pubSlot(const QString &type, const QByteArray &data);

    protected:
        void run();

    private:
        ZMQtContext m_context;
        void *m_pSocket;
    };
}

#endif // ZMQTSERVERBROADCAST_P_H

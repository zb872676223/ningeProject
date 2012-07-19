#ifndef ZMQTSUBSCRIBER_H
#define ZMQTSUBSCRIBER_H

#include <QtCore/QThread>
#include "INetwork.h"
#include "ZMQtContext.h"

namespace ZMQtNetwork
{
    class ZMQtSubscriber_P : public QThread, public INetworkSubscriber
    {
        Q_OBJECT

    public:
        ZMQtSubscriber_P(int io_threads = 1, QObject *parent=0);
        ~ZMQtSubscriber_P();

    public:
        /*
        * 连接到一个广播服务器
        * IP 为服务器IP
        * port 为端口
        */
        virtual int connect(const QString &IP="localhost", quint32 port=17051);

        /*
        * 结束绑定
        */
        virtual void close();

    public:
        /*
        * 增加客户端接收消息过滤器
        */
        virtual void addTypeFliter(const QString &type);
        /*
        * 移除客户端接收消息过滤器
        */
        virtual void removeTypeFliter(const QString &type);
        /*
        * 移除客户端接收消息过滤器
        */
        virtual void removeAllTypeFliter();

    signals:
        void dataReceived(const QString &type, const QByteArray &data);

    protected:
        void run();
        void timerEvent(QTimerEvent* event);

    private:
        int m_iTimerID;
        ZMQtContext m_context;
        void *m_pSocket;
    };
}

#endif // ZMQTSUBSCRIBER_H

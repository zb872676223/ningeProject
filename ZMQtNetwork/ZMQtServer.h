#ifndef ZMQTSERVER_H
#define ZMQTSERVER_H

#include "INetwork.h"
#include <zmq.h>
#include <QtCore/QThread>
#include <QtCore/QMap>

namespace ZMQtNetwork
{
    class ZMQtServer_P;
    class ZMQtPublisher_P;

    class ZMQTNETWORKSHARED_EXPORT ZMQtServer : public QObject
    {
        Q_OBJECT

    public:
        ZMQtServer(int io_threads = 1, QObject *parent=0);
        ~ZMQtServer();

    public:
        /*
        * 绑定到一个端口
        * 重新调用此方法会导致上一次的绑定结束
        * IP 为本机IP,可以使用 * 代替全部可用的本机IP
        * port 为端口
        */
        int bind(const QString &IP="*", quint32 port=17050);

        /*
        * 向某个UID为UID的客户端回复某个类型为type的请求
        */
        void reply(const QString &UID, const QString &type, const QByteArray &data);

        /*
        * 结束绑定
        */
        void close();

        /*
        * 返回全部的客户端UID列表(客户端不一定都可用)
        * TODO: 将不可用的客户端的UID清除
        */
        const QList<QString> clientUIDList();

        /*
        * 向某个UID为UID的客户端发送数据
        */
        void send(const QString &UID, const QByteArray &data);

        /*
        * 绑定广播端口
        */
        int bindPublish(const QString &IP="*", quint32 port=17051);

        /*
        * 广播消息
        */
        void publish(const QString &type, const QByteArray &data);


signals:
        void requestReceived(const QString &UID, const QString &type, const QByteArray &data);

    private:
        ZMQtServer_P *m_pZMQtServer;
        ZMQtPublisher_P *m_pZMQtPublisher;
    };
}

#endif // ZMQTSERVER_H

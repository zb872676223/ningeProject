#ifndef ZMQTSERVER_P_H
#define ZMQTSERVER_P_H

#include <zmq.h>
#include <QtCore/QMap>
#include <QtCore/QThread>
#include "INetwork.h"
#include "ZMQtContext.h"

namespace ZMQtNetwork
{
    class ZMQtServerWorker;

    class ZMQtServer_P : public QThread, public INetworkServer
    {
        Q_OBJECT

    public:
        ZMQtServer_P(int io_threads=1, QObject *parent=0);
        ~ZMQtServer_P();

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
        const QList<QString> clientUIDList() { return m_addressMap.keys(); }

        /*
        * 向某个UID为UID的客户端发送数据
        */
        void send(const QString &UID, const QByteArray &data);

signals:
        void requestReceived(const QString &UID, const QString &type, const QByteArray &data);
        void stopWorker();

    protected:
        void run();

        private slots:
            void requestDataReceived(const QString &UID, const QByteArray &data);
    private:
        ZMQtContext m_context;

        void *m_pSocket;
        void *m_pBackend;

        int m_iTimerID;

        friend class ZMQtServerWorker;
        ZMQtServerWorker *m_pWorker;

        QMap<QString, zmq_msg_t *> m_addressMap;

    };
}
#endif // ZMQTSERVER_P_H

#ifndef ZMQTCLIENT_P_H
#define ZMQTCLIENT_P_H

#include "INetwork.h"
#include <QObject>
#include <QtCore/QThread>
#include <QtCore/QStringList>
#include <zmq.h>
#include "ZMQtContext.h"

namespace ZMQtNetwork
{
    class ZMQtClient_P
            : public QThread
            , public INetworkClient
    {
        Q_OBJECT

    public:
        ZMQtClient_P(const QString &UID, int io_threads=1, QObject *parent=0);
        ~ZMQtClient_P();

    public:
        /*
         * 连接到一个服务器
         * 重新调用此方法会导致上一次的连接断开
         * IP 为服务器IP
         * port 为端口
         */
        int connect(const QString &IP="localhost", quint32 port=17050);

        /*
         * 结束连接
         */
        void close();

        /*
         * 向服务器发送一个类型为type的请求
         */
        void request(const QString &type, const QByteArray &data);

        /*
         * 判断是否已连接
         */
        bool isConnected() { return m_bConnected; }

        /*
         * 向服务器发送数据
         */
        void send(const QByteArray &data);

    signals:
        void replyReceived(const QString &type, const QByteArray &data);
        void requestSend(const QByteArray &data);

    protected:
        void run();
        void timerEvent(QTimerEvent *event);

    private slots:
        void sendSlot(const QByteArray &data);
        void replyDataReceived(const QByteArray &data);

    private:
        ZMQtContext m_context;

        void *m_pSocket;
        bool m_bConnected;

        int m_iTimerID;

    };
}
#endif // ZMQTCLIENT_P_H

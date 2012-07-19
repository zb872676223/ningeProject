#ifndef ZMQTCLIENT_H
#define ZMQTCLIENT_H

#include "INetwork.h"
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <zmq.h>

namespace ZMQtNetwork
{
    class ZMQtClient_P; 
    class ZMQtSubscriber_P;

    /*
    * 工具函数: 获取本机的IP地址列表
    */
    ZMQTNETWORKSHARED_EXPORT QStringList getIPList();

    /*
    * ZMQt客户端类
    */
    class ZMQTNETWORKSHARED_EXPORT ZMQtClient : public QObject
    {
        Q_OBJECT

    public:   
        /*
        * 要使用这个模块必须指定自己的UID信息, 可以任意, 但必须具有唯一性, 
        * 若为"", 则会使用以下命名方式 XXXX@XXX.XXX.XXX.XXX, 
        * 其中@字符前为随机的16进制数字, @字符后面为机器的第一个可用IP
        */
        ZMQtClient(const QString &UID, int io_threads=1, QObject *parent=0);
        ~ZMQtClient();

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
        * 设置客户端UID
        * !注意! 这是设置下一次连接服务器时将使用的UID! 
        * 也就是说如果已经连接到服务器, 此函数无法改变客户端的UID!
        */
        void setUID(const QString &UID);
        
        /*
        * 获取客户端UID
        */
        QString UID();

    public:
        /*
        * 连接到一个发布者
        * IP 为发布者IP
        * port 为端口
        */
        int connectToPublisher(const QString &IP="localhost", quint32 port=17051);

        /*
        * 结束接受者连接
        */
        void closeSubScriber();
        
        /*
        * 增加客户端接收消息过滤器
        */
        void addTypeFliter(const QString &type);
        /*
        * 移除客户端接收消息过滤器
        */
        void removeTypeFliter(const QString &type);
        /*
        * 移除客户端接收消息过滤器
        */
        void removeAllTypeFliter();

    public:
        bool isConnected();

signals:
        void replyReceived(const QString &type, const QByteArray &data);

    private:
        static QMap<QString, ZMQtClient_P *> *m_pClientList;
        static QMap<QString, ZMQtSubscriber_P *> *m_pSubscriberList;

        ZMQtClient_P *m_pZMQtClient;
        ZMQtSubscriber_P *m_pZMQtSubcriber;

        static QStringList m_IPList;
    };
}

#endif // ZMQTCLIENT_H

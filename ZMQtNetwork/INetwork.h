#ifndef INETWORK_H
#define INETWORK_H

#include "ZMQtNetwork_global.h"
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

namespace ZMQtNetwork
{
    class INetworkClient
    {

    public:      
        /*
        * 要使用这个接口必须指定自己的UID信息, 可以任意, 但必须具有唯一性
        * 用于向服务器唯一标示自己
        */
        INetworkClient(const QString &UID): m_UID(UID) {}
        virtual ~INetworkClient() {}

        // 纯虚函数, 需实现
    public:
        /*
        * 连接到一个服务器
        * IP 为服务器IP
        * port 为端口
        */
        virtual int connect(const QString &IP="localhost", quint32 port=17050)=0;

        /*
        * 向服务器发送数据
        */
        virtual void send(const QByteArray &data)=0;

        /*
        * 结束连接
        */
        virtual void close()=0;

    public:
        /*
        * 设置客户端UID
        */
        virtual void setUID(const QString &UID) { m_UID = UID; }
        /*
        * 获取当前客户端UID
        */
        QString UID() { return m_UID; }

    protected:
        QString m_UID;

    };

    class INetworkServer
    {
    public:
        INetworkServer() {}
        virtual ~INetworkServer() {}

    public:
        /*
        * 绑定到一个端口
        * IP 为本机IP,可以使用 * 代替全部可用的本机IP
        * port 为端口
        */
        virtual int bind(const QString &IP="*", quint32 port=17050) = 0;

        /*
        * 向某个UID为UID的客户端发送数据
        */
        virtual void send(const QString &UID, const QByteArray &data) = 0;

        /*
        * 结束绑定
        */
        virtual void close() = 0;

    };

    class INetworkPublisher
    {
    public:
        INetworkPublisher() {}
        virtual ~INetworkPublisher() {}

    public:
        /*
        * 绑定到一个广播端口
        * IP 为本机IP,可以使用 * 代替全部可用的本机IP
        * port 为端口
        */
        virtual int bind(const QString &IP="*", quint32 port=17051) = 0;

        /*
        * 广播一条类型为type的数据
        */
        virtual void publish(const QString &type, const QByteArray &data) = 0;

        /*
        * 结束绑定
        */
        virtual void close() = 0;
    };

    class INetworkSubscriber
    {
    public:
        /*
        * 要使用这个接口须指定想要接收的消息类型, 用于过滤消息
        */
        INetworkSubscriber() {}
        virtual ~INetworkSubscriber() {}

    public:
        /*
        * 连接到一个广播服务器
        * IP 为服务器IP
        * port 为端口
        */
        virtual int connect(const QString &IP="localhost", quint32 port=17051)=0;

        /*
        * 结束绑定
        */
        virtual void close() = 0;

    public:
        /*
        * 增加客户端接收消息过滤器
        */
        virtual void addTypeFliter(const QString &type) = 0;
        /*
        * 移除客户端接收消息过滤器
        */
        virtual void removeTypeFliter(const QString &type) = 0;
        /*
        * 移除客户端接收消息过滤器
        */
        virtual void removeAllTypeFliter() = 0;
        /*
        * 获取当前客户端接收消息的类型
        */
        const QStringList& typeFliter() { return m_typeFliterList; }

    protected:
        QStringList m_typeFliterList;
    };
}

#endif // INETWORK_H

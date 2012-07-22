#ifndef ZMQTCONTEXT_H
#define ZMQTCONTEXT_H

namespace ZMQtNetwork
{
    class ZMQtContext
    {
    public:
        ZMQtContext(int io_threads=1);
        ~ZMQtContext();

        void* getContext() { return m_pContext; }

    private:
        static void *m_pContext;
        static int m_iContextCount;
    };
}

#endif // ZMQTCONTEXT_H

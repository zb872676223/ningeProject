#include "ZMQtContext.h"
#include <zmq.h>
#include <QtCore/QDebug>

namespace ZMQtNetwork
{
    void *ZMQtContext::m_pContext = NULL;
    int ZMQtContext::m_iContextCount = 0;

    ZMQtContext::ZMQtContext(int io_threads)
    {
        if (!m_pContext && io_threads >= 0)
        {
            m_pContext = zmq_init(io_threads);
            if (!m_pContext)
            {
                qCritical() << "context create error!";
                return;
            }
        }
        ++m_iContextCount;
    }

    ZMQtContext::~ZMQtContext()
    {
        --m_iContextCount;
        if (m_iContextCount == 0 && m_pContext)
        {
            zmq_term(m_pContext);
            m_pContext = NULL;
        }
    }
}

#ifndef ZMQTNETWORK_GLOBAL_H
#define ZMQTNETWORK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ZMQTNETWORK_LIBRARY)
#  define ZMQTNETWORKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ZMQTNETWORKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ZMQTNETWORK_GLOBAL_H
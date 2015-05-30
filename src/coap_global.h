#ifndef COAP_GLOBAL_H
#define COAP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COAP_MAKE_LIBRARY)
#  define COAP_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define COAP_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COAP_GLOBAL_H

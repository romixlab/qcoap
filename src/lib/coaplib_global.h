#ifndef COAPLIB_GLOBAL_H
#define COAPLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MAKE_COAPLIB)
#  define COAPLIB_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define COAPLIB_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COAPLIB_GLOBAL_H

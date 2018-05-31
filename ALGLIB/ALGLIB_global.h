#ifndef ALGLIB_GLOBAL_H
#define ALGLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMPILE_ALG_LIB)
#  define ALG_LIB_DECL  Q_DECL_EXPORT
#elif defined(USE_ALG_LIB)
#  define ALG_LIB_DECL Q_DECL_IMPORT
#else
#  define ALG_LIB_DECL
#endif

#endif // ALGLIB_GLOBAL_H

#ifndef XLSLIB_GLOBAL_H
#define XLSLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XLSLIB_LIBRARY)
#  define XLSLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XLSLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XLSLIB_GLOBAL_H

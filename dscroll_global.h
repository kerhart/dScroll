#ifndef DSCROLL_GLOBAL_H
#define DSCROLL_GLOBAL_H

#include <QtGlobal>

#if defined(DSCROLL_LIBRARY)
#  define DSCROLLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DSCROLLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DSCROLL_GLOBAL_H

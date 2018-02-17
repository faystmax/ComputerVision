#ifndef IMAGES_GLOBAL_H
#define IMAGES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IMAGES_LIBRARY)
#  define IMAGESSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IMAGESSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // IMAGES_GLOBAL_H

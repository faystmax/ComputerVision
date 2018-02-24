#ifndef CORECREATOR_H
#define CORECREATOR_H

#include <QSharedPointer>

#include "images_global.h"
#include "Kernel.h"

class IMAGESSHARED_EXPORT CoreCreator {
public:
    CoreCreator();
    static QSharedPointer<Kernel> getBlur();
    static QSharedPointer<Kernel> getClarity();
    static QSharedPointer<Kernel> getSobelX();
    static QSharedPointer<Kernel> getSobelY();
    static QSharedPointer<Kernel> getPriutX();
    static QSharedPointer<Kernel> getPriutY();
    static QSharedPointer<Kernel> getGauss(int width, int height, double sigma);
};

#endif // CORECREATOR_H

#ifndef CORECREATOR_H
#define CORECREATOR_H

#include <QSharedPointer>

#include "images_global.h"
#include "Core.h"

class IMAGESSHARED_EXPORT CoreCreator {
public:
    CoreCreator();
    static QSharedPointer<Core> getBlur();
    static QSharedPointer<Core> getClarity();
    static QSharedPointer<Core> getSobelX();
    static QSharedPointer<Core> getSobelY();
    static QSharedPointer<Core> getPriutX();
    static QSharedPointer<Core> getPriutY();
    static QSharedPointer<Core> getGauss(int width, int height, double sigma);
};

#endif // CORECREATOR_H

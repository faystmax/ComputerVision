#ifndef CORECREATOR_H
#define CORECREATOR_H

#include "images_global.h"
#include <Core.h>

class IMAGESSHARED_EXPORT CoreCreator {
public:
    CoreCreator();
    static Core* getBlur();
    static Core* getClarity();
    static Core* getSobelX();
    static Core* getSobelY();
    static Core* getPriutX();
    static Core* getPriutY();
    static Core* getGauss(int width, int height, double sigma);

};

#endif // CORECREATOR_H

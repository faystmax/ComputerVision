#ifndef CORECREATOR_H
#define CORECREATOR_H

#include "images_global.h"
#include <Core.h>

class IMAGESSHARED_EXPORT CoreCreator {
public:
    CoreCreator();
    static Core* getBlur();
    static Core* getClarity();

};

#endif // CORECREATOR_H

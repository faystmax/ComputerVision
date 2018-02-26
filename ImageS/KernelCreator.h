#ifndef CORECREATOR_H
#define CORECREATOR_H

#include <memory>

#include "images_global.h"
#include "Kernel.h"

using namespace std;

class IMAGESSHARED_EXPORT KernelCreator {
public:
    KernelCreator();
    static unique_ptr<Kernel> getSame();
    static unique_ptr<Kernel> getBlur();
    static unique_ptr<Kernel> getClarity();
    static unique_ptr<Kernel> getSobelX();
    static unique_ptr<Kernel> getSobelY();
    static unique_ptr<Kernel> getPriutX();
    static unique_ptr<Kernel> getPriutY();
    static unique_ptr<Kernel> getGauss(int width, int height, double sigma);
};

#endif // CORECREATOR_H

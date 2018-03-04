#ifndef CORECREATOR_H
#define CORECREATOR_H

#include "images_global.h"
#include "Kernel.h"

class IMAGESSHARED_EXPORT KernelCreator {
public:
    KernelCreator() = default;
    static Kernel getSame();
    static Kernel getBlur();
    static Kernel getClarity();
    static Kernel getSobelX();
    static Kernel getSobelY();
    static Kernel getPriutX();
    static Kernel getPriutY();
    static Kernel getGauss(const double sigma);
    static Kernel getGauss(const double sigma, const int radius);
    static Kernel getGaussDoubleDim(int width, int height, double sigma);
};

#endif // CORECREATOR_H

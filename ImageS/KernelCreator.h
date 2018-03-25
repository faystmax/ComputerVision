#ifndef CORECREATOR_H
#define CORECREATOR_H

#include "images_global.h"
#include "Kernel.h"

class IMAGESSHARED_EXPORT KernelCreator {
public:
    static Kernel getSame();
    static Kernel getBlur();
    static Kernel getShift();
    static Kernel getClarity();
    static Kernel getSobelX();
    static Kernel getSobelY();
    static Kernel getPriutX();
    static Kernel getPriutY();
    static Kernel getGauss(const double sigma);
    static Kernel getGaussDoubleDim(const double sigma);
    static double getGaussValue(const int i, const int j, const double sigma);
    static Kernel getGauss(const double sigma, const int radius);
    static Kernel getGaussDoubleDim(const int width, const int height, const double sigma);
};

#endif // CORECREATOR_H

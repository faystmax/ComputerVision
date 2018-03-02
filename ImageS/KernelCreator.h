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
    static Kernel getGauss(double sigma);
    static Kernel getGaussX(double sigma);
    static Kernel getGaussY(double sigma);
    static Kernel getGauss(int width, int height, double sigma);

private:
    static int getGaussSize(double sigma);
};

#endif // CORECREATOR_H

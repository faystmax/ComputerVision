#include "KernelCreator.h"
#include <math.h>
#include <omp.h>

KernelCreator::KernelCreator() {
}

unique_ptr <Kernel> KernelCreator::getBlur() {
    const double koef = 1.0 / 9;
    double *core = new double[9]{koef, koef, koef,
                                 koef, koef, koef,
                                 koef, koef, koef};
    return unique_ptr<Kernel>(new Kernel(3, 3, core));
}

unique_ptr <Kernel> KernelCreator::getClarity() {
    double *core = new double[9]{-1, -1, -1,
                                 -1, 9, -1
                                        - 1, -1, -1};
    return unique_ptr<Kernel>(new Kernel(3, 3, core));
}

unique_ptr <Kernel> KernelCreator::getSobelX() {
    double *core = new double[9]{1, 0, -1,
                                 2, 0, -2,
                                 1, 0, -1};

    return unique_ptr<Kernel>(new Kernel(3, 3, core));
}

unique_ptr <Kernel> KernelCreator::getSobelY() {
    double *core = new double[9]{1, 2, 1,
                                 0, 0, 0,
                                 -1, -2, -1};
    return unique_ptr<Kernel>(new Kernel(3, 3, core));
}

unique_ptr <Kernel> KernelCreator::getPriutX() {
    double *core = new double[9]{1, 0, -1,
                                 1, 0, -1,
                                 1, 0, -1};
    return unique_ptr<Kernel>(new Kernel(3, 3, core));
}

unique_ptr <Kernel> KernelCreator::getPriutY() {
    double *core = new double[9]{1, 1, 1,
                                 0, 0, 0,
                                 -1, -1, -1};
    return unique_ptr<Kernel>(new Kernel(3, 3, core));
}

unique_ptr <Kernel> KernelCreator::getGauss(int width, int height, double sigma) {
    // Tmp vars
    double sum = 0.0;
    double doubleSigma = 2 * sigma * sigma;
    double mainKoef = 1.0 / (doubleSigma * M_PI);
    double halfWidth = width / 2;
    double halfHeight = height / 2;

    double *core = new double[width * height];
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            core[i + j * width] =
                    mainKoef * exp(((i - halfWidth) * (i - halfWidth) + (j - halfHeight) * (j - halfHeight)) *
                                   (-1.0 / doubleSigma));
            sum += core[i + j * width];
        }
    }

    // Normalize
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            core[i + j * width] /= sum;
        }
    }
    return unique_ptr<Kernel>(new Kernel(width, height, core));

}


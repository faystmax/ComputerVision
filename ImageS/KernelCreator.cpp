#include "KernelCreator.h"
#include <math.h>
#include <omp.h>

CoreCreator::CoreCreator() {
}

QSharedPointer <Kernel> CoreCreator::getBlur() {
    const double koef = 1.0 / 9;
    double *core = new double[9]{koef, koef, koef,
                                 koef, koef, koef,
                                 koef, koef, koef};
    return QSharedPointer<Kernel>(new Kernel(3, 3, core));
}

QSharedPointer <Kernel> CoreCreator::getClarity() {
    double *core = new double[9]{-1, -1, -1,
                                 -1, 9, -1
                                        - 1, -1, -1};
    return QSharedPointer<Kernel>(new Kernel(3, 3, core));
}

QSharedPointer <Kernel> CoreCreator::getSobelX() {
    double *core = new double[9]{1, 0, -1,
                                 2, 0, -2,
                                 1, 0, -1};

    return QSharedPointer<Kernel>(new Kernel(3, 3, core));
}

QSharedPointer <Kernel> CoreCreator::getSobelY() {
    double *core = new double[9]{1, 2, 1,
                                 0, 0, 0,
                                 -1, -2, -1};
    return QSharedPointer<Kernel>(new Kernel(3, 3, core));
}

QSharedPointer <Kernel> CoreCreator::getPriutX() {
    double *core = new double[9]{1, 0, -1,
                                 1, 0, -1,
                                 1, 0, -1};
    return QSharedPointer<Kernel>(new Kernel(3, 3, core));
}

QSharedPointer <Kernel> CoreCreator::getPriutY() {
    double *core = new double[9]{1, 1, 1,
                                 0, 0, 0,
                                 -1, -1, -1};
    return QSharedPointer<Kernel>(new Kernel(3, 3, core));
}

QSharedPointer <Kernel> CoreCreator::getGauss(int width, int height, double sigma) {
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
    return QSharedPointer<Kernel>(new Kernel(width, height, core));

}


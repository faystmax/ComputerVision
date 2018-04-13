#include "KernelCreator.h"
#include <iostream>
#include <math.h>


Kernel KernelCreator::getSame() {
    vector<double> core = {0, 0, 0,
                           0, 1, 0,
                           0, 0, 0
                          };
    return Kernel(3, 3, core);
}

Kernel KernelCreator::getBlur() {
    const double koef = 1.0 / 9;
    vector<double> core = { koef, koef, koef,
                            koef, koef, koef,
                            koef, koef, koef
                          };
    return Kernel(3, 3, core);
}

Kernel KernelCreator::getShift() {
    vector<double> core = {0, 1, 0,
                           0, 0, 0,
                           0, 0, 0
                          };
    return Kernel(3, 3, core);
}

Kernel KernelCreator::getClarity() {
    vector<double> core = { -1, -1, -1,
                            -1,  9, -1,
                            -1, -1, -1
                          };
    return Kernel(3, 3, core);
}

Kernel KernelCreator::getSobelX() {
    vector<double> core = { 1, 0, -1,
                            2, 0, -2,
                            1, 0, -1
                          };
    return Kernel(3, 3, core);
}

Kernel KernelCreator::getSobelY() {
    vector<double> core = { 1,  2,  1,
                            0,  0,  0,
                            -1, -2, -1
                          };
    return Kernel(3, 3, core);
}

Kernel KernelCreator::getPriutX() {
    vector<double> core = { 1, 0, -1,
                            1, 0, -1,
                            1, 0, -1
                          };
    return Kernel(3, 3, core);
}

Kernel KernelCreator::getPriutY() {
    vector<double> core = { 1,  1,   1,
                            0,  0,   0,
                            -1, -1, -1
                          };
    return Kernel(3, 3, core);
}

Kernel KernelCreator::getScos() {
    //TODO
    vector<double> core = { 1,  0,   0,
                            0,  -1,  0,
                            0,  0,   1
                          };
    return Kernel(3, 3, core);
}

Kernel KernelCreator::getGauss(const double sigma) {
    int radius = sigma * 6;
    if (radius % 2 == 0) {
        radius++;
    }
    return getGauss(sigma, radius);
}

Kernel KernelCreator::getGaussDoubleDim(const double sigma) {
    int radius = sigma * 6;
    if (radius % 2 == 0) {
        radius++;
    }
    return getGaussDoubleDim(radius, radius, sigma);
}

double KernelCreator::getGaussValue(const int i, const int j, const double sigma, const int radius) {
    return (1.0 / (2 * sigma * sigma * M_PI)) * exp(((i - radius) * (i - radius) + (j - radius) * (j - radius)) * (-1.0 / (2 * sigma * sigma)));
}

Kernel KernelCreator::getGauss(const double sigma, const int radius) {
    double sum = 0;
    double doubleSigma = 2 * sigma * sigma;
    double mainKoef = 1 / (sqrt(2 * M_PI) * sigma);

    vector<double> core(radius);
    for (int i = 0; i < radius; i++) {
        core[i] = mainKoef * exp(-(pow(i - (radius / 2), 2)) / doubleSigma);
        sum += core[i];
    }
    // Normalize
    for (int i = 0; i < radius; i++) {
        core[i] /= sum;
    }
    return Kernel(1, radius, core);;
}


Kernel KernelCreator::getGaussDoubleDim(const int width,const int height,const double sigma) {
    // Tmp vars
    double sum = 0.0;
    double doubleSigma = 2 * sigma * sigma;
    double mainKoef = 1.0 / (doubleSigma * M_PI);
    double halfWidth = width / 2;
    double halfHeight = height / 2;

    vector<double> core(width * height);
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
    return Kernel(width, height, core);
}


#include "KernelCreator.h"
#include <math.h>
#include <iostream>

Kernel KernelCreator::getSame() {

    double core[9] = {0, 0, 0,
                      0, 1, 0,
                      0, 0, 0};
    vector<double> data(core, core + 9);
    return Kernel(3, 3, data);
}

Kernel KernelCreator::getBlur() {
    const double koef = 1.0 / 9;
    double core[9] = {koef, koef, koef,
                      koef, koef, koef,
                      koef, koef, koef};
    vector<double> data(core, core + 9);
    return Kernel(3, 3, data);
}

Kernel KernelCreator::getClarity() {
    double core[9] = {-1, -1, -1,
                      -1, 9, -1,
                      -1, -1, -1};
    vector<double> data(core, core + 9);
    return Kernel(3, 3, data);
}

Kernel KernelCreator::getSobelX() {
    double core[9] = {1, 0, -1,
                      2, 0, -2,
                      1, 0, -1};
    vector<double> data(core, core + 9);
    return Kernel(3, 3, data);
}

Kernel KernelCreator::getSobelY() {
    double core[9] = {1, 2, 1,
                      0, 0, 0,
                      -1, -2, -1};
    vector<double> data(core, core + 9);
    return Kernel(3, 3, data);
}

Kernel KernelCreator::getPriutX() {
    double core[9] = {1, 0, -1,
                      1, 0, -1,
                      1, 0, -1};
    vector<double> data(core, core + 9);
    return Kernel(3, 3, data);
}

Kernel KernelCreator::getPriutY() {
    double core[9] = {1, 1, 1,
                      0, 0, 0,
                      -1, -1, -1};
    vector<double> data(core, core + 9);
    return Kernel(3, 3, data);
}

Kernel KernelCreator::getGauss(const double sigma) {
    int radius = sigma * 6;
    if (radius % 2 == 0) radius++;
    return getGauss(sigma, radius);
}

Kernel KernelCreator::getGauss(const double sigma, const int radius) {
    double sum = 0;
    double doubleSigma = 2 * sigma * sigma;
    double mainKoef = 1 / sqrt(2 * sigma * sigma);

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


Kernel KernelCreator::getGaussDoubleDim(int width, int height, double sigma) {
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


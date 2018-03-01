#include "KernelCreator.h"
#include <math.h>

KernelCreator::KernelCreator() {
}

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

Kernel KernelCreator::getGauss(double sigma) {
    int razmer = (int) (sigma * 3);
    if (razmer % 2 == 0) razmer++;
    return getGauss(razmer, razmer, sigma);
}

Kernel KernelCreator::getGauss(int width, int height, double sigma) {
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


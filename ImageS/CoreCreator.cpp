#include "CoreCreator.h"
#include <math.h>
#include <omp.h>
#include <iostream>

CoreCreator::CoreCreator() {
}

Core *CoreCreator::getBlur() {
    const double koef = 1.0 / 9;
    double core[3][3] = {{koef, koef, koef},
                         {koef, koef, koef},
                         {koef, koef, koef}};
    return new Core(3, 3, core);
}

Core *CoreCreator::getClarity() {
    double core[3][3] = {{-1, -1, -1},
                         {-1, 9,  -1},
                         {-1, -1, -1}};
    return new Core(3, 3, core);
}

Core *CoreCreator::getSobelX() {
    double core[3][3] = {{1, 0, -1},
                         {2, 0, -2},
                         {1, 0, -1}};

    return new Core(3, 3, core);
}

Core *CoreCreator::getSobelY() {
    double core[3][3] = {{1,  2,  1},
                         {0,  0,  0},
                         {-1, -2, -1}};
    return new Core(3, 3, core);
}

Core *CoreCreator::getPriutX() {
    double core[3][3] = {{1, 0, -1},
                         {1, 0, -1},
                         {1, 0, -1}};
    return new Core(3, 3, core);
}

Core *CoreCreator::getPriutY() {
    double core[3][3] = {{1,  1,  1},
                         {0,  0,  0},
                         {-1, -1, -1}};
    return new Core(3, 3, core);
}

Core *CoreCreator::getGauss(int width, int height, double sigma) {
    // Tmp vars
    double sum = 0.0;
    double doubleSigma = 2 * sigma * sigma;
    double mainKoef = 1.0 / (doubleSigma * M_PI);
    double halfWidth = width / 2;
    double halfHeight = height / 2;

    double **core = new double *[width];
    #pragma omp parallel for
    for (int i = 0; i < width; i++) {
        core[i] = new double[height];
        #pragma omp parallel for
        for (int j = 0; j < height; j++) {
            core[i][j] = mainKoef * exp(((i - halfWidth) * (i - halfWidth) + (j - halfHeight) * (j - halfHeight)) *
                                        (-1.0 / doubleSigma));
            sum += core[i][j];
        }
    }

    // Normalize
    #pragma omp parallel for
    for (int i = 0; i < width; i++) {
        #pragma omp parallel for
        for (int j = 0; j < height; j++) {
            core[i][j] /= sum;
        }
    }


    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            std::cout << core[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return new Core(width, height, core);

}


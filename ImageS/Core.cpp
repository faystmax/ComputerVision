#include "Core.h"

#include "memory"

Core::Core() {
}

Core::Core(int width, int height, double **core) {
    this->width = width;
    this->height = height;
    this->core = core;
}

Core::Core(int width, int height, double core[][3])
{
    this->width = width;
    this->height = height;

    // Create pointer to pointer and initialize it
    this->core = new double *[3];
    for (int i = 0; i < 3; i++) {
        this->core[i] = new double[3];
        for (int j = 0; j < 3; j++) {
            this->core[i][j] = core[i][j];
        }
    }
}

Core::~Core() {
    for (int i = 0; i < height; i++) {
        delete[] this->core[i];
    }
    delete[] this->core;
}

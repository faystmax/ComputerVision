#include "Kernel.h"
#include <iostream>

Kernel::Kernel(int width, int height, vector<double> core)
    :core(core) {
    this->width = width;
    this->height = height;
}

void Kernel::rotate() {
    std::swap(this->width, this->height);
}

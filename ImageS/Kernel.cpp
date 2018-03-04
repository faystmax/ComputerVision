#include "Kernel.h"
#include <iostream>

Kernel::Kernel(int width, int height, vector<double> core){
    this->width = width;
    this->height = height;
    this->core = std::move(core);
}

void Kernel::rotate() {
    std::swap(this->width, this->height);
}

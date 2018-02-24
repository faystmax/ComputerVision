#include "Kernel.h"

Kernel::Kernel(int width, int height, double *core)
    :core(core){
    this->width = width;
    this->height = height;
}

Kernel::~Kernel() {
}

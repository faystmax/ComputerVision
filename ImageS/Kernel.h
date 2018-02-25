#ifndef KERNEL_H
#define KERNEL_H

#include "images_global.h"
#include <memory>

using namespace std;

class IMAGESSHARED_EXPORT Kernel {
public:
    Kernel(int width, int height, double* core);
    ~Kernel();

    int getHeight() const {return height;}
    int getWidth() const {return width;}
    double getCoreAt(int x, int y) const {return core[x + y * width];}

private:
    int width;
    int height;
    unique_ptr<double[]> core;
};

#endif // KERNEL_H

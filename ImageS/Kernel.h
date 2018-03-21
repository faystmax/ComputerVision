#ifndef KERNEL_H
#define KERNEL_H

#include "images_global.h"
#include <memory>

using namespace std;

class IMAGESSHARED_EXPORT Kernel {
public:
    Kernel(int width, int height, vector<double> core);
    Kernel(Kernel&&) = default;
    Kernel& operator=(const Kernel& kernel) = default;
    Kernel& operator=(Kernel&&) = default;
    ~Kernel() = default;

    int getHeight() const {return height;}
    int getWidth() const {return width;}
    double get(int x, int y) const {return core[x + y * width];}
    void rotate();

private:
    int width;
    int height;
    vector<double> core;
};

#endif // KERNEL_H

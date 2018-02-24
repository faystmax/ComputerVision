#ifndef KERNEL_H
#define KERNEL_H

#include <QScopedArrayPointer>

class Kernel {
public:
    Kernel(int width, int height, double* core);
    ~Kernel();

    int getHeight() const {return height;}
    int getWidth() const {return width;}
    double getCoreAt(int x, int y) const {return core[x + y * width];}

private:
    int width;
    int height;
    QScopedArrayPointer<double> core;
};

#endif // KERNEL_H

#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <memory>

#include "images_global.h"
#include "Kernel.h"

using namespace std;

class IMAGESSHARED_EXPORT Image{

public:
    enum EdgeEffect{ Black, Repeat, Mirror,  Wrapping };

    Image(const Image& copy);
    Image(const QImage &image);
    Image(int width ,int height);
    Image(const QImage &image, EdgeEffect edgeEffect);
    ~Image() = default;

    QImage& getOutputImage();

    double getPixel(int x, int y);
    void setPixel(int x, int y, double pixel);

    int getHeight() const {return height;}
    int getWidth() const {return width;}

    void setEdgeEffect(EdgeEffect edgeEffect) {this->edgeEffect = edgeEffect;}
    EdgeEffect getEdgeEffect() {return this->edgeEffect;}

private:
    int height;
    int width;
    EdgeEffect edgeEffect;
    unique_ptr<double[]> pixels;


    double getPixelRepeat(int x ,int y);
    double getPixelMirror(int x ,int y);
    double getPixelWrapping(int x ,int y);
};

#endif // IMAGE_H

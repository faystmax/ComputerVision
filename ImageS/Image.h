#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>

#include "images_global.h"
#include "Core.h"

class IEdgeEffect;

class IMAGESSHARED_EXPORT Image{

public:
    Image(int width ,int height);
    Image(const QImage &image, IEdgeEffect* edgeEffect);
    ~Image();

    QImage& getOutputImage();

    double getPixel(int x, int y);
    void setPixel(int x, int y, double pixel);

    int getHeight() const {return height;}
    void setHeight(int value) {height = value;}
    int getWidth() const {return width;}
    void setWidth(int value) {width = value;}

    double **getPixels() const {return pixels;}

private:
    int height;
    int width;
    double** pixels;
    IEdgeEffect* edgeEffect;


};

#endif // IMAGE_H

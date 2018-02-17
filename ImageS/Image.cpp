#include "IEdgeEffect.h"
#include<iostream>


Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    this->pixels = new double *[width];
    for (int i = 0; i < width; i++) {
        this->pixels[i] = new double[height];
    }
}

Image::Image(const QImage &image, IEdgeEffect *edgeEffect) {
    this->width = image.width();
    this->height = image.height();
    this->edgeEffect = edgeEffect;
    this->pixels = new double *[width];

    // read pixels and form black and white image
    for (int i = 0; i < width; i++) {
        this->pixels[i] = new double[height];
        for (int j = 0; j < height; j++) {
            QRgb pixel = image.pixel(i, j);
            this->pixels[i][j] = 0.213 * qRed(pixel) + 0.715 * qGreen(pixel) + 0.072 * qBlue(pixel);
        }
    }
}

Image::~Image() {
    for (int i = 0; i < width; i++) {
        delete[] this->pixels[i];
    }
    delete[] this->pixels;
}

QImage &Image::getOutputImage() {
    QImage *image = new QImage(this->width, this->height, QImage::Format_ARGB32);
    for (int i = 0; i < this->width; i++) {
        for (int j = 0; j < this->height; j++) {
            image->setPixel(i, j, qRgb(this->pixels[i][j], this->pixels[i][j], this->pixels[i][j]));

        }
    }
    return *image;
}

double Image::getPixel(int x, int y) {
    return edgeEffect->getPixel(x, y, *this);
}

void Image::setPixel(int x, int y, double pixel) {
    pixels[x][y] = pixel;
}

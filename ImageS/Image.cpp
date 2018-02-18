#include "IEdgeEffect.h"
#include "BlackEdgeEffect.h"
#include "Image.h"
#include <omp.h>

Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    this->pixels = new double *[width];
    for (int i = 0; i < width; i++) {
        this->pixels[i] = new double[height];
    }
    this->edgeEffect = QSharedPointer<IEdgeEffect>(new BlackEdgeEffect());
}

Image::Image(Image& copy)
{
    this->width = copy.width;
    this->height = copy.height;
    this->pixels = new double *[width];

    // Copy Pixels
    #pragma omp parallel for
    for (int i = 0; i < width; i++) {
        this->pixels[i] = new double[height];
        #pragma omp parallel for
        for (int j = 0; j < height; j++) {
             this->pixels[i][j] = copy.getPixels()[i][j];
        }
    }
    this->edgeEffect = copy.edgeEffect;
}

Image::Image(const QImage &image, IEdgeEffect* edgeEffect) {
    this->width = image.width();
    this->height = image.height();

    this->pixels = new double *[width];

    // Read pixels and form black and white image
    #pragma omp parallel for
    for (int i = 0; i < width; i++) {
        this->pixels[i] = new double[height];
        #pragma omp parallel for
        for (int j = 0; j < height; j++) {
            QRgb pixel = image.pixel(i, j);
            this->pixels[i][j] = 0.213 * qRed(pixel) + 0.715 * qGreen(pixel) + 0.072 * qBlue(pixel);
        }
    }
    this->edgeEffect = QSharedPointer<IEdgeEffect>(edgeEffect);
}

Image::~Image() {
    for (int i = 0; i < width; i++) {
        delete[] this->pixels[i];
    }
    delete[] this->pixels;
    this->edgeEffect.reset();
}

QImage &Image::getOutputImage() {
    QImage *image = new QImage(this->width, this->height, QImage::Format_ARGB32);
    #pragma omp parallel for
    for (int i = 0; i < this->width; i++) {
        #pragma omp parallel for
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
    // Validation
    if(pixel<0) pixel = 0;
    if(pixel>255) pixel = 255;

    pixels[x][y] = pixel;
}

void Image::setEdgeEffect(IEdgeEffect* value)
{
    this->edgeEffect.reset(value);
}

#include "Image.h"

Image::Image(int width, int height)
        : pixels(new double[width * height]) {
    this->width = width;
    this->height = height;
    this->edgeEffect = EdgeEffect::Black;
}

Image::Image(const Image &copy)
        : pixels(new double[copy.width * copy.height]) {
    this->width = copy.width;
    this->height = copy.height;
    this->edgeEffect = copy.edgeEffect;

    // Copy Pixels
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            this->pixels[i + j * width] = copy.pixels[i + j * width];
        }
    }
}

Image::Image(const QImage &image) : Image(image, EdgeEffect::Wrapping) {
}

Image::Image(const QImage &image, EdgeEffect edgeEffect)
        : pixels(new double[image.width() * image.height()]) {
    this->width = image.width();
    this->height = image.height();
    this->edgeEffect = edgeEffect;

    // Read pixels and form black and white image
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            QRgb pixel = image.pixel(i, j);
            this->pixels[i + j * width] = 0.213 * qRed(pixel) + 0.715 * qGreen(pixel) + 0.072 * qBlue(pixel);
        }
    }
}

QImage &Image::getOutputImage() {
    QImage *image = new QImage(this->width, this->height, QImage::Format_ARGB32);
    for (int i = 0; i < this->width; i++) {
        for (int j = 0; j < this->height; j++) {
            double pixel = pixels[i + j * width];
            image->setPixel(i, j, qRgb(pixel, pixel, pixel));
        }
    }
    return *image;
}

void Image::setPixel(int x, int y, double pixel) {
    // Validation
    if (pixel < 0) pixel = 0;
    if (pixel > 255) pixel = 255;

    pixels[x + y * width] = pixel;
}

double Image::getPixel(int x, int y) {
    if (x < width && x > -1 && y < height && y > -1)
        return pixels[x + y * width];
    switch (edgeEffect) {
        case EdgeEffect::Black:
            return 0;
        case EdgeEffect::Repeat:
            return getPixelRepeat(x, y);
        case EdgeEffect::Mirror:
            return getPixelMirror(x, y);
        case EdgeEffect::Wrapping:
            return getPixelWrapping(x, y);
    }
    return 0;
}

double Image::getPixelRepeat(int x, int y) {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= width) x = width - 1;
    if (y >= height) y = height - 1;

    return pixels[x + y * width];
}

double Image::getPixelMirror(int x, int y) {
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    if (x >= width) x = 2 * width - x - 1;
    if (y >= height) y = 2 * height - y - 1;

    return pixels[x + y * width];
}

double Image::getPixelWrapping(int x, int y) {
    if (x < 0) x = width + x;
    if (y < 0) y = height + y;
    if (x >= width) x = 1 + (x - width);
    if (y >= height) y = 1 + (y - height);

    return pixels[x + y * width];
}

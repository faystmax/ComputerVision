#include "Image.h"

Image::Image() {
    this->width = 0;
    this->height = 0;
    this->edgeEffect = EdgeEffect::Black;
}

Image::Image(const int width, const int height, const EdgeEffect edgeEffect) {
    this->width = width;
    this->height = height;
    this->edgeEffect = edgeEffect;
    this->pixels.resize(width * height);
}

Image::Image(const Image &copy) {
    this->width = copy.width;
    this->height = copy.height;
    this->edgeEffect = copy.edgeEffect;
    this->pixels = copy.pixels;
}

void Image::setPixel(int x, int y, double pixel) {
    // Validation
    if (pixel < 0) pixel = 0;
    if (pixel > 255) pixel = 255;

    pixels[x + y * width] = pixel;
}

double Image::getPixel(int x, int y) const {
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

double Image::getPixelRepeat(int x, int y) const {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= width) x = width - 1;
    if (y >= height) y = height - 1;

    return pixels[x + y * width];
}

double Image::getPixelMirror(int x, int y) const {
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    if (x >= width) x = 2 * width - x - 1;
    if (y >= height) y = 2 * height - y - 1;

    return pixels[x + y * width];
}

double Image::getPixelWrapping(int x, int y) const {
    if (x < 0) x = width + x;
    if (y < 0) y = height + y;
    if (x >= width) x = 1 + (x - width);
    if (y >= height) y = 1 + (y - height);

    return pixels[x + y * width];
}

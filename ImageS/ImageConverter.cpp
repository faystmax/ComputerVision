#include "ImageConverter.h"
#include "KernelCreator.h"
#include "math.h"
#include <omp.h>

ImageConverter::ImageConverter() {
}

void ImageConverter::convolution(Image & image, Kernel & core) {
    Image copyImage(image);
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double resultPixel = 0;
            for (int x = 0; x < core.getWidth(); x++) {
                for (int y = 0; y < core.getHeight(); y++) {
                    int realI = i + (x - (core.getWidth() / 2));
                    int realJ = j + (y - (core.getHeight() / 2));
                    resultPixel += copyImage.getPixel(realI, realJ) * core.getCoreAt(x, y);
                }
            }
            image.setPixel(i, j, resultPixel);
        }
    }
}

void ImageConverter::sobel(Image & image) {
    Image copyImageX(image);
    Image copyImageY(image);
    convolution(copyImageX, *CoreCreator::getSobelX().data());
    convolution(copyImageY, *CoreCreator::getSobelY().data());
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double pixelX = copyImageX.getPixel(i, j);
            double pixelY = copyImageY.getPixel(i, j);
            image.setPixel(i, j, sqrt(pixelX * pixelX + pixelY * pixelY));
        }
    }
}

void ImageConverter::priut(Image & image) {
    Image copyImageX(image);
    Image copyImageY(image);
    convolution(copyImageX, *CoreCreator::getPriutX().data());
    convolution(copyImageY, *CoreCreator::getPriutY().data());
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double pixelX = copyImageX.getPixel(i, j);
            double pixelY = copyImageY.getPixel(i, j);
            image.setPixel(i, j, sqrt(pixelX * pixelX + pixelY * pixelY));
        }
    }
}

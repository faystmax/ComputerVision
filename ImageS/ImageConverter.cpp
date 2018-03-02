#include "ImageConverter.h"
#include "KernelCreator.h"
#include <math.h>

Image ImageConverter::convolution(const Image &image, const Kernel &core) {
    Image resultImage(image);
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double resultPixel = 0;
            for (int x = 0; x < core.getWidth(); x++) {
                for (int y = 0; y < core.getHeight(); y++) {
                    int realI = i + (x - (core.getWidth() / 2));
                    int realJ = j + (y - (core.getHeight() / 2));
                    resultPixel += image.getPixel(realI, realJ) * core.getCoreAt(x, y);
                }
            }
            resultImage.setPixel(i, j, resultPixel);
        }
    }
    return resultImage;
}

Image ImageConverter::sobel(const Image &image) {
    Image resultImage(image);
    Image copyImageX = convolution(image, KernelCreator::getSobelX());
    Image copyImageY = convolution(image, KernelCreator::getSobelY());
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double pixelX = copyImageX.getPixel(i, j);
            double pixelY = copyImageY.getPixel(i, j);
            resultImage.setPixel(i, j, sqrt(pixelX * pixelX + pixelY * pixelY));
        }
    }
    return resultImage;
}

Image ImageConverter::priut(const Image &image) {
    Image resultImage(image);
    Image copyImageX = convolution(image, KernelCreator::getPriutX());
    Image copyImageY = convolution(image, KernelCreator::getPriutY());
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double pixelX = copyImageX.getPixel(i, j);
            double pixelY = copyImageY.getPixel(i, j);
            resultImage.setPixel(i, j, sqrt(pixelX * pixelX + pixelY * pixelY));
        }
    }
    return resultImage;
}

Image ImageConverter::halfReduce(const Image &image) {
    Image resultImage(image.getWidth() / 2, image.getHeight() / 2);
    resultImage.setEdgeEffect(image.getEdgeEffect());
    for (int i = 0; i < image.getWidth() / 2; i++) {
        for (int j = 0; j < image.getHeight() / 2; j++) {
            double resullPixel = (image.getPixel(2 * i, 2 * j) + image.getPixel(2 * i + 1, 2 * j) +
                                  image.getPixel(2 * i, 2 * j + 1) + image.getPixel(2 * i + 1, 2 * j + 1)) / 4;
            resultImage.setPixel(i, j, resullPixel);
        }
    }
    return resultImage;
}

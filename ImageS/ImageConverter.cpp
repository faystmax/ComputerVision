#include "ImageConverter.h"
#include "KernelCreator.h"
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

Image ImageConverter::convolution(const Image &image, const Kernel &core) {
    Image resultImage(image);
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double resultPixel = 0;
            for (int x = 0; x < core.getWidth(); x++) {
                for (int y = 0; y < core.getHeight(); y++) {
                    int real_i = i + (x - (core.getWidth() / 2));
                    int real_j = j + (y - (core.getHeight() / 2));
                    resultPixel += image.getPixel(real_i, real_j) * core.get(x, y);
                }
            }
            resultImage.setPixel(i, j, resultPixel);
        }
    }
    return resultImage;
}

Image ImageConverter::sobel(const Image &image) {
    Image resultImage(image);
    Image image_dx = convolution(image, KernelCreator::getSobelX());
    Image image_dy = convolution(image, KernelCreator::getSobelY());
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double pixelX = image_dx.getPixel(i, j);
            double pixelY = image_dy.getPixel(i, j);
            resultImage.setPixel(i, j, sqrt(pixelX * pixelX + pixelY * pixelY));
        }
    }
    return resultImage;
}

Image ImageConverter::priut(const Image &image) {
    Image resultImage(image);
    Image image_dx = convolution(image, KernelCreator::getPriutX());
    Image image_dy = convolution(image, KernelCreator::getPriutY());
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double pixelX = image_dx.getPixel(i, j);
            double pixelY = image_dy.getPixel(i, j);
            resultImage.setPixel(i, j, sqrt(pixelX * pixelX + pixelY * pixelY));
        }
    }
    return resultImage;
}

Image ImageConverter::rotate(const Image &image) {
    Image resultImage(image.getHeight(),image.getWidth());
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            resultImage.setPixel(image.getHeight() -1 -j, i, image.getPixel(i,j));
        }
    }
    return resultImage;
}

Image ImageConverter::noise(const Image &image, const int count) {
    srand(time(0));

    double noise = image.getMax() - image.getMin();
    Image resultImage(image);
    for (int i = 0; i < count; i++) {
        resultImage.setPixel(rand() % image.getWidth(), rand() % image.getHeight(), noise);
    }
    return resultImage;
}

Image ImageConverter::halfReduce(const Image &image) {
    Image resultImage(image.getWidth() / 2, image.getHeight() / 2);
    resultImage.setEdgeEffect(image.getEdgeEffect());
    for (int i = 0; i < resultImage.getWidth(); i++) {
        for (int j = 0; j < resultImage.getHeight(); j++) {
            double resullPixel = (image.getPixel(2 * i, 2 * j) + image.getPixel(2 * i + 1, 2 * j) +
                                  image.getPixel(2 * i, 2 * j + 1) + image.getPixel(2 * i + 1, 2 * j + 1)) / 4;
            resultImage.setPixel(i, j, resullPixel);
        }
    }
    return resultImage;
}


Image ImageConverter::bilinearHalfReduce(const Image &image) {
    Image resultImage(image.getWidth() / 2, image.getHeight() / 2);
    resultImage.setEdgeEffect(image.getEdgeEffect());
    double x_koef = image.getWidth()  / (image.getWidth()/2.);
    double y_koef = image.getHeight()  / (image.getHeight()/2.);

    for (auto i = 0; i < resultImage.getWidth(); i++) {
        for (auto j = 0; j < resultImage.getHeight(); j++) {

            int x = x_koef * i;
            int y = y_koef * j;
            double x_ost = (x_koef * i) - x;
            double y_ost = (y_koef * j) - y;
            double p1 = image.getPixel(x, y) * (1 - x_ost) * (1 - y_ost);
            double p2 = image.getPixel(x + 1, y) * (x_ost) * (1 - y_ost);
            double p3 = image.getPixel(x, y + 1) * (1 - x_ost) * (y_ost);
            double p4 = image.getPixel(x + 1, y + 1) * (x_ost * y_ost);
            resultImage.setPixel(i, j, p1 + p2 + p3 + p4);
        }
    }
    return resultImage;
}

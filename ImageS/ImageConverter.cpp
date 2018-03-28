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
            resultImage.setPixelNoValidation(i, j, resultPixel);
        }
    }

//    for (int i = 0; i < core.getWidth(); i++) {
//        for (int j = 0; j < core.getHeight(); j++) {
//            std::cout << core.getCoreAt(i, j) << " ";
//        }
//        std::cout << std::endl;
//    }
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

Image ImageConverter::rotate(const Image &image){
    Image resultImage(image.getHeight(),image.getWidth());
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
          resultImage.setPixelNoValidation(image.getHeight()-1-j, i, image.getPixel(i,j));
      }
    }
    return resultImage;
}

Image ImageConverter::noise(const Image &image, const int count){
    srand(time(0));
    Image resultImage(image);
    for (int i = 0; i < count; i++) {
        resultImage.setPixel(rand() % image.getWidth(), rand() % image.getHeight(), 150);
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

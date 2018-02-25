#include "ImageConverter.h"
#include "KernelCreator.h"
#include "math.h"
#include <omp.h>

ImageConverter::ImageConverter() {
}

unique_ptr <Image> ImageConverter::convolution(Image & image, Kernel & core) {
    unique_ptr <Image> resultImage(new Image(image));
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
            resultImage.get()->setPixel(i, j, resultPixel);
        }
    }
    return resultImage;
}

unique_ptr <Image> ImageConverter::sobel(Image & image) {
    unique_ptr <Image> resultImage(new Image(image));
    unique_ptr <Image> copyImageX = convolution(image, *KernelCreator::getSobelX().get());
    unique_ptr <Image> copyImageY = convolution(image, *KernelCreator::getSobelY().get());
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double pixelX = copyImageX.get()->getPixel(i, j);
            double pixelY = copyImageY.get()->getPixel(i, j);
            resultImage.get()->setPixel(i, j, sqrt(pixelX * pixelX + pixelY * pixelY));
        }
    }
    return resultImage;
}

unique_ptr <Image> ImageConverter::priut(Image & image) {
    unique_ptr <Image> resultImage(new Image(image));
    unique_ptr <Image> copyImageX = convolution(image, *KernelCreator::getPriutX().get());
    unique_ptr <Image> copyImageY = convolution(image, *KernelCreator::getPriutY().get());
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double pixelX = copyImageX.get()->getPixel(i, j);
            double pixelY = copyImageY.get()->getPixel(i, j);
            resultImage.get()->setPixel(i, j, sqrt(pixelX * pixelX + pixelY * pixelY));
        }
    }
    return resultImage;
}

unique_ptr <Image> ImageConverter::halfReduce(Image & image) {
    unique_ptr <Image> resultImage(new Image(image.getWidth() / 2, image.getHeight() / 2));
    resultImage.get()->setEdgeEffect(image.getEdgeEffect());
    for (int i = 0; i < image.getWidth() / 2; i++) {
        for (int j = 0; j < image.getHeight() / 2; j++) {
            double resullPixel = (image.getPixel(2 * i, 2 * j) + image.getPixel(2 * i + 1, 2 * j) +
                                  image.getPixel(2 * i, 2 * j + 1) + image.getPixel(2 * i + 1, 2 * j + 1)) / 4;
            resultImage.get()->setPixel(i, j, resullPixel);
        }
    }
    return resultImage;
}

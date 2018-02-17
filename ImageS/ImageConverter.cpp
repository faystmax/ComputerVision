#include "ImageConverter.h"

ImageConverter::ImageConverter() {

}

void ImageConverter::convolution(Image * image, Core * core) {
    for (int i = 0; i < image->getWidth(); i++) {
        for (int j = 0; j < image->getHeight(); j++) {
            double resultPixel = 0;
            for (int x = 0; x < core->getWidth(); x++) {
                for (int y = 0; y < core->getHeight(); y++) {
                    int realI = i + (x - (core->getWidth() / 2));
                    int realJ = j + (y - (core->getHeight() / 2));
                    resultPixel += image->getPixel(realI, realJ) * core->getCoreAt(x, y);
                }
            }
            image->setPixel(i, j, resultPixel);
        }
    }

}

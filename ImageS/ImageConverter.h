#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

#include "Image.h"
#include "KernelCreator.h"


class IMAGESSHARED_EXPORT ImageConverter
{
public:

    static Image sobel(const Image &image);
    static Image priut(const Image &image);
    static Image rotate(const Image &image);
    static Image halfReduce(const Image &image);
static Image noise(const Image &image, const int count);
static Image convolution(const Image &image, const Kernel &core);
};

#endif // IMAGECONVERTER_H

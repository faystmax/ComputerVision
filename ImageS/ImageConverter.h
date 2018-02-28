#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

#include "Image.h"
#include "KernelCreator.h"


class IMAGESSHARED_EXPORT ImageConverter
{
public:
    ImageConverter() = default;

    static Image convolution(const Image &image,const Kernel &core);
    static Image sobel(const Image &image);
    static Image priut(const Image &image);
    static Image halfReduce(const Image &image);
};

#endif // IMAGECONVERTER_H

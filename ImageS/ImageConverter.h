#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

#include "Image.h"
#include "KernelCreator.h"

class IMAGESSHARED_EXPORT ImageConverter
{
public:
    ImageConverter();

    static void convolution(Image &image,Kernel &core);
    static void sobel(Image &image);
    static void priut(Image &image);
};

#endif // IMAGECONVERTER_H

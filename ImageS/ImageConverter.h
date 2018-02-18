#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

#include "Image.h"
#include "CoreCreator.h"

class IMAGESSHARED_EXPORT ImageConverter
{
public:
    ImageConverter();

    static void convolution(Image* image,Core* core);
    static void sobel(Image* image);
    static void priut(Image* image);
};

#endif // IMAGECONVERTER_H

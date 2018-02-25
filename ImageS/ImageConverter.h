#ifndef IMAGECONVERTER_H
#define IMAGECONVERTER_H

#include "Image.h"
#include "KernelCreator.h"
#include <memory>

using namespace std;

class IMAGESSHARED_EXPORT ImageConverter
{
public:
    ImageConverter();

    static unique_ptr<Image> convolution(Image &image,Kernel &core);
    static unique_ptr<Image> sobel(Image &image);
    static unique_ptr<Image> priut(Image &image);
    static unique_ptr<Image> halfReduce(Image &image);
};

#endif // IMAGECONVERTER_H

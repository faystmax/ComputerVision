#include "Pyramid.h"

#include "KernelCreator.h"
#include "ImageConverter.h"
#include "math.h"

Pyramid::Pyramid() {

}

void Pyramid::generate(Image &image, int scales, double sigma) {

    items.clear();
    unique_ptr<Image> tmpImage(new Image(image));
    double sigmaScale = sigma;
    double sigmaEffect = sigma;
    double octave = 1;

    while (tmpImage.get()->getWidth() >= 2 && tmpImage.get()->getHeight() >= 2) {
        sigmaScale = sigma;
        double deltsSigma = pow(2, 1.0 / scales);
        int scale = 1;
        for (int i = 0; i < scales; i++) {
            sigmaScale *= deltsSigma;
            sigmaEffect *= deltsSigma;
            unique_ptr<Kernel> gauss =  KernelCreator::getGauss(5, 5, sigmaScale);
            unique_ptr<Image> result = ImageConverter::convolution(*tmpImage.get(), *gauss.get());
            items.push_back(Item(result.release(), octave, scale, sigmaScale, sigmaEffect));
            scale++;
        }
        octave++;
        unique_ptr<Image> half = ImageConverter::halfReduce(*items.at(items.size()-1).image.get());
        tmpImage.reset(half.release());
    }
}

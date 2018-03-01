#include "Pyramid.h"

#include "KernelCreator.h"
#include "ImageConverter.h"
#include "math.h"

void Pyramid::generate(const Image &image, const int scales, const double sigma, const double sigmaStart) {

    items.clear();

    // Push original
    items.emplace_back(image, 0, 0, sigmaStart, sigmaStart);

    double deltaSigma = getDeltaSigma(sigmaStart, sigma);
    Kernel gauss = KernelCreator::getGauss(deltaSigma);
    Image result = ImageConverter::convolution(getLastImage(), gauss);
    items.emplace_back(result, 1, 0, sigma, sigma);

    double sigmaScale = sigma;
    double sigmaEffect = sigma;
    double octave = 1;

    // While image can be reduced
    while (getLastImage().getWidth() >= 2 && getLastImage().getHeight() >= 2) {
        sigmaScale = sigma;
        double intervalSigma = pow(2, 1.0 / scales);
        for (int i = 0; i < scales; i++) {
            double sigmaScalePrev = sigmaScale;
            sigmaScale = sigma * pow(intervalSigma, i + 1);
            sigmaEffect *= sigmaScale;
            double deltaSigma = getDeltaSigma(sigmaScalePrev, sigmaScale);
            Kernel gauss = KernelCreator::getGauss(deltaSigma);
            Image result = ImageConverter::convolution(getLastImage(), gauss);
            items.emplace_back(result, octave, i + 1, sigmaScale, sigmaEffect);
        }
        octave++;
        Image half = ImageConverter::halfReduce(getLastImage());
        items.push_back(Item(half, octave, 0, sigmaScale, sigmaEffect));
    }
}

double Pyramid::getDeltaSigma(double sigmaPrev, double sigmaCur) const {
    return sqrt(sigmaCur * sigmaCur - sigmaPrev * sigmaPrev);
}

Image Pyramid::getLastImage() const {
    return items.at(items.size() - 1).image;
}

#include "Pyramid.h"

#include "KernelCreator.h"
#include "ImageConverter.h"
#include "math.h"

Pyramid::Pyramid(const Image &image, const int scales, double sigma, double sigmaStart) {
    items.clear();

    double deltaSigma = getDeltaSigma(sigmaStart, sigma);
    Kernel gauss = KernelCreator::getGaussX(deltaSigma);
    Image imageX = ImageConverter::convolution(image, gauss);
    gauss.rotate();
    items.emplace_back(ImageConverter::convolution(std::move(imageX), gauss), 1, 0, sigma, sigma);

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
            gauss = KernelCreator::getGaussX(deltaSigma);
            imageX = ImageConverter::convolution(getLastImage(), gauss);
            gauss.rotate();
            items.emplace_back(ImageConverter::convolution(std::move(imageX), gauss),
                               octave, i + 1, sigmaScale, sigmaEffect);
        }
        octave++;
        items.emplace_back(ImageConverter::halfReduce(getLastImage()), octave, 0, sigmaScale, sigmaEffect);
    }
}

int Pyramid::L(int x, int y, double sigma) const {
    if (items.size() > 0) {
        int width = items[0].image.getWidth();
        int height = items[0].image.getHeight();
        for (unsigned int i = 0; i < items.size() - 1; i++) {
            if (sigma >= items[i].sigmaEffect && sigma <= items[i + 1].sigmaEffect) {
                int xCur = x * items[i].image.getWidth() * (1.0 / width);
                int yCur = y * items[i].image.getHeight() * (1.0 / height);
                return items[i].image.getPixel(xCur, yCur);
            }
        }
    }
    return 0;
}

double Pyramid::getDeltaSigma(double sigmaPrev, double sigmaCur) const {
    return sqrt(sigmaCur * sigmaCur - sigmaPrev * sigmaPrev);
}

Image Pyramid::getLastImage() const {
    return items.at(items.size() - 1).image;
}

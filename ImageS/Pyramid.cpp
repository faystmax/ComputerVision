#include "Pyramid.h"

#include "KernelCreator.h"
#include "ImageConverter.h"
#include "math.h"

Pyramid::Pyramid(const Image &image, const int scales, double sigma, double sigmaStart) {
    items.clear();
    int octaveCount = min(log2(image.getWidth()),log2(image.getHeight()))-1;
    items.reserve(octaveCount * scales);

    double deltaSigma = getDeltaSigma(sigmaStart, sigma);
    Kernel gauss = KernelCreator::getGauss(deltaSigma);
    Image imageX = ImageConverter::convolution(image, gauss);
    gauss.rotate();
    items.emplace_back(ImageConverter::convolution(imageX, gauss), 0, 0, sigma, sigma);

    double sigmaScale = sigma;
    double sigmaEffect = sigma;
    double octave = 0;

    // While image can be reduced
    while (octaveCount > 0) {
        double intervalSigma = pow(2, 1.0 / scales);

        for (int i = 0; i < scales; i++) {
            double sigmaScalePrev = sigmaScale;
            sigmaScale = sigma * pow(intervalSigma, i + 1);
            double deltaSigma = getDeltaSigma(sigmaScalePrev, sigmaScale);
            sigmaEffect *= intervalSigma;

            gauss = KernelCreator::getGauss(deltaSigma);
            imageX = ImageConverter::convolution(getLastImage(), gauss);
            gauss.rotate();
            items.emplace_back(ImageConverter::convolution(imageX, gauss), octave, i + 1, sigmaScale, sigmaEffect);
        }
        octave++;
        sigmaScale = 1;
        octaveCount--;
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

double Pyramid::getDeltaSigma(double sigmaPrev, double sigmaNext) const {
    return sqrt(sigmaNext * sigmaNext - sigmaPrev * sigmaPrev);
}

Image &Pyramid::getLastImage() {
    return items.at(items.size() - 1).image;
}

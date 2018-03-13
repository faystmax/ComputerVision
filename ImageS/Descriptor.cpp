#include "Descriptor.h"
#include "KernelCreator.h"
#include "ImageConverter.h"
#include <Util.cpp>
#include <math.h>

void Descriptor::normalize() {
    double length = getLength();
    if (length > 0) {
        for (auto &data : this->data)
            data /= length;
    }
}

double Descriptor::getLength() {
    auto length = 0;
    for (auto &data : this->data)
        length += data * data;
    return sqrt(length);
}

void Descriptor::clampData(const double min, const double max) {
    for (auto &data : this->data)
        data = clamp(min, max, data);
}

double DescriptorCreator::getDistance(const Descriptor &d1, const Descriptor &d2) {
    auto result = 0;
    for (unsigned int i = 0; i < d1.data.size() && i < d2.data.size(); i++) {
        auto razn = d1.data[i] - d2.data[i];
        result += razn * razn;
    }
    return sqrt(result);
}

vector <Descriptor> DescriptorCreator::getDescriptors(const Image &image, const vector <Point> interestPoints,
                                                      const int radius, const int basketCount, const int barCharCount) {
    auto sector = 2 * M_PI / basketCount;
    auto halfSector = M_PI / basketCount;
    auto dimension = 2 * radius;

    Image image_dx = ImageConverter::convolution(image, KernelCreator::getSobelX());
    Image image_dy = ImageConverter::convolution(image, KernelCreator::getSobelY());

    vector <Descriptor> descriptors(interestPoints.size());
    for (unsigned int i = 0; i < interestPoints.size(); i++) {
        descriptors[i] = Descriptor(barCharCount * basketCount);

        for (auto i = 0; i < dimension; i++) {
            for (auto j = 0; j < dimension; j++) {
                // get Gradient
                auto gradient_X = image_dx.getPixel(i - radius + interestPoints[i].x, j - radius + interestPoints[i].y);
                auto gradient_Y = image_dy.getPixel(i - radius + interestPoints[i].x, j - radius + interestPoints[i].y);

                // get value and phi
                auto value = getGradientValue(gradient_X, gradient_Y);
                auto phi = getGradientDirection(gradient_X, gradient_Y);

                // получаем индекс корзины в которую входит phi и смежную с ней
                int mainBasketIndex = floor(phi / sector);
                int sideBasketIndex = floor((phi - halfSector) / sector);

                // вычисляем центры смежных корзин
                auto mainBasketPhi = mainBasketIndex * sector + halfSector;
                auto sideBasketPhi = sideBasketIndex * sector + halfSector;

                // распределяем L(value) между ними
                auto mainBasketValue = (abs(phi - mainBasketPhi) / abs(mainBasketPhi - sideBasketPhi)) * value;
                auto sideBasketValue = (abs(phi - sideBasketPhi) / abs(mainBasketPhi - sideBasketPhi)) * value;

                // TODO вычисляем индекс куда записывать значения
                auto barCharStep = dimension / (barCharCount / 2);
                auto tmp_i = i / barCharStep;
                auto tmp_j = j / barCharStep;

                auto indexMain = tmp_i + tmp_j * (barCharCount / 2) + mainBasketIndex;
                auto indexSide = tmp_i + tmp_j * (barCharCount / 2) + sideBasketIndex;

                descriptors[i].data[indexMain] += mainBasketValue;
                descriptors[i].data[indexSide] += sideBasketValue;
            }
        }
        descriptors[i].normalize();
        descriptors[i].clampData(0, 0.2);
        descriptors[i].normalize();
    }
    return descriptors;
}




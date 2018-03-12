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
    auto minimum = min(d1.data.size(), d2.data.size());
    for (unsigned int i = 0; i < minimum; i++) {
        auto razn = d1.data[i] - d2.data[i];
        result += razn * razn;
    }
    return sqrt(result);
}

vector <Descriptor> DescriptorCreator::getDescriptors(const Image &image, const vector <Point> interestPoints,
                                                      const int radius, const int basketCount, const int barCharCount) {
    Image image_dx = ImageConverter::convolution(image, KernelCreator::getSobelX());
    Image image_dy = ImageConverter::convolution(image, KernelCreator::getSobelY());

    vector <Descriptor> descriptors(interestPoints.size());
    for (unsigned int i = 0; i < interestPoints.size(); i++) {
        descriptors[i] = getDescriptor(image_dx, image_dy, interestPoints.at(i), radius, basketCount, barCharCount);
    }
    return descriptors;
}

Descriptor DescriptorCreator::getDescriptor(const Image &image_dx, const Image &image_dy, const Point &point,
                                            const int radius, const int basketCount, const int barCharCount) {

    Descriptor result(barCharCount * basketCount);
    auto sector = 2 * M_PI / basketCount;
    auto halfSector = M_PI / basketCount;
    auto dimension = 2 * radius;

    for (auto i = -radius + 1; i < radius; i++) {
        for (auto j = -radius + 1; j < radius; j++) {
            // get Gradient
            auto gradient_X = image_dx.getPixel(i + point.x, j + point.y);
            auto gradient_Y = image_dy.getPixel(i + point.x, j + point.y);

            // get value and phi
            auto value = getGradientValue(gradient_X, gradient_Y);
            auto phi = getGradientDirection(gradient_X, gradient_Y);

            // получаем индекс корзины в которую входит phi и смежную с ней
            auto mainBasketIndex = phi - fmod(phi, sector);
            auto sideBasketIndex = floor(phi / halfSector) % 2 == 0 ? mainBasketIndex - 1 : mainBasketIndex + 1;
            if (sideBasketIndex >= basketCount) sideBasketIndex = 0;
            if (sideBasketIndex < 0) sideBasketIndex = basketCount - 1;

            // вычисляем центры смежных корзин
            auto mainBasketPhi = mainBasketIndex * sector + halfSector;
            auto sideBasketPhi = sideBasketIndex * sector + halfSector;

            // распределяем L(value) между ними
            auto mainBasketValue = (abs(phi - mainBasketPhi) / abs(mainBasketPhi - sideBasketPhi)) * value;
            auto sideBasketValue = (abs(phi - sideBasketPhi) / abs(mainBasketPhi - sideBasketPhi)) * value;

            // TODO вычисляем индекс куда записывать значения
            auto barCharStep = dimension / (barCharCount / 2);
            auto tmp_i = (i + radius) / barCharStep;
            auto tmp_j = (j + radius) / barCharStep;

            auto indexMain = tmp_i + tmp_j * (barCharCount / 2) + mainBasketIndex;
            auto indexSide = tmp_i + tmp_j * (barCharCount / 2) + sideBasketIndex;

            result.data[indexMain] += mainBasketValue;
            result.data[indexSide] += sideBasketValue;
        }
    }
    result.normalize();
    result.clampData(0, 0.2);
    result.normalize();
    return result;
}





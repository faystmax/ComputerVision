#include "Descriptor.h"
#include "KernelCreator.h"
#include "ImageConverter.h"
#include <Util.cpp>
#include <math.h>
#include <iostream>


Descriptor::Descriptor(const int size, Point interPoint) {
    data.resize(size, 0);
    this->interPoint = interPoint;
}

void Descriptor::normalize() {
    double length = 0;
    for (auto &data : this->data)
        length += data;
    for (auto &data : this->data)
        data /= length;
}

void Descriptor::clampData(const double min, const double max) {
    for (auto &data : this->data)
        data = clamp(min, max, data);
}

double DescriptorCreator::getDistance(const Descriptor &d1, const Descriptor &d2) {
    double result = 0;
    for (unsigned int i = 0; i < d1.data.size(); i++) {
        double tmp = d1.data[i] - d2.data[i];
        result += tmp * tmp;
    }
    return sqrt(result);
}

vector <Descriptor> DescriptorCreator::getDescriptors(const Image &image, const vector <Point> interestPoints,
                                                      const int radius, const int basketCount, const int barCharCount) {
    auto dimension = 2 * radius;
    auto sector = 2 * M_PI / basketCount;
    auto halfSector = M_PI / basketCount;
    auto barCharStep = dimension / (barCharCount / 4);
    auto barCharCountInLine = (barCharCount / 4);


    Image image_dx = ImageConverter::convolution(image, KernelCreator::getSobelX());
    Image image_dy = ImageConverter::convolution(image, KernelCreator::getSobelY());

    vector <Descriptor> descriptors(interestPoints.size());
    for (unsigned int k = 0; k < interestPoints.size(); k++) {
        descriptors[k] = Descriptor(barCharCount * basketCount, interestPoints[k]);

        for (auto i = 0; i < dimension; i++) {
            for (auto j = 0; j < dimension; j++) {
                // get Gradient
                auto gradient_X = image_dx.getPixel(i - radius + interestPoints[k].x, j - radius + interestPoints[k].y);
                auto gradient_Y = image_dy.getPixel(i - radius + interestPoints[k].x, j - radius + interestPoints[k].y);

                // get value and phi
                auto value = getGradientValue(gradient_X, gradient_Y);
                auto phi = getGradientDirection(gradient_X, gradient_Y);

                // получаем индекс корзины в которую входит phi и смежную с ней
                int mainBasketIndex = floor(phi / sector);
                int sideBasketIndex = floor((phi + halfSector) / sector) == mainBasketIndex ? mainBasketIndex - 1 : mainBasketIndex +1;
                if (sideBasketIndex > basketCount) sideBasketIndex = 0;
                if (sideBasketIndex < 0) sideBasketIndex = basketCount - 1;

                // вычисляем центр
                auto mainBasketPhi = mainBasketIndex * sector + halfSector;

                // распределяем L(value)
                auto mainBasketValue = (1 - (abs(phi - mainBasketPhi) / sector)) * value;
                auto sideBasketValue = value - mainBasketValue;

                // вычисляем индекс куда записывать значения
                auto tmp_i = (i / barCharStep) * basketCount;
                auto tmp_j = (j / barCharStep) * basketCount;

                auto indexMain = tmp_i + tmp_j * barCharCountInLine + mainBasketIndex;
                auto indexSide = tmp_i + tmp_j * barCharCountInLine + sideBasketIndex;

                // записываем значения
                descriptors[k].data[indexMain] += mainBasketValue;
                descriptors[k].data[indexSide] += sideBasketValue;
            }
        }
        descriptors[k].normalize();
        descriptors[k].clampData(0, 0.2);
        descriptors[k].normalize();
    }
    return descriptors;
}

vector<Vector> DescriptorCreator::findSimilar(const vector<Descriptor> &d1, const vector<Descriptor> &d2, const double treshhold){
    vector<Vector> similar;

    for(unsigned int i = 0; i < d1.size(); i++){
        int indexSimilar = -1;
        double minDistance = numeric_limits<int>::max();
        for(unsigned int j = 0; j < d2.size(); j++){

            double dist = getDistance(d1[i], d2[j]);
            // отбрасываем
            if(indexSimilar != -1 && dist>0 && dist > minDistance && minDistance/dist > treshhold){
                indexSimilar = -1;
                break;
            }
            if(dist < minDistance){
                indexSimilar = j;
                minDistance = dist;
            }
        }

        if(indexSimilar != -1){
            similar.emplace_back(d1[i],d2[indexSimilar]);
        }
    }
    return similar;
}




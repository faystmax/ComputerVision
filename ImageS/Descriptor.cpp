#include "Descriptor.h"
#include "KernelCreator.h"
#include "ImageConverter.h"
#include <Util.cpp>
#include <math.h>
#include <iostream>
#include <numeric>

Descriptor::Descriptor(const int size, Point interPoint) {
    data.resize(size, 0);
    this->interPoint = interPoint;
}

void Descriptor::normalize() {
    double length = 0;
    for (auto &data : this->data)
        length += data * data;
    length = sqrt(length);
    for (auto &data : this->data)
        data /= length;
}

void Descriptor::clampData(const double min, const double max) {
    for (auto &data : this->data)
        data = clamp(min, max, data);
}

double DescriptorCreator::getDistance(const Descriptor &d1, const Descriptor &d2) {
  auto op = [](double a, double b){ return pow(a - b, 2); };
  auto sum = inner_product(begin(d1.data), end(d1.data), begin(d2.data), 0., plus<>(), op);
  return sqrt(sum);
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
                int firstBasketIndex = floor(phi / sector);
                int secondBasketIndex = floor((phi - halfSector) / sector);
                if (secondBasketIndex > basketCount) secondBasketIndex = 0;
                if (secondBasketIndex < 0) secondBasketIndex = basketCount - 1;

                // вычисляем центр
                auto mainBasketPhi = firstBasketIndex * sector + halfSector;

                // распределяем L(value)
                auto mainBasketValue = (1 - (abs(phi - mainBasketPhi) / sector)) * value;
                auto sideBasketValue = value - mainBasketValue;

                // вычисляем индекс куда записывать значения
                auto tmp_i = (i / barCharStep) * basketCount;
                auto tmp_j = (j / barCharStep) * basketCount;

                auto indexMain = tmp_i + tmp_j * barCharCountInLine + firstBasketIndex;
                auto indexSide = tmp_i + tmp_j * barCharCountInLine + secondBasketIndex;

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


double DescriptorCreator::getPointOrientation(const Image &image_dx, const Image &image_dy,const Point &point, const int radius){

    const int basketCount = 36;

    auto dimension = 2 * radius;
    auto sector = 2 * M_PI / basketCount;
    auto halfSector = M_PI / basketCount;

    vector<double> values(basketCount, 0);
    for (auto i = 0; i < dimension; i++) {
        for (auto j = 0; j < dimension; j++) {
            // get Gradient
            auto coord_X = i - radius + point.x;
            auto coord_Y = j - radius + point.y;
            auto gradient_X = image_dx.getPixel(coord_X + 1, coord_Y) - image_dx.getPixel(coord_X - 1, coord_Y);
            auto gradient_Y = image_dy.getPixel(coord_X,  coord_Y + 1) - image_dy.getPixel(coord_X,  coord_Y - 1);

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

            // записываем значения
            values[mainBasketIndex] += mainBasketValue;
            values[sideBasketIndex] += sideBasketValue;
        }
    }
    // TODO
    auto max = *std::max(values.begin()+1, values.end()-1);
    return max;
}

/*  Инвариантость к вращению TODO */
vector<Descriptor> DescriptorCreator::getDescriptorsInvRotation(const Image &image, const vector<Point> interestPoints,
                                                             const int radius,const int basketCount, const int barCharCount){
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
        double prevDistance = numeric_limits<double>::max();       // Предыдущий
        double minDistance = numeric_limits<double>::max();        // Минимальный
        for(unsigned int j = 0; j < d2.size(); j++){
            double dist = getDistance(d1[i], d2[j]);
            if(dist < minDistance){
                indexSimilar = j;
                prevDistance = minDistance;
                minDistance = dist;
            }
        }

        if(minDistance/prevDistance > treshhold){
            continue;      // отбрасываем
        } else {
            similar.emplace_back(d1[i],d2[indexSimilar]);
        }
    }
    return similar;
}




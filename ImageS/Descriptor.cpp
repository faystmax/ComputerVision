#include "Descriptor.h"
#include "Kernel.h"
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

/* Ориентация точки */
double DescriptorCreator::getPointOrientation(const Image &image_dx, const Image &image_dy,const Point &point, const Kernel &gauss){

    const int basketCount = 36;

    auto radius = (gauss.getWidth()/2);
    auto dimension = 2 * radius;
    auto sector = 2 * M_PI / basketCount;
    auto halfSector = M_PI / basketCount;

    vector<double> baskets(basketCount, 0);
    for (auto i = 0; i < dimension; i++) {
        for (auto j = 0; j < dimension; j++) {
            // координаты
            auto coord_X = i - radius + point.x;
            auto coord_Y = j - radius + point.y;

            // градиент
            auto gradient_X = image_dx.getPixel(coord_X + 1, coord_Y) - image_dx.getPixel(coord_X - 1, coord_Y);
            auto gradient_Y = image_dy.getPixel(coord_X,  coord_Y + 1) - image_dy.getPixel(coord_X,  coord_Y - 1);

            // получаем значение(домноженное на Гаусса) и угол
            auto value = getGradientValue(gradient_X, gradient_Y) * gauss.get(i, j);
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

            // записываем значения
            baskets[firstBasketIndex] += mainBasketValue;
            baskets[secondBasketIndex] += sideBasketValue;
        }
    }
    // Ищем Пики
    auto maxIndex = 0;
    auto prevMaxIndex = 0;
    auto max = 0;
    for(unsigned int i = 0; i < baskets.size(); i++){
        if(max < baskets[i]){
            prevMaxIndex = maxIndex;
            maxIndex = i;
            max = baskets[i];
        }
    }
    // Проверка на 2 пика TODO
//    if(baskets[prevMaxIndex]/baskets[maxIndex] >= 0.8){
//        //Интерполяция параболой
//        max =
//    }

    return maxIndex * sector + halfSector;
}

/*  Инвариантость к вращению TODO */
vector<Descriptor> DescriptorCreator::getDescriptorsInvRotation(const Image &image, const vector<Point> interestPoints,
                                                             const int radius,const int basketCount, const int barCharCount){
    auto sigma = 1.5;
    auto dimension = 2 * radius;
    auto sector = 2 * M_PI / basketCount;
    auto halfSector = M_PI / basketCount;
    auto barCharStep = dimension / (barCharCount / 4);
    auto barCharCountInLine = (barCharCount / 4);
    auto gauss = KernelCreator::getGaussDoubleDim(sigma);

    Image image_dx = ImageConverter::convolution(image, KernelCreator::getSobelX());
    Image image_dy = ImageConverter::convolution(image, KernelCreator::getSobelY());

    vector <Descriptor> descriptors(interestPoints.size());
    for (unsigned int k = 0; k < interestPoints.size(); k++) {
        descriptors[k] = Descriptor(barCharCount * basketCount, interestPoints[k]);
        auto phiRotate = getPointOrientation(image_dx,image_dy,interestPoints[k],gauss);    // Ориентация точки

        for (auto i = 0; i < dimension; i++) {
            for (auto j = 0; j < dimension; j++) {
                // get Gradient
                auto gradient_X = image_dx.getPixel(i - radius + interestPoints[k].x, j - radius + interestPoints[k].y);
                auto gradient_Y = image_dy.getPixel(i - radius + interestPoints[k].x, j - radius + interestPoints[k].y);

                // get value and phi
                auto value = getGradientValue(gradient_X, gradient_Y);
                auto phi = getGradientDirection(gradient_X, gradient_Y) + 2 * M_PI - phiRotate;
                phi = fmod(phi, 2 * M_PI);  //Shift

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
                auto i_Rotate = i * cos(phiRotate) + j * sin(phiRotate);
                auto j_Rotate = -i * sin(phiRotate) + j * cos(phiRotate);
                auto tmp_i = (i_Rotate / barCharStep) * basketCount;
                auto tmp_j = (j_Rotate / barCharStep) * basketCount;

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




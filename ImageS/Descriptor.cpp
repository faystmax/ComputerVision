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
    auto op = [](double a, double b) { return pow(a - b, 2); };
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
                auto tmp_i = i / barCharStep;
                auto tmp_j = j / barCharStep;

                auto indexMain = (tmp_i * barCharCountInLine + tmp_j) * basketCount + firstBasketIndex;
                auto indexSide = (tmp_i * barCharCountInLine + tmp_j) * basketCount + secondBasketIndex;

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
vector<double> DescriptorCreator::getPointOrientation(const Image &image_dx, const Image &image_dy, const Point &point,
                                                      const Kernel &gauss) {

    const int basketCount = 36;

    auto radius = (gauss.getWidth() / 2);
    auto dimension = gauss.getWidth();
    auto sector = 2 * M_PI / basketCount;
    auto halfSector = M_PI / basketCount;

    vector<double> baskets(basketCount, 0);
    for (auto i = 0; i < dimension; i++) {
        for (auto j = 0; j < dimension; j++) {
            // координаты
            auto coord_X = i - radius + point.x;
            auto coord_Y = j - radius + point.y;

            // градиент
            auto gradient_X = image_dx.getPixel(coord_X, coord_Y);
            auto gradient_Y = image_dy.getPixel(coord_X, coord_Y);

            // получаем значение(домноженное на Гаусса) и угол
            auto value = getGradientValue(gradient_X, gradient_Y) * gauss.get(i, j);
            auto phi = getGradientDirection(gradient_X, gradient_Y);

            // получаем индекс корзины в которую входит phi и смежную с ней
            int firstBasketIndex = floor(phi / sector);
            int secondBasketIndex = int(floor((phi - halfSector) / sector) + basketCount) % basketCount;

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
    auto peak_1 = getPeak(baskets);
    auto peak_2 = getPeak(baskets, peak_1);

    vector<double> peaks;
    peaks.push_back(parabaloidInterpolation(baskets, peak_1));
    if (peak_2 != -1 && baskets[peak_2] / baskets[peak_1] >= 0.8) { // Если второй пик не ниже 80%
        peaks.push_back(parabaloidInterpolation(baskets, peak_2));
    }
    return peaks;
}

/* Интерполяция параболой */
double DescriptorCreator::parabaloidInterpolation(const vector<double> &baskets, const int maxIndex) {
    // берём левую и правую корзину и интерполируем параболой
    auto left = baskets[(maxIndex - 1 + baskets.size()) % baskets.size()];
    auto right = baskets[(maxIndex + 1) % baskets.size()];
    auto mid = baskets[maxIndex];

    auto sector = 2 * M_PI / baskets.size();
    auto phi = (left - right) / (2 * (left + right - 2 * mid));
    return (phi + maxIndex) * sector + (sector / 2);
}

/* Поиск пика */
double DescriptorCreator::getPeak(const vector<double> &baskets, const int notEqual) {
    int maxBasketIndex = -1;
    for (unsigned int i = 0; i < baskets.size(); i++) {
        if (baskets[i] > baskets[(i - 1 + baskets.size()) % baskets.size()] && baskets[i] > baskets[(i + 1) % baskets.size()] && i != notEqual) {
            if(maxBasketIndex != -1 && baskets[maxBasketIndex] > baskets[i] ){
                continue;
            }
            maxBasketIndex = i;
        }
    }
    return maxBasketIndex;
}

/*  Инвариантость к вращению TODO */
vector <Descriptor> DescriptorCreator::getDescriptorsInvRotation(const Image &image, const vector <Point> interestPoints,
                                             const int radius, const int basketCount, const int barCharCount) {
    auto sigma = 2;
    auto dimension = 2 * radius;
    auto sector = 2 * M_PI / basketCount;
    auto halfSector = M_PI / basketCount;
    auto barCharStep = dimension / (barCharCount / 4);
    auto barCharCountInLine = (barCharCount / 4);

    auto gauss_1 = KernelCreator::getGaussDoubleDim(sigma);
    auto gauss_2 = KernelCreator::getGaussDoubleDim(sigma * 2);

    Image image_dx = ImageConverter::convolution(image, KernelCreator::getSobelX());
    Image image_dy = ImageConverter::convolution(image, KernelCreator::getSobelY());

    vector <Descriptor> descriptors(interestPoints.size());
    for (unsigned int k = 0; k < interestPoints.size(); k++) {
        descriptors[k] = Descriptor(barCharCount * basketCount, interestPoints[k]);
        auto peaks = getPointOrientation(image_dx, image_dy, interestPoints[k], gauss_1);    // Ориентация точки

        for (auto &phiRotate : peaks) {
            for (auto i = 0 ; i < dimension ; i++) {
                for (auto j = 0 ; j < dimension; j++) {

                    // координаты
                    auto coord_X = i - radius + interestPoints[k].x;
                    auto coord_Y = j - radius + interestPoints[k].y;

                    // градиент
                    auto gradient_X = image_dx.getPixel(coord_X, coord_Y);
                    auto gradient_Y = image_dy.getPixel(coord_X, coord_Y);

                    // получаем значение(домноженное на Гаусса) и угол
                    auto value = getGradientValue(gradient_X, gradient_Y) * gauss_2.get(i, j);
                    auto phi = getGradientDirection(gradient_X, gradient_Y) + 2 * M_PI - phiRotate;
                    phi = fmod(phi, 2 * M_PI);  // Shift

                    // получаем индекс корзины в которую входит phi и смежную с ней
                    int firstBasketIndex = floor(phi / sector);
                    int secondBasketIndex = int(floor((phi - halfSector) / sector) + basketCount) % basketCount;

                    // вычисляем центр
                    auto mainBasketPhi = firstBasketIndex * sector + halfSector;

                    // распределяем L(value)
                    auto mainBasketValue = (1 - (abs(phi - mainBasketPhi) / sector)) * value;
                    auto sideBasketValue = value - mainBasketValue;

                    // вычисляем индекс куда записывать значения
                    int i_Rotate = round((i - radius) * cos(phiRotate) +(j- radius) * sin(phiRotate));
                    int j_Rotate = round(-(i - radius) * sin(phiRotate) + (j- radius) * cos(phiRotate));

                    // отбрасываем
                    if (i_Rotate < -radius || j_Rotate < -radius || i_Rotate >= radius || j_Rotate >= radius) {
//                        std::cout<<"drop "<<i_Rotate<<" "<<j_Rotate <<std::endl;
                        continue;
                    }
//                    std::cout<<i_Rotate<<" "<<j_Rotate <<std::endl;
                    auto tmp_i = (i_Rotate + radius) / barCharStep;
                    auto tmp_j = (j_Rotate + radius) / barCharStep;

                    auto indexMain = (tmp_i * barCharCountInLine + tmp_j) * basketCount + firstBasketIndex;
                    auto indexSide = (tmp_i * barCharCountInLine + tmp_j) * basketCount + secondBasketIndex;

                    // записываем значения
                    descriptors[k].data[indexMain] += mainBasketValue;
                    descriptors[k].data[indexSide] += sideBasketValue;
                }
            }
            descriptors[k].normalize();
            descriptors[k].clampData(0, 0.2);
            descriptors[k].normalize();
        }
    }
    return descriptors;
}

vector <Vector> DescriptorCreator::findSimilar(const vector <Descriptor> &d1, const vector <Descriptor> &d2, const double treshhold) {
    vector <Vector> similar;
    for (unsigned int i = 0; i < d1.size(); i++) {
        int indexSimilar = -1;
        double prevDistance = numeric_limits<double>::max();       // Предыдущий
        double minDistance = numeric_limits<double>::max();        // Минимальный
        for (unsigned int j = 0; j < d2.size(); j++) {
            double dist = getDistance(d1[i], d2[j]);
            if (dist < minDistance) {
                indexSimilar = j;
                prevDistance = minDistance;
                minDistance = dist;
            }
        }

        if (minDistance / prevDistance > treshhold) {
            continue;      // отбрасываем
        } else {
            similar.emplace_back(d1[i], d2[indexSimilar]);
        }
    }
    return similar;
}




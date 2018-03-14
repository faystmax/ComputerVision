#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include "Image.h"
#include "InterestPoints.h"

class DescriptorCreator;

/**
 * @brief The Vector struct
 * for painting
 */
struct IMAGESSHARED_EXPORT Vector {
    Point start;
    Point end;
    Vector(const Point start, const Point end) {
        this->start = start;
        this->end = end;
    }
};

/**
 * @brief The Descriptor class
 * N - Количество корзин * L кол-во гистограмм
 */
class IMAGESSHARED_EXPORT Descriptor {
public:
    Descriptor() = default;
    Descriptor(const int size) { data.resize(size);}
    Descriptor(Descriptor&&) = default;
    Descriptor& operator=(Descriptor&&) = default;

    void normalize();
    double getLength();
    int getSize() const { return data.size(); }
    double getAt(const int index) const {return data[index];}
    void clampData(const double min, const double max);

private:
   vector<double> data; // Гистограммы * Кол-во корзин

   friend DescriptorCreator;
};

/**
 * @brief The DescriptorCreator class
 * creates Descriptors
 */
class IMAGESSHARED_EXPORT DescriptorCreator{
public:
    static double getDistance(const Descriptor &d1, const Descriptor &d2);
    static inline double getGradientValue(const double x, const double y) {return sqrt(x*x + y*y);}
    static inline double getGradientDirection(const double x, const double y) {return atan2(y, x);}

    static vector<Descriptor> getDescriptors(const Image& image, const vector<Point> interestPoints,
                                             const int radius, const int basketCount, const int barCharCount);

};

#endif // DESCRIPTOR_H


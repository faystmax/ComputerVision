#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include "Image.h"
#include "InterestPoints.h"

class DescriptorCreator;

/**
 * @brief The Descriptor class
 */
class IMAGESSHARED_EXPORT Descriptor {
public:
    Descriptor() = default;
    Descriptor(const int size,  Point interPoint);
    Descriptor(const Descriptor&) = default ;
    Descriptor(Descriptor&&) = default;
    Descriptor& operator=(Descriptor&&) = default;
    Descriptor& operator=(const Descriptor&) = default;

    void normalize();
    int getDataSize() const { return data.size(); }
    double getAt(const int index) const {return data[index];}
    Point getInterPoint() const {return interPoint;}
    Point& getInterPointRef() {return interPoint;}
    void clampData(const double min, const double max);
    void setPointXY(const int x, const int y);

    double getSize() const {return size;}
    void setSize(double value) {size = value;}

    double getOrientation() const {return orientation;}
    void setOrientation(double value) {orientation = value;}

    double getCenterAngle() const {return centerAngle;}
    void setCenterAngle(double value) {centerAngle = value;}

    double getCenterDistance() const {return centerDistance;}
    void setCenterDistance(double value) {centerDistance = value;}

private:
    Point interPoint;    // Интересная точка - центр
    vector<double> data; // N - Количество корзин * L кол-во гистограмм

   // Hough
   double centerDistance;
   double centerAngle;
   double orientation;      // Ориентация
   double size;             // Размер дескриптора


   friend DescriptorCreator;
};

/**
 * @brief The Vector struct
 * for painting
 */
struct IMAGESSHARED_EXPORT Vector {
    Descriptor first;
    Descriptor second;
    Vector(Descriptor first, Descriptor second) {
        this->first = first;
        this->second = second;
    }
    Vector(Vector&&) = default;
    Vector(const Vector&) = default ;
    Vector& operator=(Vector&&) = default;
    Vector& operator=(const Vector&) = default;
};

/**
 * @brief The DescriptorCreator class
 * creates Descriptors
 */
class IMAGESSHARED_EXPORT DescriptorCreator{
public:
    // Поиск самих дескрипторов
    static vector<Descriptor> getDescriptors(const Image& image, const vector<Point> interestPoints,
                                             const int radius, const int basketCount, const int barCharCount);
    // Поиск дескрипторов инвариантых к вращению
    static vector<Descriptor> getDescriptorsInvRotation(const Image& image, const vector<Point> interestPoints,
                                             const int radius, const int basketCount, const int barCharCount);

    // Поиск дескрипторов инвариантых к вращению и масштабу
    static vector<Descriptor> getDescriptorsInvRotationScale(Pyramid &pyramid, vector<Point> points, const int _radius,
                                                             const int basketCount, const int barCharCount);

    // Поиск дескрипторов инвариантых к вращению, масштабу и афинным преобразованиям
    static vector<Descriptor> getDescriptorsInvRotationScaleAfinn(Pyramid &pyramid, vector<Point> points, const int _radius,
                                                             const int basketCount, const int barCharCount);

    // Поиск похожих дескрипторов
    static vector<Vector> findSimilar(const vector<Descriptor> &d1, const vector<Descriptor> &d2, const double treshhold = 0.8);
private:

    static double getDistance(const Descriptor &d1, const Descriptor &d2);
    static vector<double> getPointOrientation(const Image& image_dx, const Image& image_dy, const Point &point, const int sigma, const int radius);

    static inline double getGradientValue(const double x, const double y) {return sqrt(x * x + y * y);}
    static inline double getGradientDirection(const double x, const double y) {return atan2(y, x) + M_PI;}

    static double parabaloidInterpolation(const vector<double> &baskets, const int maxIndex);
    static double getPeak(const vector<double> &baskets, const int notEqual =- 1);
};

#endif // DESCRIPTOR_H


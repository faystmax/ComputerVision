#ifndef INTERESTPOINTS_H
#define INTERESTPOINTS_H

#include "images_global.h"
#include "Image.h"
class Image;
class Pyramid;

struct Point {
    int x;
    int y;
    int z;
    double s; // S(x,y) - значение оператора
    Point(int x = 0, int y = 0, double s = 0) {
        this->x = x;
        this->y = y;
        this->s = s;
    }
    Point(int x, int y, int z, double s) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->s = s;
    }
    Point(Point&&) = default;
    Point(const Point&) = default ;
    Point& operator=(Point&&) = default;
    Point& operator=(const Point&) = default;
};

class IMAGESSHARED_EXPORT InterestPoints{
public:

    vector<Point> moravek(const Image &image, const double threshold, const int radius, const int pointsCount);
    vector<Point> harris(const Image &image, const double threshold, const int radius, const int pointsCount);
    vector<Point> blob(const Image &image, const double threshold, const int radius, const int pointsCount);

private:
    // Adaptive Non-Maximum Suppression
    vector<Point> anmsFilter(vector<Point> points, const int pointsCount);

    // Lambda calculation
    double lambda(const Image &image_dx, const Image &image_dy, const int x, const int y, const int radius);

    // PorogFilter
    vector <Point> thresholdFilter(const Image &image_S, const double threshold);

    // Local maximum
    vector <Point> localMaximum(const vector <Point> points, const Image &image_S);

    // Check extremum
    bool isExtremum(const Pyramid& pyramid,const int x,const int y,const int z);
};

#endif // INTERESTPOINTS_H

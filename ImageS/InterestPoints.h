#ifndef INTERESTPOINTS_H
#define INTERESTPOINTS_H

#include "images_global.h"
#include "Image.h"
class Image;

struct Point {
    int x;
    int y;
    double s; // S(x,y) - значение оператора
    Point(int x = 0, int y = 0, double s = 0) {
        this->x = x;
        this->y = y;
        this->s = s;
    };
    Point(Point&&) = default;
    Point(const Point&) = default ;
    Point& operator=(Point&&) = default;
    Point& operator=(const Point&) = default;
};

class IMAGESSHARED_EXPORT InterestPoints{
public:

    vector<Point> moravek(Image &image, const double threshold, const int radius, const int pointsCount);
    vector<Point> harris(Image &image,const double threshold,const int radius,const int pointsCount);

private:
    // Adaptive Non-Maximum Suppression
    vector<Point> anmsFilter(vector<Point> points, const int pointsCount);

    // Lambda calculation
    double lambda(const Image &image_dx, const Image &image_dy, const int x, const int y, const int radius);

    // PorogFilter
    vector <Point> thresholdFilter(Image &image_S, const double threshold);

    // Local maximum
    vector <Point> localMaximum(const  vector <Point> points);
};

#endif // INTERESTPOINTS_H

#ifndef INTERESTPOINTS_H
#define INTERESTPOINTS_H

#include "images_global.h"
#include "iostream"
class Image;

using namespace std;

struct Point {
    int x;
    int y;
    double s; // S(x,y) - значение опеератора
    Point(int x = 0, int y = 0, double s = 0) {
        this->x = x;
        this->y = y;
        this->s = s;
    };
};

class IMAGESSHARED_EXPORT InterestPoints{
public:

    vector<Point> moravek(Image &image, const double threshold, const int radius, const int pointsCount);
    vector<Point> harris(Image &image,const double threshold,const int radius,const int pointsCount);

private:
    // Adaptive Non-Maximum Suppression
    vector<Point> filter(vector<Point> points, const int pointsCount);

    // Lambda calculation
    double lambda(const Image &image_dx, const Image &image_dy, const int x, const int y, const int radius);

    // PorogFilter
    vector <Point> porogFilter(Image &image, const double porog);

    // local maximum
    vector <Point> localMaximum(const  vector <Point> points);
};

#endif // INTERESTPOINTS_H

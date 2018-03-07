#ifndef INTERESTPOINTS_H
#define INTERESTPOINTS_H

#include "Image.h"

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

    vector<Point> moravek(const Image &image,const double porog,const int radius,const int pointsCount);
    vector<Point> harris(const Image &image,const double porog,const int radius,const int pointsCount);

private:
    // Adaptive Non-Maximum Suppression
    vector<Point> filter(vector<Point> &points, const int pointsCount);

    // Lambda calculation
    double lambda(const Image &imgX, const Image &imgY, const int x, const int y, const int radius);

    // PorogFilter
    vector <Point> porogFilter(vector<double> &pointsS, const Image& image, const double porog, const int radius);
};

#endif // INTERESTPOINTS_H

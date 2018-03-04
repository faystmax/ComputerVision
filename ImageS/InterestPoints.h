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

    vector<Point> &moravek(const Image &image, double porog, int radius, int pointsCount);
    vector<Point>& harris(const Image &image, double porog, int radius, int pointsCount);

private:
    vector<Point> points;

    // Adaptive Non-Maximum Suppression
    void filter(vector<Point> &points, const int pointsCount);

    // Lambda calculation
    double lambda(const Image &imgX, const Image &imgY, int x0, int y0, int width, int height);
};

#endif // INTERESTPOINTS_H

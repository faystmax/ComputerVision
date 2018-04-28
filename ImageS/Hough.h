#ifndef HOUGH_H
#define HOUGH_H

#include "Descriptor.h"


struct IMAGESSHARED_EXPORT Transform {
    double x;
    double y;
    double size;
    double angle;
    double width;
    double height;

    Transform(const double x,const double y,const double size,const double angle,const double width,const double height) {
        this->x = x;
        this->y = y;
        this->size = size;
        this->angle = angle;
        this->width = width;
        this->height = height;
    }
};


class IMAGESSHARED_EXPORT Hough
{
public:
    Hough();

    vector<Transform> search(vector<Vector> &lines, const Image& obj, const Image &img,
                             const int widthStep, const int heightStep, const double sizeStep, const int angleStep);
    void calcCenterDistanceAndAngle(vector<Descriptor>& desc, const Image& image);
};

#endif // HOUGH_H

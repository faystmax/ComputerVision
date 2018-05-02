#ifndef HOUGH_H
#define HOUGH_H

#include "Descriptor.h"
#include "iostream"

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

struct Space {
    vector<double> data;
    int width;
    int height;
    int size;
    int angle;

    Space(const int width, const int height, const int size, const int angle) {
        this->width = width;
        this->height = height;
        this->size = size;
        this->angle = angle;
        data.resize(width * height * size * angle, 0);
    }

    // Возвращает нужный элемент
    double getAt(const int x, const int y, const int z, const int k){
        return data[x + y * width + z * width * height + k * width * height * size];
    }

    // Добавляет 1
    void add(const int x, const int y, const int z, const int k){
        data[x + y * width + z * width * height + k * width * height * size]++;
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

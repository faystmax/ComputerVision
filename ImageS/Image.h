#ifndef IMAGE_H
#define IMAGE_H

#include "images_global.h"
#include "Kernel.h"
#include "InterestPoints.h"

using namespace std;

class IMAGESSHARED_EXPORT Image{

public:
    enum EdgeEffect{ Black, Repeat, Mirror,  Wrapping };

    Image();
    Image(Image&&) = default;
    Image(const Image&) = default ;
    Image& operator=(Image&&) = default;
    Image& operator=(const Image&) = default;
    Image operator-(const Image& right)const;
    Image(const int width , const int height, const EdgeEffect edgeEffect = Repeat );
    ~Image() = default;

    double getPixel(const int x,const int y) const;
    void setPixel(const int x,const int y, double pixel);
    void setPixelNoValidation(const int x,const int y, double pixel);
    static bool sizeEq(Image& img1,Image& img2);
    vector<double> deNormolize() const;

    int getHeight() const {return height;}
    int getWidth() const {return width;}

    void setEdgeEffect(const EdgeEffect edgeEffect) {this->edgeEffect = edgeEffect;}
    EdgeEffect getEdgeEffect() const {return this->edgeEffect;}
private:
    int height;
    int width;
    EdgeEffect edgeEffect;
    vector<double> pixels;

    double getPixelRepeat(int x ,int y) const;
    double getPixelMirror(int x ,int y) const;
    double getPixelWrapping(int x ,int y) const;
    void normolize();
};

#endif // IMAGE_H

#ifndef PYRAMID_H
#define PYRAMID_H

#include "Image.h"

struct Item {
        int octave;
        int scale;
        double sigmaScale;
        double sigmaEffect;
        Image image;
        Item(Image image, int octave, double scale, double sigmaScale, double sigmaEffect):
        image(image){
            this->octave = octave;
            this->scale = scale;
            this->sigmaScale = sigmaScale;
            this->sigmaEffect = sigmaEffect;
        }
};

class IMAGESSHARED_EXPORT Pyramid{
public:
    Pyramid() = default;
    Pyramid(const Image &image, const int scales = 2, double sigma = 1, double sigmaStart = 0.5);

    int L(int x, int y, double sigma) const;
    int getItemsSize() const {return items.size();}
    Item getItem(int index) const {return items.at(index);}

private:
    vector<Item> items;

    double getDeltaSigma(double sigmaPrev,double sigmaNext) const;
    Image  getLastImage() const;
};

#endif // PYRAMID_H

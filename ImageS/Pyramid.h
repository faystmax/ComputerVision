#ifndef PYRAMID_H
#define PYRAMID_H

#include "Image.h"

struct Item {
        int octave;
        int scale;
        double sigmaScale;
        double sigmaEffect;
        unique_ptr<Image> image;
        Item(Image* image, int octave, double scale, double sigmaScale, double sigmaEffect)
            : image(image){
            this->octave = octave;
            this->scale = scale;
            this->sigmaScale = sigmaScale;
            this->sigmaEffect = sigmaEffect;
        }
};

class IMAGESSHARED_EXPORT Pyramid{
public:
    Pyramid();

    void generate(Image &image, int scales = 2, double sigma = 1);

    int getItemsSize() {return items.size();}
    Item* getItem(int index) {return &items.at(index);}

private:
    vector<Item> items;

    double getDeltaSigma(double sigmaPrev,double sigmaNext);
    Image* getLastImage();
};

#endif // PYRAMID_H

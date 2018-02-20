#ifndef BLACKEDGEEFFECT_H
#define BLACKEDGEEFFECT_H

#include "images_global.h"
#include "IEdgeEffect.h"

class IMAGESSHARED_EXPORT BlackEdgeEffect : public IEdgeEffect {
public:
    BlackEdgeEffect();
    ~BlackEdgeEffect();
    virtual double getPixel(int x ,int y, Image& image);
};

#endif // BLACKEDGEEFFECT_H

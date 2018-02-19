#ifndef MIRROREDGEEFFECT_H
#define MIRROREDGEEFFECT_H

#include "images_global.h"
#include "IEdgeEffect.h"
#include "Image.h"

class IMAGESSHARED_EXPORT MirrorEdgeEffect: public IEdgeEffect {
public:
    MirrorEdgeEffect();
    ~MirrorEdgeEffect();
    virtual double getPixel(int x ,int y,Image& image);
};

#endif // MIRROREDGEEFFECT_H

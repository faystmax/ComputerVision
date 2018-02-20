#ifndef REPEATEDGEEFFECT_H
#define REPEATEDGEEFFECT_H

#include "images_global.h"
#include "IEdgeEffect.h"
#include "Image.h"

class IMAGESSHARED_EXPORT RepeatEdgeEffect : public IEdgeEffect {
public:
    RepeatEdgeEffect();
    ~RepeatEdgeEffect();
    virtual double getPixel(int x ,int y,Image& image);
};

#endif // REPEATEDGEEFFECT_H

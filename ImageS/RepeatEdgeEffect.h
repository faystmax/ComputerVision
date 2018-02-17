#ifndef REPEATEDGEEFFECT_H
#define REPEATEDGEEFFECT_H

#include "IEdgeEffect.h"

class IMAGESSHARED_EXPORT RepeatEdgeEffect : public IEdgeEffect {

public:
    RepeatEdgeEffect();
    virtual double getPixel(int x ,int y,Image& image);
};

#endif // REPEATEDGEEFFECT_H

#ifndef IEDGEEFFECT_H
#define IEDGEEFFECT_H

#include <Image.h>

class IEdgeEffect
{

public:
    virtual double getPixel(int x ,int y,Image& image) = 0;
};

#endif // IEDGEEFFECT_H

#include "MirrorEdgeEffect.h"

MirrorEdgeEffect::MirrorEdgeEffect() {

}

MirrorEdgeEffect::~MirrorEdgeEffect() {

}

double MirrorEdgeEffect::getPixel(int x, int y, Image &image) {
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    if (x >= image.getWidth()) x = 2 * image.getWidth() - x - 1;
    if (y >= image.getHeight()) y = 2 * image.getHeight() - y - 1;

    return image.getPixels()[x][y];
}

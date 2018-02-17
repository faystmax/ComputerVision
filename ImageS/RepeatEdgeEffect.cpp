#include "RepeatEdgeEffect.h"

RepeatEdgeEffect::RepeatEdgeEffect() {

}

double RepeatEdgeEffect::getPixel(int x, int y, Image &image) {
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= image.getWidth()) x = image.getWidth() - 1;
    if (y >= image.getHeight()) y = image.getHeight() - 1;

    return image.getPixels()[x][y];
}


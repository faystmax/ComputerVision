#include "BlackEdgeEffect.h"
#include "Image.h"

BlackEdgeEffect::BlackEdgeEffect() {

}

BlackEdgeEffect::~BlackEdgeEffect() {

}

double BlackEdgeEffect::getPixel(int x, int y, Image &image) {
    if (x < 0 || y < 0 || x >= image.getWidth() || y >= image.getHeight()) {
        return 0;
    }
    return image.getPixels()[x][y];
}

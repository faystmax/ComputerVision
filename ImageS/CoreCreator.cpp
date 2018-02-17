#include "CoreCreator.h"

CoreCreator::CoreCreator() {
}

Core *CoreCreator::getBlur() {
    const double koef = 0.1;
    double core[3][3] = {{koef, koef, koef},
                         {koef, koef, koef},
                         {koef, koef, koef}};
    return new Core(3, 3, core);
}

Core *CoreCreator::getClarity() {
    double core[3][3] = {{-1, -1, -1},
                         {-1, 9,  -1},
                         {-1, -1, -1}};
    return new Core(3, 3, core);
}

#include "Hough.h"

Hough::Hough() {
}

Transform Hough::search(vector<Vector> &lines, const Image &img1, const Image &img2) {

    int width = img1.getWidth() / 50;
    int height = img1.getHeight() / 50;
    int sizeMax = 10;
    int angle = 360/30;
    double angleStep = 2 * M_PI / angle;
    double sizeStep = 2 / sizeMax;

    double parameterSpace[width][height][sizeMax][angle] = {0};      /* Пространство параметров */

    for (size_t i = 0; i < lines.size(); i++) {
        double diff_angle = lines[i].second.getOrientation() - lines[i].first.getOrientation();
        double main_angle = diff_angle + lines[i].first.getCenterAngle();

        double size = lines[i].second.getSize() / lines[i].first.getSize();
        double distance = lines[i].first.getCenterDistance() * size;

        double diff_x = distance * cos(main_angle);
        double diff_y = distance * sin(main_angle);

        int index_X = (lines[i].second.getInterPoint().x +  diff_x) / 50;
        int index_Y = (lines[i].second.getInterPoint().y +  diff_y) / 50;
        int index_Size = size / sizeStep; // TODO
        int index_Angle = diff_angle / angleStep;


        // Интерполируем (размазываем)
        for (int m1=-1; m1 <= 1; m1++)
            for (int m2=-1; m2 <= 1; m2++)
                for (int m3=-1; m3 <= 1; m3++)
                    for (int m4=-1; m4 <= 1; m4++) {
                        parameterSpace[(index_X + m1 + width) % width]
                        [(index_Y + m2 + height) % height]
                        [(index_Size + m3 + sizeMax) % sizeMax]
                        [(index_Angle + m4 + angle) % angle]++;
                    }
    }

    double x, y, bestSize, bestAngle;
    int bestVotes = 0;

    // Ищем максимальный
    for (int i = 1; i < width - 1; i++) {
        for (int j = 1; j < height - 1; j++) {
            for (int k = 0; k < sizeMax - 1; k++) {
                for (int n = 0; n < angle - 1; n++) {
                    if (parameterSpace[i][j][k][n] > bestVotes) {
                        x = i;
                        y = j;
                        bestSize = k;
                        bestAngle = n;
                        bestVotes = parameterSpace[i][j][k][n];
                    }
                }
            }
        }
    }

    Transform transform(50 * x + 25, 50 * y + 25,  bestSize, bestAngle* angleStep + angleStep/2, img1.getWidth() * bestSize, img1.getHeight() * bestSize);
    return transform;
}

void Hough::calcCenterDistanceAndAngle(vector<Descriptor> &desc, const Image &image) {
    double x_center = image.getWidth()/2.;
    double y_center = image.getHeight()/2.;

    for (size_t i = 0; i < desc.size(); i++) {
        desc[i].setCenterDistance(hypot(desc[i].getInterPoint().x - x_center, desc[i].getInterPoint().y - y_center));
        desc[i].setCenterAngle(atan2(desc[i].getInterPoint().y - y_center, desc[i].getInterPoint().x - x_center));
    }
}

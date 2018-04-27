#include "Hough.h"
#include "iostream"
#include <cstring>

Hough::Hough() {
}

// Пространство параметров - объявляем здесь так, как нехватает места в стеке при маленьких step
double parameterSpace[100][100][20][40];

void clear() {
    memset(parameterSpace, 0, 100*100*20*40*(sizeof(double)));
}

vector<Transform> Hough::search(vector<Vector> &lines, const Image &obj, const Image &img,
                                const int widthStep, const int heightStep, const double sizeStep, const int angleStep) {
    int width = img.getWidth() / widthStep;
    int height = img.getHeight() / heightStep;
    int sizeCount = 2 / sizeStep;
    int angleCount = 360 / angleStep;

    // Шаг в радианах
    double angleStepRad = 2 * M_PI / angleCount;

    // чистим пространство параметров
    clear();

    for (size_t i = 0; i < lines.size(); i++) {
    double diff_angle = lines[i].second.getOrientation() - lines[i].first.getOrientation();
        double main_angle = diff_angle + lines[i].first.getCenterAngle();

        double size = lines[i].second.getSize() / lines[i].first.getSize();
        double distance = lines[i].first.getCenterDistance() * size;

        // разница по x и y от искомой точки
        double diff_x = distance * cos(main_angle);
        double diff_y = distance * sin(main_angle);

        int index_X = (lines[i].second.getInterPoint().x - diff_x) / widthStep;
        int index_Y = (lines[i].second.getInterPoint().y - diff_y) / heightStep;
        int index_Size = size / sizeStep;
        int index_Angle = diff_angle / angleStepRad;
        std::cout<< index_X <<" "<<index_Y<<" "<<index_Size<<" "<<index_Angle<<std::endl;

        // Пропускаем если ушли за границу
        if (index_X < 1 || index_Y < 1 || index_X >= width-1 || index_Y >= height-1 ||
                index_Size < 1 || index_Size >= sizeCount-1 || index_Angle < 1 || index_Angle >= angleCount-1) {
            continue;
        }

        // Интерполируем (размазываем)
        for (int m1=-1; m1 <= 1; m1++)
            for (int m2=-1; m2 <= 1; m2++)
                for (int m3=-1; m3 <= 1; m3++)
                    for (int m4=-1; m4 <= 1; m4++) {
                        parameterSpace[index_X + m1]
                        [index_Y + m2]
                        [index_Size + m3]
                        [index_Angle + m4]++;
                    }
    }

    vector<Transform> peaks;
    double x, y, size, angle;
    int max = 0;

    // Ищем максимальный
    for (int i = 1; i < width - 1; i++) {
    for (int j = 1; j < height - 1; j++) {
            for (int k = 1; k < sizeCount - 1; k++) {
                for (int n = 1; n < angleCount - 1; n++) {
                    if (parameterSpace[i][j][k][n] > max) {
                        x = i;
                        y = j;
                        size = k;
                        angle = n;
                        max = parameterSpace[i][j][k][n];
                    }
                }
            }
        }
    }

    double resultSize = size * sizeStep + 0.5 * sizeStep;
                        peaks.emplace_back(widthStep * x + 0.5 * widthStep, heightStep * y + 0.5 * heightStep, resultSize,
                                           angle * angleStepRad + 0.5 * angleStepRad, obj.getWidth() * resultSize, obj.getHeight() * resultSize);
                        return peaks;
}


void Hough::calcCenterDistanceAndAngle(vector<Descriptor> &desc, const Image &image) {
    double x_center = image.getWidth()/2.;
    double y_center = image.getHeight()/2.;

    for (size_t i = 0; i < desc.size(); i++) {
        desc[i].setCenterDistance(hypot(desc[i].getInterPoint().x - x_center, desc[i].getInterPoint().y - y_center));
        desc[i].setCenterAngle(atan2(desc[i].getInterPoint().y - y_center, desc[i].getInterPoint().x - x_center));
    }
}

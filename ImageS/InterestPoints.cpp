#include "InterestPoints.h"
#include "KernelCreator.h"
#include "ImageConverter.h"


vector <Point> &InterestPoints::moravek(const Image &image, double porog, int radius, int pointsCount) {

    double max = 0;
    vector<double> pointsS(image.getWidth() * image.getHeight());
    for (int x = radius; x < image.getWidth() - radius; x++) {
        for (int y = radius; y < image.getHeight() - radius; y++) {
            vector<double> directDiff(8, 0); // 8 directions
            for (int u = -radius; u < radius; u++) {
                for (int v = -radius; v < radius; v++) {
                    double pixel = image.getPixel(x + u, y + v);

                    directDiff[0] = pixel - image.getPixel(x + u, y + v - 1);
                    directDiff[1] = pixel - image.getPixel(x + u + 1, y + v);
                    directDiff[2] = pixel - image.getPixel(x + u, y + v + 1);
                    directDiff[3] = pixel - image.getPixel(x + u - 1, y + v);
                    directDiff[4] = pixel - image.getPixel(x + u + 1, y + v - 1);
                    directDiff[5] = pixel - image.getPixel(x + u + 1, y + v + 1);
                    directDiff[6] = pixel - image.getPixel(x + u - 1, y + v + 1);
                    directDiff[7] = pixel - image.getPixel(x + u - 1, y + v - 1);

                    for (int i = 0; i < 8; i++) {
                        pointsS[i] += directDiff[i] * directDiff[i];
                    }
                }
            }

            pointsS[x + y * image.getWidth()] = *std::min_element(directDiff.begin(), directDiff.end());
            if (pointsS[x + y * image.getWidth()] > max) {
                max = pointsS[x + y * image.getWidth()];
            }
        }
    }

    vector <Point> points;
    for (int i = radius; i < image.getWidth() - radius; i += 2) {
        for (int j = radius; j < image.getHeight() - radius; j += 2) {
            if (pointsS[i + j * image.getWidth()] >= porog) {
                points.push_back(Point(i, j, pointsS[i + j * image.getWidth()]));
            }
        }
    }

    std::sort(points.begin(), points.end(), [](auto &p1, auto &p2) { return p1.s > p2.s; });
    filter(points, pointsCount);
    return this->points;
}

vector <Point> &InterestPoints::harris(const Image &image, double porog, int radius, int pointsCount) {

    Image copyImageX = ImageConverter::convolution(image, KernelCreator::getSobelX());
    Image copyImageY = ImageConverter::convolution(image, KernelCreator::getSobelY());

    vector<double> pointsS(image.getWidth() * image.getHeight());
    for (int x = radius; x < image.getWidth() - radius; x++) {
        for (int y = radius; y < image.getHeight() - radius; y++) {
            pointsS[x + y * image.getWidth()] = lambda(copyImageX, copyImageY, x, y, radius, radius);
        }
    }

    vector <Point> points;
    for (int i = radius; i < image.getWidth() - radius; i += 2) {
        for (int j = radius; j < image.getHeight() - radius; j += 2) {
            if (pointsS[i + j * image.getWidth()] >= porog) {
                points.push_back(Point(i, j, pointsS[i + j * image.getWidth()]));
            }
        }
    }

    std::sort(points.begin(), points.end(), [](auto &p1, auto &p2) { return p1.s > p2.s; });
    filter(points, pointsCount);
    return this->points;
}

void InterestPoints::filter(vector <Point> &points, const int pointsCount) {

    vector<bool> flagUsedPoints(points.size(), true);
    int radius = 1;
    int usedPointsCount = points.size();
    while (usedPointsCount > pointsCount) {
        for (unsigned int i = 0; i < points.size(); i++) {

            if (!flagUsedPoints[i]) {
                continue;
            }

            Point &p1 = points[i];
            for (unsigned int j = i + 1; j < points.size(); j++) {
                if (flagUsedPoints[j]) {
                    Point &p2 = points[j];
                    if (sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)) <= radius && p1.s > p2.s) {
                        flagUsedPoints[j] = false;
                        usedPointsCount--;
                        if (usedPointsCount <= pointsCount) {
                            break;
                        }
                    }
                }
            }
        }
        radius++;
    }
    for (unsigned int i = 0; i < points.size(); i++) {
        if (flagUsedPoints[i]) {
            this->points.push_back(points[i]);
        }
    }
}

double InterestPoints::lambda(const Image &imgX, const Image &imgY, int x0, int y0, int width, int height) {
    double A = 0;
    double B = 0;
    double C = 0;
    for (int i = x0; i < x0 + width; i++) {
        for (int j = y0; j < y0 + height; j++) {
            double curA = imgX.getPixel(i, j);
            double curB = imgY.getPixel(i, j);
            A += curA * curA;
            B += curA * curB;
            C += curB * curB;
        }
    }
    double descreminant = sqrt(pow(A - C, 2) + 4 * B * B);
    double lambdaMin = min(abs((A + C - descreminant) / 2), abs((A + C + descreminant) / 2));
    return lambdaMin;
}


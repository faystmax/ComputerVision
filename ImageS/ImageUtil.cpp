#include <QImage>
#include <QPixmap>
#include <QPainter>
#include "Image.h"
#include "Pyramid.h"
#include "InterestPoints.h"
#include "Descriptor.h"

QImage getOutputImage(const Image &image) {
    QImage resultImage(image.getWidth(), image.getHeight(), QImage::Format_ARGB32);
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double pixel = image.getPixel(i, j);
            resultImage.setPixel(i, j, qRgb(pixel, pixel, pixel));
        }
    }
    return resultImage;
}

Image constructImage(const QImage &image, const Image::EdgeEffect edgeEffect = Image::EdgeEffect::Mirror) {

    Image resultImage(image.width(), image.height(), edgeEffect);

    // Read pixels and form black and white image
    for (int i = 0; i < resultImage.getWidth(); i++) {
        for (int j = 0; j < resultImage.getHeight(); j++) {
            QRgb pixel = image.pixel(i, j);
            resultImage.setPixel(i, j, 0.213 * qRed(pixel) + 0.715 * qGreen(pixel) + 0.072 * qBlue(pixel));
        }
    }
    return resultImage;
}

QImage createFromL(const Image &image, const Pyramid &pyramid, const double sigma) {
    QImage resultImage(image.getWidth(), image.getHeight(), QImage::Format_ARGB32);
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double pixel = pyramid.L(i, j, sigma);
            resultImage.setPixel(i, j, qRgb(pixel, pixel, pixel));
        }
    }
    return resultImage;
}

QImage createImageWithPoints(const Image &image, const vector <Point> &points) {
    QImage resultImage = getOutputImage(image);
    for (unsigned int i = 0; i < points.size(); i++) {
        resultImage.setPixel(points[i].x - 1, points[i].y - 1, qRgb(255, 0, 0));
        resultImage.setPixel(points[i].x - 1, points[i].y, qRgb(255, 0, 0));
        resultImage.setPixel(points[i].x - 1, points[i].y + 1, qRgb(255, 0, 0));

        resultImage.setPixel(points[i].x, points[i].y - 1, qRgb(255, 0, 0));
        resultImage.setPixel(points[i].x, points[i].y, qRgb(255, 0, 0));
        resultImage.setPixel(points[i].x, points[i].y + 1, qRgb(255, 0, 0));

        resultImage.setPixel(points[i].x + 1, points[i].y - 1, qRgb(255, 0, 0));
        resultImage.setPixel(points[i].x + 1, points[i].y, qRgb(255, 0, 0));
        resultImage.setPixel(points[i].x + 1, points[i].y + 1, qRgb(255, 0, 0));
    }
    return resultImage;
}

QImage glueImages(const Image &imageLeft, const Image &imageRight) {

    // max height
    auto height = max(imageLeft.getHeight(),imageRight.getHeight());

    QImage resultImage(imageLeft.getWidth() + imageRight.getWidth(), height, QImage::Format_ARGB32);
    // imageLeft
    for (auto i = 0; i < imageLeft.getWidth(); i++) {
        for (auto j = 0; j < imageLeft.getHeight(); j++) {
            double pixel = imageLeft.getPixel(i, j);
            resultImage.setPixel(i, j, qRgb(pixel, pixel, pixel));
        }
    }

    // imageRight
    for (auto i = 0; i < imageRight.getWidth(); i++) {
        for (auto j = 0; j < imageRight.getHeight(); j++) {
            double pixel = imageRight.getPixel(i, j);
            resultImage.setPixel(i + imageLeft.getWidth(), j, qRgb(pixel, pixel, pixel));
        }
    }
    return resultImage;
}

void drawLines(QImage& image, const int firstWidth, vector<Vector> similar){
    QPainter painter(&image);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    for (Vector& vec :similar) {
        Point p1 = vec.first.getInterPoint();
        Point p2 = vec.second.getInterPoint();
        painter.drawLine (p1.x, p1.y, p2.x + firstWidth,  p2.y);
    }
    painter.end ();
}

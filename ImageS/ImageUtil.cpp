#include <QImage>
#include <QPixmap>
#include <QPainter>
#include "Image.h"
#include "Pyramid.h"
#include "InterestPoints.h"
#include "Descriptor.h"

QImage getOutputImage(const Image &image) {
    QImage resultImage(image.getWidth(), image.getHeight(), QImage::Format_ARGB32);
//    vector<double> newPixels = image.deNormolize();
    for (int i = 0; i < image.getWidth(); i++) {
        for (int j = 0; j < image.getHeight(); j++) {
            double pixel = image.getPixel(i, j);
            resultImage.setPixel(i, j, qRgb(pixel, pixel, pixel));
        }
    }
    return resultImage;
}

Image constructImage(const QImage &image, const Image::EdgeEffect edgeEffect = Image::EdgeEffect::Repeat) {

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

inline vector<QColor> randomColors(int count){
    vector<QColor> colors;
    float currentHue = 0.0;
    for (int i = 0; i < count; i++){
        colors.push_back( QColor::fromHslF(currentHue, 1.0, 0.5) );
        currentHue += 0.618033988749895f;
        currentHue = std::fmod(currentHue, 1.0f);
    }
    return colors;
}

void drawLines(QImage& image, const int firstWidth, vector<Vector> similar){
    QPainter painter(&image);
    QPen pen;
    pen.setWidth(1);
    vector<QColor> colors = randomColors(similar.size());
    for (unsigned int i = 0; i < similar.size(); i++) {
        pen.setColor(colors[i]);
        painter.setPen(pen);
        Point p1 = similar[i].first.getInterPoint();
        Point p2 = similar[i].second.getInterPoint();
        painter.drawLine (p1.x, p1.y, p2.x + firstWidth,  p2.y);

        // Circle 1
        double radius1 = sqrt(2) * p1.sigmaEffect;
        painter.drawEllipse(QRect(p1.x - radius1, p1.y - radius1, 2 * radius1, 2 * radius1));

         // Circle 2
        double radius2 = sqrt(2) * p2.sigmaEffect;
        painter.drawEllipse(QRect(p2.x + firstWidth - radius2, p2.y - radius2, 2 * radius2, 2 * radius2));

    }
    painter.end();
}

QImage createImageWithPointsBlob(const Image &image, const vector <Point> &points) {
    QImage resultImage = getOutputImage(image);
    QPainter painter(&resultImage);
    QPen pen;
    pen.setWidth(2);
    vector<QColor> colors = randomColors(points.size());
    for (unsigned int i = 0; i < points.size(); i++) {
        pen.setColor(colors[i]);
        painter.setPen(pen);
        double radius = sqrt(2) * points[i].sigmaEffect;
        painter.drawEllipse(QRect(points[i].x - radius, points[i].y - radius, 2 * radius, 2 * radius));
        painter.drawPoint(points[i].x, points[i].y);
    }
    painter.end();
    return resultImage;
}


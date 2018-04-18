#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include "Image.h"
#include "Pyramid.h"
#include "InterestPoints.h"
#include "Ransac.h"
using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void on_openImageButton_clicked();
    void on_blurButton_clicked();
    void on_clarityButton_clicked();
    void on_sobelButton_clicked();
    void on_priutButton_clicked();
    void on_gaussButton_clicked();
    void on_edgeEffectComboBox_currentIndexChanged(int index);

    void on_pyramidButton_clicked();
    void on_pyramidLeftButton_clicked();
    void on_pyramidRightButton_clicked();
    void on_generateLImageButton_clicked();
    void on_moravekButton_clicked();
    void on_harrisButton_clicked();

    void on_shiftButton_clicked();
    void on_noiseButton_clicked();
    void on_rotateButton_clicked();
    void on_rotaterButton_clicked();
    void on_blobButton_clicked();
    void on_scaleButton_clicked();
    void on_reloadButton_clicked();
    void on_scosButton_clicked();
    void on_scaleXButton_clicked();
    void on_scaleYButton_clicked();
    void on_invRotScaleButton_clicked();
    void on_invRotScaleAffButton_clicked();
    void on_invRotButton_clicked();

    void on_glueButton_clicked();

    void on_reloadPanoramButton_clicked();

    void on_affineButton_clicked();

private:
    int curPyramidIdex = 0;
    Ui::MainWindow *ui;
    Image image;
    Image imageOriginal;
    Pyramid pyramid;
    InterestPoints interestPoints;
    Ransac ransac;

    void showImage(const QImage &image);
    void showImage(const Image &image);
    void enableButtons(bool enable);
    void showPyramidInfo(const Item &item);
    void reloadImages();
};

#endif // MAINWINDOW_H

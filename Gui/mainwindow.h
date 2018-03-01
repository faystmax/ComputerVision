#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include "Image.h"
#include "Pyramid.h"

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

private:
    int curPyramidIdex = 0;
    Ui::MainWindow *ui;
    Image image;
    Pyramid pyramid;

    void showImage(const QImage &image);
    void showImage(const Image &image);
    void enableButtons(bool enable);
    void showPyramidInfo(const Item &item);
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Image.h>
#include <memory>

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

private:
    Ui::MainWindow *ui;
    Image* image = nullptr;

    void showImage(Image* image);
};

#endif // MAINWINDOW_H

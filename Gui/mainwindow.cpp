#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QImageReader>
#include <iostream>

#include "RepeatEdgeEffect.h"
#include "ImageConverter.h"
#include "CoreCreator.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->ui->graphicsView->setScene(new QGraphicsScene());
}

MainWindow::~MainWindow() {
    delete ui;
    delete this->image;
}

void MainWindow::on_openImageButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), nullptr,
                                                    tr("Image Files (*.png *.jpg *.bmp)"));
    QImage image(fileName);

    if (!image.isNull()) {
        // Clear if not null
        if (this->image != nullptr)
            delete this->image;

        // Init our Image
        this->image = new Image(image, new RepeatEdgeEffect());

        // Show image in window
        showImage(this->image);
    }
}

void MainWindow::on_blurButton_clicked() {
    if (this->image != nullptr) {
        ImageConverter::convolution(this->image, CoreCreator::getBlur());
        showImage(this->image);
    }
}

void MainWindow::on_clarityButton_clicked() {
    if (this->image != nullptr) {
        ImageConverter::convolution(this->image, CoreCreator::getClarity());
        showImage(this->image);
    }
}


void MainWindow::showImage(Image *image) {
    this->ui->graphicsView->scene()->clear();
    this->ui->graphicsView->scene()->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(image->getOutputImage())));
}





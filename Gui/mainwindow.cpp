#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QImageReader>
#include <QLineEdit>

#include "RepeatEdgeEffect.h"

#include "MirrorEdgeEffect.h"
#include "ImageConverter.h"
#include "CoreCreator.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->ui->graphicsView->setScene(new QGraphicsScene());

    // Fill combobox edgeEffects
    ui->edgeEffectComboBox->addItem("Black");
    ui->edgeEffectComboBox->addItem("Repeat");
    ui->edgeEffectComboBox->addItem("Mirror");
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
        this->image = new Image(image, getEdgeEffect());

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

void MainWindow::on_sobelButton_clicked() {
    if (this->image != nullptr) {
        ImageConverter::sobel(this->image);
        showImage(this->image);
    }
}

void MainWindow::on_priutButton_clicked() {
    if (this->image != nullptr) {
        ImageConverter::priut(this->image);
        showImage(this->image);
    }
}

void MainWindow::on_gaussButton_clicked() {
    if (this->image != nullptr) {
        ImageConverter::convolution(this->image, CoreCreator::getGauss(5, 5, 0.5));
        showImage(this->image);
    }
}

IEdgeEffect *MainWindow::getEdgeEffect() {
    switch (this->ui->edgeEffectComboBox->currentIndex()) {
        case (0):
            return new BlackEdgeEffect();
            break;
        case (1):
            return new RepeatEdgeEffect();
            break;
        case (2):
            return new MirrorEdgeEffect();
            break;
    }
    return nullptr;
}

void MainWindow::on_edgeEffectComboBox_currentIndexChanged(int index) {
    if (this->image != nullptr) {
        this->image->setEdgeEffect(getEdgeEffect());
    }
}

/**
 * @brief Show image in graphicsView
 * @param image
 */
void MainWindow::showImage(Image *image) {
    this->ui->graphicsView->scene()->clear();
    this->ui->graphicsView->scene()->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(image->getOutputImage())));
}




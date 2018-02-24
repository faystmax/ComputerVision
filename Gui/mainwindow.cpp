#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QImageReader>
#include <QGraphicsPixmapItem>

#include "KernelCreator.h"
#include "ImageConverter.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->ui->graphicsView->setScene(new QGraphicsScene());

    // Fill combobox edgeEffects
    ui->edgeEffectComboBox->addItem("Mirror");
    ui->edgeEffectComboBox->addItem("Repeat");
    ui->edgeEffectComboBox->addItem("Black");
    ui->edgeEffectComboBox->addItem("Wrapping");

    // Disable all active buttons
    enableButtons(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_openImageButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", nullptr, "Image Files (*.png *.jpg *.bmp)");
    QImage qImage(fileName);

    if (!qImage.isNull()) {
        // Init our Image
        this->image.reset(new Image(qImage));
        ui->edgeEffectComboBox->setCurrentIndex(0);

        // Show image in window
        showImage();

        // Enable all active buttons
        enableButtons(true);
    }
}

void MainWindow::on_blurButton_clicked() {
    ImageConverter::convolution(*this->image.data(), *CoreCreator::getBlur().data());
    showImage();
}

void MainWindow::on_clarityButton_clicked() {
    ImageConverter::convolution(*this->image.data(), *CoreCreator::getClarity().data());
    showImage();
}

void MainWindow::on_sobelButton_clicked() {
    ImageConverter::sobel(*this->image.data());
    showImage();
}

void MainWindow::on_priutButton_clicked() {
    ImageConverter::priut(*this->image.data());
    showImage();
}

void MainWindow::on_gaussButton_clicked() {
    ImageConverter::convolution(*this->image.data(), *CoreCreator::getGauss(5, 5, 4).data());
    showImage();
}

void MainWindow::on_edgeEffectComboBox_currentIndexChanged(int index) {
    if (!this->image.isNull()) {
        switch (index) {
            case (0):
                return this->image.data()->setEdgeEffect(Image::EdgeEffect::Mirror);
                break;
            case (1):
                return this->image.data()->setEdgeEffect(Image::EdgeEffect::Repeat);
                break;
            case (2):
                return this->image.data()->setEdgeEffect(Image::EdgeEffect::Black);
                break;
            case (3):
                return this->image.data()->setEdgeEffect(Image::EdgeEffect::Wrapping);
                break;
        }
    }
}

/**
 * @brief Show image in graphicsView
 * @param image
 */
void MainWindow::showImage() {
    this->ui->graphicsView->scene()->clear();
    this->ui->graphicsView->scene()->addItem(
            new QGraphicsPixmapItem(QPixmap::fromImage(this->image.data()->getOutputImage())));
}

/**
 * @brief Enable or disable active buttons on widget
 * @param enable
 */
void MainWindow::enableButtons(bool enable) {
    this->ui->blurButton->setEnabled(enable);
    this->ui->clarityButton->setEnabled(enable);
    this->ui->gaussButton->setEnabled(enable);
    this->ui->priutButton->setEnabled(enable);
    this->ui->sobelButton->setEnabled(enable);
}




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
        showImage(this->image.get());

        // Enable all active buttons
        enableButtons(true);
    }
}

void MainWindow::on_blurButton_clicked() {
    unique_ptr <Image> resultImage = ImageConverter::convolution(*this->image.get(), *KernelCreator::getBlur().get());
    this->image.reset(resultImage.release());
    showImage(this->image.get());
}

void MainWindow::on_clarityButton_clicked() {
    unique_ptr <Image> resultImage = ImageConverter::convolution(*this->image.get(),
                                                                 *KernelCreator::getClarity().get());
    this->image.reset(resultImage.release());
    showImage(this->image.get());
}

void MainWindow::on_sobelButton_clicked() {
    unique_ptr <Image> resultImage = ImageConverter::sobel(*this->image.get());
    this->image.reset(resultImage.release());
    showImage(this->image.get());
}

void MainWindow::on_priutButton_clicked() {
    unique_ptr <Image> resultImage = ImageConverter::priut(*this->image.get());
    this->image.reset(resultImage.release());
    showImage(this->image.get());
}

void MainWindow::on_gaussButton_clicked() {
    unique_ptr <Image> resultImage = ImageConverter::convolution(*this->image.get(),
                                                                 *KernelCreator::getGauss(5, 5, 4).get());
    this->image.reset(resultImage.release());
    showImage(this->image.get());
}

void MainWindow::on_pyramidButton_clicked() {
    pyramid.reset(new Pyramid());
    pyramid.get()->generate(*this->image.get(), this->ui->scalesSpinBox->value(), this->ui->sigmaSpinBox->value());
    curPyramidIdex = 0;
    showImage(pyramid.get()->getItem(curPyramidIdex)->image.get());

    //Enable buttons
    this->ui->pyramidLeftButton->setEnabled(true);
    this->ui->pyramidRightButton->setEnabled(true);
}

void MainWindow::on_pyramidLeftButton_clicked() {
    if (curPyramidIdex > 0) curPyramidIdex--;
    showImage(pyramid.get()->getItem(curPyramidIdex)->image.get());
    showPyramidInfo(pyramid.get()->getItem(curPyramidIdex));
}

void MainWindow::on_pyramidRightButton_clicked() {
    if (curPyramidIdex < pyramid.get()->getItemsSize() - 1) curPyramidIdex++;
    showImage(pyramid.get()->getItem(curPyramidIdex)->image.get());
    showPyramidInfo(pyramid.get()->getItem(curPyramidIdex));
}

void MainWindow::showPyramidInfo(Item *item) {
    this->ui->infoPyramidTextEdit->setText(
            QString::fromStdString("Octave:     ") + QString::number(item->octave) + QString::fromStdString("<br>") +
            QString::fromStdString("Scale:      ") + QString::number(item->scale) + QString::fromStdString("<br>") +
            QString::fromStdString("SigmaScale: ") + QString::number(item->sigmaScale) +
            QString::fromStdString("<br>") +
            QString::fromStdString("SigmaEffect:") + QString::number(item->sigmaEffect) +
            QString::fromStdString("<br>"));
}


void MainWindow::on_edgeEffectComboBox_currentIndexChanged(int index) {
    if (this->image.get() != nullptr) {
        switch (index) {
            case (0):
                return this->image.get()->setEdgeEffect(Image::EdgeEffect::Mirror);
                break;
            case (1):
                return this->image.get()->setEdgeEffect(Image::EdgeEffect::Repeat);
                break;
            case (2):
                return this->image.get()->setEdgeEffect(Image::EdgeEffect::Black);
                break;
            case (3):
                return this->image.get()->setEdgeEffect(Image::EdgeEffect::Wrapping);
                break;
        }
    }
}

/**
 * @brief Show image in graphicsView
 * @param image
 */
void MainWindow::showImage(Image *image) {
    this->ui->graphicsView->scene()->clear();
    this->ui->graphicsView->scene()->addItem(
            new QGraphicsPixmapItem(QPixmap::fromImage(image->getOutputImage())));
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
    this->ui->pyramidButton->setEnabled(enable);
}





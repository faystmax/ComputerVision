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
    ui->edgeEffectComboBox->addItem("Mirror");
    ui->edgeEffectComboBox->addItem("Repeat");
    ui->edgeEffectComboBox->addItem("Black");

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
    ImageConverter::convolution(*this->image.data(), CoreCreator::getBlur().data());
    showImage();
}

void MainWindow::on_clarityButton_clicked() {
    ImageConverter::convolution(*this->image.data(), CoreCreator::getClarity().data());
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
    ImageConverter::convolution(*this->image.data(), CoreCreator::getGauss(5, 5, 0.5).data());
    showImage();
}

void MainWindow::on_edgeEffectComboBox_currentIndexChanged(int index) {
    if (!this->image.isNull()) {
        switch (index) {
            case (0):
                return this->image.data()->setEdgeEffect(new MirrorEdgeEffect());
                break;
            case (1):
                return this->image.data()->setEdgeEffect(new RepeatEdgeEffect());
                break;
            case (2):
                return this->image.data()->setEdgeEffect(new BlackEdgeEffect());
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




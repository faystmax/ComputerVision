#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QImageReader>
#include <QGraphicsPixmapItem>
#include <iostream>

#include "ImageUtil.cpp"
#include "KernelCreator.h"
#include "ImageConverter.h"
#include "Descriptor.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->ui->graphicsView->setScene(new QGraphicsScene());

    // Fill combobox edgeEffects
    ui->edgeEffectComboBox->addItem("Repeat");
    ui->edgeEffectComboBox->addItem("Mirror");
    ui->edgeEffectComboBox->addItem("Black");
    ui->edgeEffectComboBox->addItem("Wrapping");

    this->imageOriginal =  constructImage(QImage(":/resource/img/resource/img/lenna.jpg"));
    this->image = constructImage(QImage(":/resource/img/resource/img/lenna.jpg"));
    showImage(this->image);
    reloadImages();

    // Disable all active buttons
    //enableButtons(false);
}

MainWindow::~MainWindow() {
    delete ui;
}




void MainWindow::on_openImageButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", nullptr, "Image Files (*.png *.jpg *.bmp)");
    QImage qImage(fileName);

    if (!qImage.isNull()) {
        // Init our Image
        this->imageOriginal = this->image;
        this->image = constructImage(qImage);
        ui->edgeEffectComboBox->setCurrentIndex(0);

        // Show image in window
        showImage(this->image);

        // Enable all active buttons
        enableButtons(true);
    }
}

void MainWindow::on_blurButton_clicked() {
    this->image = ImageConverter::convolution(this->image, KernelCreator::getBlur());
    showImage(this->image);
}

void MainWindow::on_clarityButton_clicked() {
    this->image = ImageConverter::convolution(this->image, KernelCreator::getClarity());
    showImage(this->image);
}

void MainWindow::on_sobelButton_clicked() {
    this->image = ImageConverter::sobel(this->image);
    showImage(this->image);
}

void MainWindow::on_priutButton_clicked() {
    this->image = ImageConverter::priut(this->image);
    showImage(this->image);
}

void MainWindow::on_gaussButton_clicked() {
    this->image = ImageConverter::convolution(this->image, KernelCreator::getGauss(1));
    showImage(this->image);
}

void MainWindow::on_shiftButton_clicked(){
    this->image = ImageConverter::convolution(this->image, KernelCreator::getShift());
    showImage(this->image);
}

void MainWindow::on_noiseButton_clicked(){
    this->image = ImageConverter::noise(this->image, 2000);
    showImage(this->image);
}
void MainWindow::on_rotateButton_clicked(){
    this->image = ImageConverter::rotate(this->image);
    showImage(this->image);
}

void MainWindow::on_scosButton_clicked(){
    //TODO
    this->image = ImageConverter::convolution(this->image, KernelCreator::getScos());
    showImage(this->image);
}



/* Pyramids */
void MainWindow::on_pyramidButton_clicked() {
    this->pyramid = Pyramid(this->image, this->ui->scalesSpinBox->value(), this->ui->sigmaSpinBox->value(),
                            this->ui->sigmaStartSpinBox->value());
    curPyramidIdex = 0;
    showImage(pyramid.getItem(curPyramidIdex).image);
    showPyramidInfo(pyramid.getItem(curPyramidIdex));

    //Enable buttons
    this->ui->pyramidLeftButton->setEnabled(true);
    this->ui->pyramidRightButton->setEnabled(true);
}

void MainWindow::on_pyramidLeftButton_clicked() {
    if (curPyramidIdex > 0) curPyramidIdex--;
    showImage(pyramid.getItem(curPyramidIdex).image);
    showPyramidInfo(pyramid.getItem(curPyramidIdex));
}

void MainWindow::on_pyramidRightButton_clicked() {
    if (curPyramidIdex < pyramid.getItemsSize() - 1) curPyramidIdex++;
    showImage(pyramid.getItem(curPyramidIdex).image);
    showPyramidInfo(pyramid.getItem(curPyramidIdex));
}

void MainWindow::on_generateLImageButton_clicked() {
    double sigma = this->ui->sigmaLSpinBox->value();
    QImage result = createFromL(image, pyramid, sigma);
    showImage(result);
}

void MainWindow::showPyramidInfo(const Item &item) {
    this->ui->infoPyramidTextEdit->setText(
            QString::fromStdString("Octave:     ") + QString::number(item.octave) + QString::fromStdString("<br>") +
            QString::fromStdString("Scale:      ") + QString::number(item.scale) + QString::fromStdString("<br>") +
            QString::fromStdString("SigmaScale: ") + QString::number(item.sigmaScale) + QString::fromStdString("<br>") +
                QString::fromStdString("SigmaEffect:") + QString::number(item.sigmaEffect) + QString::fromStdString("<br>"));
}

void MainWindow::reloadImages(){
    this->imageOriginal =  constructImage(QImage(":/resource/img/resource/img/lenna.jpg"));
    this->image = constructImage(QImage(":/resource/img/resource/img/lenna.jpg"));
    showImage(this->image);
}

/* Interest Points */
void MainWindow::on_moravekButton_clicked() {
    vector <Point> points = interestPoints.moravek(this->image, this->ui->ThresholdSpinBox->value(),
                                                   this->ui->radiusSpinBox->value(),
                                                   this->ui->pointsCountSpinBox->value());
    showImage(createImageWithPoints(this->image, points));
}

void MainWindow::on_harrisButton_clicked() {
    vector <Point> points = interestPoints.harris(this->image, this->ui->ThresholdSpinBox->value(),
                                                  this->ui->radiusSpinBox->value(),
                                                  this->ui->pointsCountSpinBox->value());
    showImage(createImageWithPoints(this->image, points));
}

void MainWindow::on_blobButton_clicked() {
    Pyramid pyramid(this->image);
    vector <Point> points = interestPoints.blob(pyramid, this->ui->ThresholdSpinBox->value(),
                                                  this->ui->radiusSpinBox->value(),
                                                  this->ui->pointsCountSpinBox->value());
    interestPoints.restorePoints(pyramid, points);
    showImage(createImageWithPointsBlob(this->image, points));
}

/* Descriptors */
void MainWindow::on_invRotButton_clicked(){
    // Vars
    int treshold = this->ui->ThresholdSpinBox->value();
    int radius = this->ui->radiusSpinBox->value();
    int pointsCount = this->ui->pointsCountSpinBox->value();
    int radiusDesc = this->ui->radiusDescSpinBox->value();
    int basketCount = this->ui->basketCountDescSpinBox->value();
    int barCharCount = this->ui->barCharCountDescSpinBox->value();
    double T = this->ui->TSpinBox->value();

    vector <Point> points1 = interestPoints.harris(this->imageOriginal, treshold, radius, pointsCount);
    vector <Descriptor> descriptors1 = DescriptorCreator::getDescriptorsInvRotation(this->imageOriginal, points1, radiusDesc, basketCount, barCharCount);

    vector <Point> points2 = interestPoints.harris(this->image, treshold, radius, pointsCount);
    vector <Descriptor> descriptors2 = DescriptorCreator::getDescriptorsInvRotation(this->image,  points2, radiusDesc,basketCount, barCharCount);

    // Glue and draw
    QImage result = glueImages(this->imageOriginal, this->image);
    vector<Vector>  similar = DescriptorCreator::findSimilar(descriptors1, descriptors2, T);
    drawLines(result, this->imageOriginal.getWidth(), similar);
    showImage(result);
}

void MainWindow::on_invRotScaleButton_clicked(){
    // Vars
    int treshold = this->ui->ThresholdSpinBox->value();
    int radius = this->ui->radiusSpinBox->value();
    int pointsCount = this->ui->pointsCountSpinBox->value();
    int radiusDesc = this->ui->radiusDescSpinBox->value();
    int basketCount = this->ui->basketCountDescSpinBox->value();
    int barCharCount = this->ui->barCharCountDescSpinBox->value();
    double T = this->ui->TSpinBox->value();

    Pyramid pyramid_1(this->imageOriginal);
    vector <Point> points1 = interestPoints.blob(pyramid_1, treshold, radius, pointsCount);
    vector <Descriptor> descriptors1 = DescriptorCreator::getDescriptorsInvRotationScale(pyramid_1, points1, radiusDesc, basketCount, barCharCount);

    Pyramid pyramid_2(this->image);
    vector <Point> points2 = interestPoints.blob(pyramid_2, treshold, radius, pointsCount);
    vector <Descriptor> descriptors2 = DescriptorCreator::getDescriptorsInvRotationScale(pyramid_2,  points2, radiusDesc,basketCount, barCharCount);

    // Glue and draw
    QImage result = glueImages(this->imageOriginal, this->image);
    vector<Vector>  similar = DescriptorCreator::findSimilar(descriptors1, descriptors2, T);
    drawLinesAndCircles(result, this->imageOriginal.getWidth(), similar);
    showImage(result);
}

void MainWindow::on_invRotScaleAffButton_clicked(){
    // Vars
    int treshold = this->ui->ThresholdSpinBox->value();
    int radius = this->ui->radiusSpinBox->value();
    int pointsCount = this->ui->pointsCountSpinBox->value();
    int radiusDesc = this->ui->radiusDescSpinBox->value();
    int basketCount = this->ui->basketCountDescSpinBox->value();
    int barCharCount = this->ui->barCharCountDescSpinBox->value();
    double T = this->ui->TSpinBox->value();

    Pyramid pyramid_1(this->imageOriginal);
    vector <Point> points1 = interestPoints.blob(pyramid_1, treshold, radius, pointsCount);
    vector <Descriptor> descriptors1 = DescriptorCreator::getDescriptorsInvRotationScaleAfinn(pyramid_1, points1, radiusDesc, basketCount, barCharCount);

    Pyramid pyramid_2(this->image);
    vector <Point> points2 = interestPoints.blob(pyramid_2, treshold, radius, pointsCount);
    vector <Descriptor> descriptors2 = DescriptorCreator::getDescriptorsInvRotationScaleAfinn(pyramid_2,  points2, radiusDesc,basketCount, barCharCount);

    // Glue and draw
    QImage result = glueImages(this->imageOriginal, this->image);
    vector<Vector>  similar = DescriptorCreator::findSimilar(descriptors1, descriptors2, T);
    drawLinesAndCircles(result, this->imageOriginal.getWidth(), similar);
    showImage(result);
}



void MainWindow::on_rotaterButton_clicked() {
    QImage outputImage = getOutputImage(this->image);
    QPoint center = outputImage.rect().center();
    QMatrix matrix;
    matrix.translate(center.x(), center.y());
    matrix.rotate(this->ui->rotateSpinBox->value());
    QImage dstImg = outputImage.transformed(matrix);
    this->image = constructImage(dstImg);
    showImage(this->image);
}


void MainWindow::on_scaleButton_clicked(){
    QImage outputImage = getOutputImage(this->image);
    QImage rotateImage = outputImage.scaled(outputImage.width() + this->ui->scaleSpinBox->value(), outputImage.height() + this->ui->scaleSpinBox->value(),Qt::KeepAspectRatio);
    this->image = constructImage(rotateImage);
    showImage(this->image);
}

void MainWindow::on_scaleXButton_clicked(){
    QImage outputImage = getOutputImage(this->image);
    QImage rotateImage = outputImage.scaled(outputImage.width() + this->ui->scaleSpinBox->value(), outputImage.height());
    this->image = constructImage(rotateImage);
    showImage(this->image);
}

void MainWindow::on_scaleYButton_clicked(){
    QImage outputImage = getOutputImage(this->image);
    QImage rotateImage = outputImage.scaled(outputImage.width() , outputImage.height() + this->ui->scaleSpinBox->value());
    this->image = constructImage(rotateImage);
    showImage(this->image);
}


void MainWindow::on_edgeEffectComboBox_currentIndexChanged(int index) {
    switch (index) {
        case (0):
            return this->image.setEdgeEffect(Image::EdgeEffect::Repeat);
            break;
        case (1):
            return this->image.setEdgeEffect(Image::EdgeEffect::Mirror);
            break;
        case (2):
            return this->image.setEdgeEffect(Image::EdgeEffect::Black);
            break;
        case (3):
            return this->image.setEdgeEffect(Image::EdgeEffect::Wrapping);
            break;
    }
}

/**
 * @brief Show image in graphicsView
 * @param image
 */
void MainWindow::showImage(const Image &image) {
    this->ui->graphicsView->scene()->clear();
    QImage outputImage = getOutputImage(image);
    this->ui->graphicsView->scene()->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(outputImage)));
}

/**
 * @brief Show image in graphicsView
 * @param image
 */
void MainWindow::showImage(const QImage &image) {
    this->ui->graphicsView->scene()->clear();
    this->ui->graphicsView->scene()->addItem(new QGraphicsPixmapItem(QPixmap::fromImage(image)));
}

/**
 * @brief Enable or disable active buttons on widget
 * @param enable
 */
void MainWindow::enableButtons(bool enable) {
    this->ui->blurButton->setEnabled(enable);
    this->ui->gaussButton->setEnabled(enable);
    this->ui->priutButton->setEnabled(enable);
    this->ui->sobelButton->setEnabled(enable);
    this->ui->shiftButton->setEnabled(enable);
    this->ui->noiseButton->setEnabled(enable);
    this->ui->harrisButton->setEnabled(enable);
    this->ui->rotateButton->setEnabled(enable);
    this->ui->clarityButton->setEnabled(enable);
    this->ui->moravekButton->setEnabled(enable);
    this->ui->pyramidButton->setEnabled(enable);
    this->ui->generateLImageButton->setEnabled(enable);
}

void MainWindow::on_reloadButton_clicked(){
    reloadImages();
}


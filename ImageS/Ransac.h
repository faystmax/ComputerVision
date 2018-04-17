#ifndef RANSAC_H
#define RANSAC_H

#include "linalg.h"
#include "Descriptor.h"

using namespace alglib;

class Ransac;

class IMAGESSHARED_EXPORT Matrix {
  public:
    Matrix() = default;
    Matrix(const int rows, const int cols);
    Matrix(const int rows, const int cols, const vector<double> data);
    Matrix(const Matrix &) = default ;
    Matrix(Matrix &&) = default;
    Matrix &operator=(Matrix &&) = default;
    Matrix &operator=(const Matrix &) = default;

    int getRows() const {return rows;}
    int getCols() const {return cols;}
    int getInliers() const {return inliers;}

    double at(const int i, const int j) const {return data[i * cols + j];}
    void set(const int i, const int j, const double value) { data[i * cols + j] = value;}

private:
    int rows;
    int cols;
    vector<double> data;
    int inliers = 0;                // Количество inliers

    friend Ransac;
};

class IMAGESSHARED_EXPORT Ransac {
  public:
    Ransac();

    Matrix search(vector<Vector> &lines, const double threshhold);
  private:
    vector<int> get4RandomNumbers(const int max);
    Matrix getHypothesis(Vector &line_1, Vector &line_2, Vector &line_3, Vector &line_4);
    int countInliers(const Matrix &hyp, const vector<Vector> &lines, const double threshhold);

    // методы для работы с матрицей
    Matrix transpose(const Matrix &matr);
    Matrix multiply(const Matrix &matr_1, const Matrix &matr_2);

};

#endif // RANSAC_H

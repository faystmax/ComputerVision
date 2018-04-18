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
    Matrix(const int rows, const int cols, const array<double, 100> &data);
    Matrix(const Matrix &) = default ;
    Matrix(Matrix &&) = default;
    Matrix &operator=(Matrix &&) = default;
    Matrix &operator=(const Matrix &) = default;

    int getRows() const {return rows;}
    int getCols() const {return cols;}

    double at(const int i, const int j) const {return data[i * cols + j];}
    void set(const int i, const int j, const double value) { data[i * cols + j] = value;}

    // Методы для работы с матрицей
    static Matrix transpose(const Matrix &matr);
    static Matrix multiply(const Matrix &matr_1, const Matrix &matr_2);

private:
    int rows;
    int cols;
    array<double, 10 * 10> data;

    friend Ransac;
};

class IMAGESSHARED_EXPORT Ransac {
  public:
    Ransac();

    // Поиск матрицы преобразования
    Matrix search(vector<Vector> &lines, const double threshhold);
    // Получает новые координаты из старых и матрицы преобразования
    static Matrix convert(const Matrix& transMatrix, const int x, const int y);

  private:
    Matrix getHypothesis(Vector &line_1, Vector &line_2, Vector &line_3, Vector &line_4);
    int countInliers(const Matrix &hyp, const vector<Vector> &lines, const double threshhold);
};

#endif // RANSAC_H

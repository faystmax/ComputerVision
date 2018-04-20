#ifndef RANSAC_H
#define RANSAC_H

#include "linalg.h"
#include "Descriptor.h"

using namespace alglib;

class Ransac;

template <int Rows, int Cols>
class IMAGESSHARED_EXPORT Matrix {
  public:
    Matrix() = default;
    Matrix(const int x, const int y);
    Matrix<Rows,Cols>(const array<double, Rows * Cols> array);
    Matrix(const Matrix &) = default ;
    Matrix(Matrix &&) = default;
    Matrix &operator=(Matrix &&) = default;
    Matrix &operator=(const Matrix &) = default;

    int getRows() const {return Rows;}
    int getCols() const {return Cols;}

    double at(const int i, const int j) const {return data[i * Cols + j];}
    void set(const int i, const int j, const double value) { data[i * Cols + j] = value;}
    void setData(const array<double, Rows * Cols> &data) { this->data = data;}
    array<double, Rows * Cols> getData() const { return data;}

private:
    array<double, Rows * Cols> data;
};


// Методы для работы с матрицей
template <int rows, int cols>
static Matrix<cols, rows> transpose(const Matrix<rows, cols> &matr);

template <int rows_1, int cols_2, int cols_rows>
static Matrix<rows_1, cols_2> multiply(const Matrix<rows_1, cols_rows> &matr_1, const Matrix<cols_rows, cols_2> &matr_2);

class IMAGESSHARED_EXPORT Ransac {
  public:
    Ransac();

    // Поиск матрицы преобразования
    Matrix<9, 1> search(vector<Vector> &lines, const double threshhold);

    // Получает новые координаты из старых и матрицы преобразования
    static Matrix<3, 1> convert(const Matrix<9, 1>& transMatrix, const int x, const int y);

  private:
    Matrix<9, 1> getHypothesis(Vector &line_1, Vector &line_2, Vector &line_3, Vector &line_4);
    int countInliers(const Matrix<9, 1> &hyp, const vector<Vector> &lines, const double threshhold);
    Matrix<9, 1> correctDLT(const Matrix<9, 1> &hyp, vector<Vector> &lines);
};

#endif // RANSAC_H

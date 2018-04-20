#include "Ransac.h"
#include "iostream"
#include <cstdlib>
#include <ctime>
#include <algorithm>


template<int Rows, int Cols>
Matrix<Rows, Cols>::Matrix(const int x, const int y)
{
    this->data[0] = x;
    this->data[1] = y;
    this->data[2] = 1;
}

template<int Rows, int Cols>
Matrix<Rows, Cols>::Matrix(const array<double, Rows * Cols> array){
    this->data = array;
}

Ransac::Ransac() {
    srand(time(0));
}

Matrix<9, 1> Ransac::search(vector<Vector> &lines, const double threshhold) {

    int numbers[4];
    int bestInliers = -1;
    Matrix<9, 1> bestHypothesis;

    for (auto i = 0; i < 200; i++) {
        // Генерим рандомные числа
        std::generate_n(numbers, 4, [&lines]() {return std::rand() % lines.size();});

        // Получаем гипотезу
        Matrix<9, 1> hypothesis = getHypothesis(lines[numbers[0]],lines[numbers[1]],lines[numbers[2]],lines[numbers[3]]);

        // Считаем inliers
        int inliers = countInliers(hypothesis, lines, threshhold);
        if (inliers > bestInliers) {
            bestInliers = inliers;
            bestHypothesis = std::move(hypothesis);
        }
    }

    return correctDLT(bestHypothesis,lines);
}

Matrix<9, 1> Ransac::correctDLT(const Matrix<9, 1> &hyp, vector<Vector> &lines)
{
//    int numbers[4];
//    int bestInliers = -1;
//    Matrix<9, 1> bestHypothesis;

//    for (auto i = 0; i < 200; i++) {
//        // Генерим рандомные числа
//        std::generate_n(numbers, 4, [&lines]() {return std::rand() % lines.size();});

//        // Получаем гипотезу
//        Matrix<9, 1> hypothesis = getHypothesis(lines[numbers[0]],lines[numbers[1]],lines[numbers[2]],lines[numbers[3]]);

//        // Считаем inliers
//        int inliers = countInliers(hypothesis, lines, threshhold);
//        if (inliers > bestInliers) {
//            bestInliers = inliers;
//            bestHypothesis = std::move(hypothesis);
//        }
//    }
    return hyp;
}

Matrix<3, 1> Ransac::convert(const Matrix<9, 1> &transMatrix, const int x, const int y) {
    Matrix<3, 1> a(x, y);

    // Строим матрицу h 3 на 3
    Matrix<3, 3> h(transMatrix.getData());

    // Находим новые координаты
    return multiply(h, a);
}

Matrix<9, 1> Ransac::getHypothesis(Vector &line_1, Vector &line_2, Vector &line_3, Vector &line_4) {
    // Переименовываем для простоты использования
    double x1 = line_1.second.getInterPointRef().x;
    double y1 = line_1.second.getInterPointRef().y;
    double x1_s = line_1.first.getInterPointRef().x;
    double y1_s = line_1.first.getInterPointRef().y;

    double x2 = line_2.second.getInterPointRef().x;
    double y2 = line_2.second.getInterPointRef().y;
    double x2_s = line_2.first.getInterPointRef().x;
    double y2_s = line_2.first.getInterPointRef().y;

    double x3 = line_3.second.getInterPointRef().x;
    double y3 = line_3.second.getInterPointRef().y;
    double x3_s = line_3.first.getInterPointRef().x;
    double y3_s = line_3.first.getInterPointRef().y;

    double x4 = line_4.second.getInterPointRef().x;
    double y4 = line_4.second.getInterPointRef().y;
    double x4_s = line_4.first.getInterPointRef().x;
    double y4_s = line_4.first.getInterPointRef().y;

    // Инициализируем матрицу A
    array<double, 8 * 9> matr_A_data = { x1, y1, 1, 0,   0,  0, -x1_s * x1, -x1_s * y1, -x1_s,
                                         0,  0,  0, x1, y1,  1, -y1_s * x1, -y1_s * y1, -y1_s,
                                         x2, y2, 1, 0,   0,  0, -x2_s * x2, -x2_s * y2, -x2_s,
                                         0,  0,  0, x2, y2,  1, -y2_s * x2, -y2_s * y2, -y2_s,
                                         x3, y3, 1, 0,   0,  0, -x3_s * x3, -x3_s * y3, -x3_s,
                                         0,  0,  0, x3, y3,  1, -y3_s * x3, -y3_s * y3, -y3_s,
                                         x4, y4, 1, 0,   0,  0, -x4_s * x4, -x4_s * y4, -x4_s,
                                         0,  0,  0, x4, y4,  1, -y4_s * x4, -y4_s * y4, -y4_s,
                                       };

    // Транспонируем и перемножаем
    Matrix<8, 9> matr_A(matr_A_data);
    matr_A.setData(matr_A_data);
    Matrix<9, 8> transp_A = transpose(matr_A);
    Matrix<9, 9> matr_ATA = multiply(transp_A, matr_A);

    // Кладём в real_2d_array
    real_2d_array matr, u, vt;
    matr.setcontent(9, 9, &matr_ATA.getData().at(0));

    // Считаем SVD
    real_1d_array w;
    bool isSucces = rmatrixsvd(matr, 9, 9, 2, 0, 2, w, u, vt);
    Q_ASSERT(isSucces);

    // так как  W - contains singular values in descending order.
    // берём последний столбец в u
    Matrix<9, 1> hypothesis;
    double koef = 1.0 / u[8][u.cols()-1];  // Делим на последний элемент в матрице - чтоб h22 = 1
    for (auto i = 0; i < hypothesis.getRows(); i++) {
        hypothesis.set(i, 0, koef * u[i][u.cols()-1]);
    }

    return hypothesis;
}

int Ransac::countInliers(const Matrix<9, 1> &hyp, const vector<Vector> &lines, const double threshhold) {
    int inliers = 0;
    for (auto i = 0; i < lines.size(); i++) {
        // Находим новые координаты
        Matrix<3, 1> newCoord = convert(hyp, lines[i].second.getInterPoint().x, lines[i].second.getInterPoint().y);

        // Считаем разницу
        double distance = sqrt(pow(newCoord.at(0,0) - lines[i].first.getInterPoint().x, 2) +
                               pow(newCoord.at(1,0) - lines[i].first.getInterPoint().y, 2));
        if (distance <= threshhold) {
            inliers++;
        }
    }
    return inliers;
}

/* Транспонированеие */
template <int rows, int cols>
Matrix<cols, rows> transpose(const Matrix<rows, cols> &matr) {
    Matrix<cols, rows> result;
    for (auto i = 0; i < rows; i++) {
        for (auto j = 0; j < cols; j++) {
            result.set(j, i, matr.at(i, j));
        }
    }
    return result;
}

/* Перемножение */

template <int rows_1, int cols_2, int cols_rows>
Matrix<rows_1, cols_2> multiply(const Matrix<rows_1, cols_rows> &matr_1, const Matrix<cols_rows, cols_2> &matr_2) {
    Matrix<rows_1, cols_2> result;
    for (auto i = 0; i < rows_1; i++) {
        for (auto j = 0; j < cols_2; j++) {
            double sum = 0;
            for (auto k = 0; k < cols_rows; k++) {
                sum += matr_1.at(i, k) * matr_2.at(k, j);
            }
            result.set(i, j, sum);
        }
    }
    return result;
}


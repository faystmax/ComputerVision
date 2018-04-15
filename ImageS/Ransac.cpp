#include "Ransac.h"
#include "iostream"
#include <cstdlib>
#include <ctime>



Matrix::Matrix(const int rows, const int cols) {
    this->rows = rows;
    this->cols = cols;
    this->data.resize(rows * cols, 0);
}

Matrix::Matrix(const int rows, const int cols, const vector<double> data) {
    Q_ASSERT(data.size() == rows * cols);
    this->rows = rows;
    this->cols = cols;
    this->data = data;

}

Matrix::Matrix(const real_1d_array &matr) {
    Q_ASSERT(matr.length() == 9);
    double koef = 1.0 / matr[8];
    this->cols = 1;
    this->rows = 1;

    this->data = vector<double>(matr.getcontent(), matr.getcontent() + matr.length());
    for (auto &elem : this->data) {
        elem *= koef;
    }
}

Ransac::Ransac() {
    srand(time(0));
}

Matrix Ransac::search(vector<Vector> &lines, const double threshhold) {

    vector<Matrix> matrixes;

    for (auto i = 0; i < 200; i++) {
        vector<int> numbers = get4RandomNumbers(lines.size());
        Matrix hypothesis = getHypothesis(lines[numbers[0]],lines[numbers[1]],lines[numbers[2]],lines[numbers[3]]);

        //Считаем inliers
        hypothesis.inliers = countInliers(hypothesis, lines, threshhold);
        matrixes.push_back(hypothesis);
    }
    // Сортируем и берём самый подходящий
    std::sort(matrixes.begin(), matrixes.end(), [](const Matrix &m_1, const Matrix &m_2) {return m_1.inliers > m_2.inliers;});
    return matrixes[0];
}

Matrix Ransac::getHypothesis(Vector &line_1, Vector &line_2, Vector &line_3, Vector &line_4) {
    // Переименовываем для простоты использования
    double x1 = line_1.first.getInterPointRef().x;
    double y1 = line_1.first.getInterPointRef().y;
    double x1_s = line_1.second.getInterPointRef().x;
    double y1_s = line_1.second.getInterPointRef().y;

    double x2 = line_2.first.getInterPointRef().x;
    double y2 = line_2.first.getInterPointRef().y;
    double x2_s = line_2.second.getInterPointRef().x;
    double y2_s = line_2.second.getInterPointRef().y;

    double x3 = line_3.first.getInterPointRef().x;
    double y3 = line_3.first.getInterPointRef().y;
    double x3_s = line_3.second.getInterPointRef().x;
    double y3_s = line_3.second.getInterPointRef().y;

    double x4 = line_4.first.getInterPointRef().x;
    double y4 = line_4.first.getInterPointRef().y;
    double x4_s = line_4.second.getInterPointRef().x;
    double y4_s = line_4.second.getInterPointRef().y;

    // Инициализируем матрицу A
    vector<double> matr_A_data = { x1, y1, 1, 0,   0,  0, -x1_s * x1, -x1_s * y1, -x1_s,
                                   0,  0,  0, x1, y1,  1, -y1_s * x1, -y1_s * y1, -y1_s,
                                   x2, y2, 1, 0,   0,  0, -x2_s * x2, -x2_s * y2, -x2_s,
                                   0,  0,  0, x2, y2,  1, -y2_s * x2, -y2_s * y2, -y2_s,
                                   x3, y3, 1, 0,   0,  0, -x3_s * x3, -x3_s * y3, -x3_s,
                                   0,  0,  0, x3, y3,  1, -y3_s * x3, -y3_s * y3, -y3_s,
                                   x4, y4, 1, 0,   0,  0, -x4_s * x4, -x4_s * y4, -x4_s,
                                   0,  0,  0, x4, y4,  1, -y4_s * x4, -y4_s * y4, -y4_s,
                                 };

    // Транспонируем и перемножаем
    Matrix matr_A(8, 9, matr_A_data);
    Matrix transp_A = transpose(matr_A);
    Matrix matr_ATA = multiply(transp_A, matr_A);

    // Кладём в real_2d_array
    real_2d_array matr, u, vt;
    matr.setcontent(9, 9, &matr_ATA.data[0]);

    // Считаем SVD
    real_1d_array w;
    bool isSucces = rmatrixsvd(matr, 9, 9, 2, 2, 2, w, u, vt);
    Q_ASSERT(isSucces);

    // так как  W - contains singular values in descending order.
    // берём последний столбец в u
    Matrix hypothesis(9, 1);
    for (int i = 0;i < hypothesis.rows;i++) {
        hypothesis.set(i, 0, u[i][u.cols()-1]);
    }
    return hypothesis;
}

int Ransac::countInliers(const Matrix &hyp, const vector<Vector> &lines, const double threshhold) {
    int inliers = 0;
    for (auto i = 0; i < lines.size(); i++) {
        // Строим матрицу
        Matrix a(3, 1);
        a.set(0, 0, lines[i].second.getInterPoint().x);
        a.set(1, 0, lines[i].second.getInterPoint().y);
        a.set(2, 0, 1);

        // Строим матрицу h 3 на 3
        Matrix h(3, 3);
        h.data = hyp.data;

        // Находим новые координаты
        Matrix result = multiply(h, a);

        // Считаем разницу
        double distance = sqrt(pow(result.at(0,0) - lines[i].first.getInterPoint().x,2) +
                               pow(result.at(1,0) - lines[i].first.getInterPoint().y,2));
        if (distance <= threshhold) {
            inliers++;
        }
    }
    return inliers;
}


/* Транспонированеие */
Matrix Ransac::transpose(const Matrix &matr) {
    Matrix result(matr.cols, matr.rows);
    for (auto i = 0; i < matr.rows; i++) {
        for (auto j = 0; j < matr.cols; j++) {
            result.set(j, i, matr.at(i, j));
        }
    }
    return result;
}

/* Перемножение */
Matrix Ransac::multiply(const Matrix &matr_1, const Matrix &matr_2) {
    Matrix result(matr_1.rows, matr_2.cols);
    for (auto i = 0; i < matr_1.rows; i++) {
        for (auto j = 0; j < matr_2.cols; j++) {
            double sum = 0;
            for (auto k = 0; k < matr_1.cols; k++) {
                sum += matr_1.at(i, k) * matr_2.at(k, j);
            }
            result.set(i, j, sum);
        }
    }
    return result;
}

/* 4 разных рандомных числа */
vector<int> Ransac::get4RandomNumbers(const int max) {
    int rand_1, rand_2, rand_3, rand_4;
    do {
        rand_1 = rand() % max;
        rand_2 = rand() % max;
        rand_3 = rand() % max;
        rand_4 = rand() % max;
    } while (rand_1 != rand_2 && rand_2!=rand_3 && rand_3 != rand_4);

    vector<int> lines_4 = {rand_1, rand_2, rand_3, rand_4};
    return lines_4;
}

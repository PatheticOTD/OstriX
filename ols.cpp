#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

const double TOL = 1e-6;           // Точность
const int MAX_ITER = 100;          // Максимум итераций
const double C = 299792.458;       // Скорость света (км/с)

// Структура для хранения координат спутника и псевдодальности
struct Satellite {
    double r, x, y, z;
};

// Функция для вычисления расстояния между точками
double distance(double x1, double y1, double z1, double x2, double y2, double z2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

// Решение системы линейных уравнений методом Гаусса
void gaussSolve(vector<vector<double>> A, vector<double>& b, vector<double>& x) {
    int n = b.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double ratio = A[j][i] / A[i][i];
            for (int k = i; k < n; ++k) {
                A[j][k] -= ratio * A[i][k];
            }
            b[j] -= ratio * b[i];
        }
    }

    for (int i = n - 1; i >= 0; --i) {
        x[i] = b[i];
        for (int j = i + 1; j < n; ++j) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }
}

// Вычисление вектора невязок F(x)
vector<double> computeF(const vector<Satellite>& sats, const vector<double>& x) {
    vector<double> F(sats.size());
    for (size_t i = 0; i < sats.size(); ++i) {
        double dist = distance(x[0], x[1], x[2], sats[i].x, sats[i].y, sats[i].z);
        F[i] = dist + C * x[3] - sats[i].r; // Учёт скорости света
    }
    return F;
}

// Вычисление матрицы Якоби H
vector<vector<double>> computeJacobian(const vector<Satellite>& sats, const vector<double>& x) {
    size_t n = sats.size();
    vector<vector<double>> H(n, vector<double>(4));
    for (size_t i = 0; i < n; ++i) {
        double dist = distance(x[0], x[1], x[2], sats[i].x, sats[i].y, sats[i].z);
        H[i][0] = (x[0] - sats[i].x) / dist; // Производная по x
        H[i][1] = (x[1] - sats[i].y) / dist; // Производная по y
        H[i][2] = (x[2] - sats[i].z) / dist; // Производная по z
        H[i][3] = C;                        // Производная по b с учётом скорости света
    }
    return H;
}

// Умножение транспонированной H на H: H^T * H
vector<vector<double>> multiplyHtH(const vector<vector<double>>& H) {
    size_t rows = H.size(), cols = H[0].size();
    vector<vector<double>> HtH(cols, vector<double>(cols, 0));
    for (size_t i = 0; i < cols; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            for (size_t k = 0; k < rows; ++k) {
                HtH[i][j] += H[k][i] * H[k][j];
            }
        }
    }
    return HtH;
}

// Умножение транспонированной H на F: H^T * F
vector<double> multiplyHtF(const vector<vector<double>>& H, const vector<double>& F) {
    size_t rows = H.size(), cols = H[0].size();
    vector<double> HtF(cols, 0);
    for (size_t i = 0; i < cols; ++i) {
        for (size_t j = 0; j < rows; ++j) {
            HtF[i] += H[j][i] * F[j];
        }
    }
    return HtF;
}

int main() {
    vector<Satellite> sats = {
        {20753, 14952, 17934, 10307},
        {20523, -2082, 11452, 22695},
        {21749, 11196, -11301, 19967},
        {19322, 9341, 9080, 21942},
        {22904, 6433, -23618, 7272}
    };

    vector<double> x = {0, 0, 0, 0}; // Начальное приближение
    cout << "Начинаем итерации метода Ньютона с учётом скорости света...\n";

    for (int iter = 0; iter < MAX_ITER; ++iter) {
        vector<double> F = computeF(sats, x);
        vector<vector<double>> H = computeJacobian(sats, x);

        vector<vector<double>> HtH = multiplyHtH(H);
        vector<double> HtF = multiplyHtF(H, F);

        vector<double> deltaX(4, 0);
        for (auto& val : HtF) val = -val;
        gaussSolve(HtH, HtF, deltaX);

        for (size_t i = 0; i < x.size(); ++i) x[i] += deltaX[i];

        double norm = 0.0;
        for (double val : deltaX) norm += val * val;
        norm = sqrt(norm);

        cout << "Итерация " << iter + 1 << ": x = ";
        for (double xi : x) cout << xi << " ";
        cout << "\n";

        if (norm < TOL) {
            cout << "Решение найдено:\n";
            break;
        }
    }

    cout << "Координаты приёмника: x = " << x[0] << ", y = " << x[1] << ", z = " << x[2] << "\n";
    cout << "Временная поправка b = " << x[3] << " секунд\n";
    cout << "Поправка в расстоянии = " << C * x[3] << " км\n";

    return 0;
}

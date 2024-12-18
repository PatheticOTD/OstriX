#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Структура для хранения псевдодальности и координат спутников
struct Satellite {
    double rho;      // Псевдодальность
    double x, y, z;  // Координаты спутника
};

// Функция для вычисления расстояния между точками
double calcDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

// Функция для решения системы линейных уравнений методом Гаусса
vector<double> solveLinearSystem(vector<vector<double>> A, vector<double> b) {
    int n = A.size();
    for (int i = 0; i < n; ++i) {
        // Нормализация текущей строки
        double diag = A[i][i];
        for (int j = 0; j < n; ++j) A[i][j] /= diag;
        b[i] /= diag;

        // Прямой ход метода Гаусса
        for (int k = i + 1; k < n; ++k) {
            double factor = A[k][i];
            for (int j = 0; j < n; ++j) A[k][j] -= factor * A[i][j];
            b[k] -= factor * b[i];
        }
    }

    // Обратный ход
    vector<double> x(n);
    for (int i = n - 1; i >= 0; --i) {
        x[i] = b[i];
        for (int j = i + 1; j < n; ++j) x[i] -= A[i][j] * x[j];
    }
    return x;
}

// Функция для решения навигационной задачи методом Ньютона
vector<double> solveNavigation(vector<Satellite> satellites, vector<double> X, double tolerance, int max_iterations) {
    const double c = 299792458.0;  // Скорость света (м/с)

    for (int iter = 0; iter < max_iterations; ++iter) {
        vector<double> delta_rho(satellites.size());
        vector<vector<double>> H(satellites.size(), vector<double>(4));

        for (size_t i = 0; i < satellites.size(); ++i) {
            double dx = X[0] - satellites[i].x;
            double dy = X[1] - satellites[i].y;
            double dz = X[2] - satellites[i].z;
            double r = calcDistance(X[0], X[1], X[2], satellites[i].x, satellites[i].y, satellites[i].z);

            delta_rho[i] = satellites[i].rho - (r + X[3]);

            // Заполняем матрицу H
            H[i][0] = dx / r;
            H[i][1] = dy / r;
            H[i][2] = dz / r;
            H[i][3] = 1.0;  // Производная по времени
        }

        // Вычисляем H^T * H и H^T * delta_rho
        vector<vector<double>> HTH(4, vector<double>(4, 0));
        vector<double> HTdR(4, 0);

        for (size_t i = 0; i < satellites.size(); ++i) {
            for (int j = 0; j < 4; ++j) {
                HTdR[j] += H[i][j] * delta_rho[i];
                for (int k = 0; k < 4; ++k) {
                    HTH[j][k] += H[i][j] * H[i][k];
                }
            }
        }

        // Решаем систему линейных уравнений: H^T * H * delta_X = H^T * delta_rho
        vector<double> delta_X = solveLinearSystem(HTH, HTdR);

        // Обновляем параметры
        for (int i = 0; i < 4; ++i) X[i] += delta_X[i];

        // Проверка на сходимость
        double norm = 0;
        for (double val : delta_X) norm += val * val;
        if (sqrt(norm) < tolerance) {
            cout << "Сходимость достигнута на итерации " << iter + 1 << endl;
            break;
        }
    }
    return X;  // Возвращаем решение
}

int main() {
    // Входные данные: координаты спутников и псевдодальности
    vector<Satellite> satellites = {
        {20753, 14952, 17934, 10307},
        {20523, -2082, 11452, 22695},
        {21749, 11196, -11301, 19967},
        {19322, 9341, 9080, 21942},
        {22904, 6433, -23618, 7272}
    };

    // Начальное приближение: (x, y, z, Δτ)
    vector<double> initial_guess = {0, 0, 0, 0};

    // Задаём параметры
    double tolerance = 1e-10;
    int max_iterations = 10000;
    // Решаем навигационную задачу
    vector<double> result = solveNavigation(satellites, initial_guess, tolerance, max_iterations);

    // Вывод результатов в main
    const double c = 299792458.0;  // Скорость света (м/с)
    cout << "Решение навигационной задачи:" << endl;
    cout << "x = " << result[0] << " м" << endl;
    cout << "y = " << result[1] << " м" << endl;
    cout << "z = " << result[2] << " м" << endl;
    cout << "Сдвиг времени Δτ = " << result[3] / c << " с" << endl;

    return 0;
}

#include <iostream>
#include <cmath>
#include <iomanip>
#include <functional>

using namespace std;
# define M_PI           3.14159265358979323846



double f(double x) {
    return sin(x);
}

// Функция для вычисления интеграла методом трапеций с динамическим количеством разбиений
double integral(function<double(double)> f, double a, double b, double tolerance, double eps) {
    int n = 2;               // Начальное число разбиений
    double prev_result = 0;   // Предыдущее значение интеграла
    double current_result = 0;

    do {
        prev_result = current_result;
        double h = (b - a) / n;  // Шаг по x
        current_result = 0.0;

        // Вычисляем сумму по трапециям с текущим количеством разбиений
        for (int i = 1; i < n; ++i) {
            current_result += f(a + i * h);
        }
        current_result += (f(a) + f(b)) / 2.0;  // Добавляем концы
        current_result *= h;

        n *= 2;  // Удваиваем число разбиений для повышения точности
    } while (abs(current_result - prev_result) < eps * abs(current_result) + tolerance);  // Проверка на сходимость

    return current_result;
}


// Аналитическое значение интеграла
double analytical_integral() {
    return 2.0;
}

int main() {
    double a = 0;         // Фиксированное значение epsilon для нижнего предела
    long double tolerance = 1e-18;       // Заданная погрешность для проверки сходимости
    double eps = 1e-18;
    cout << fixed << setprecision(8);
    
    // Вычисляем интеграл с проверкой на сходимость
    double numerical_result = integral(f, a, M_PI, tolerance, eps);
    cout << "Численное значение интеграла: " << numerical_result << endl;

    // Вывод аналитического значения
    double analytical_result = analytical_integral();
    cout << "Аналитическое значение интеграла: " << analytical_result << endl;

    return 0;
}

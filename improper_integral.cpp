#include <iostream>
#include <cmath>
#include <iomanip>
#include <functional>
# define M_PI           3.14159265358979323846
using namespace std;
// Функция, которую нужно интегрировать
double f(double x) {
    return sin(x);
}

// Функция для вычисления несобственного интеграла
double improper_integral(function<double(double)> f, double a, double b, double tolerance, double eps) {
    int n = 1;                 // Начальное количество разбиений
    double lower = 1e-8;     // Малое значение для избежания бесконечности при x -> 0
    double h, prev_result = .0, current_result = .0;

    // Начинаем с малого значения нижнего предела a = lower
    a = lower;

    do {
        prev_result = current_result;
        n *= 2;               // Увеличиваем количество разбиений
        h = (b - a) / n;      // Размер шага
        current_result = .0;

        // Считаем интеграл методом трапеций с текущим количеством разбиений
        for (int i = 1; i < n; ++i) {
            double x = a + i * h;
            current_result += f(x);
        }
        current_result = h * (f(a) / 2. + current_result + f(b) / 2.);  // Завершаем расчет

        // Уменьшаем значение lower и корректируем нижний предел
        lower *= .1;
        a = lower;

    } while (abs(current_result - prev_result) > eps * abs(prev_result) + tolerance );  // Проверка сходимости

    return current_result;
}

// Аналитическое значение интеграла для сравнения
double analytical_integral() {
    return 2.;
}

int main() {
    double a = 0;             // Нижний предел интегрирования (начальное значение)
    double b = M_PI;           // Верхний предел интегрирования
    long double tolerance = 1e-10;       // Заданная погрешность для проверки сходимости
    double eps = 1e-10;

    cout << fixed << setprecision(8);

    // Численный расчет несобственного интеграла
    double numerical_result = improper_integral(f, a, b, tolerance, eps);
    cout << "Численное значение несобственного интеграла: " << numerical_result << endl;

    // Аналитическое значение интеграла
    double analytical_result = analytical_integral();
    cout << "Аналитическое значение интеграла: " << analytical_result << endl;

    return 0;
}

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

// Функция для преобразованной функции f(x) = x^(-6/7) с заменой x = t^2
double transformed_f(double t) {
    return 2 * pow(t, -(12.0 / 7.0) + 1);
}

// Функция для вычисления интеграла методом трапеций с динамическим количеством разбиений
double integral(double a, double b, double tolerance) {
    int n = 2;               // Начальное число разбиений
    double prev_result = 0;   // Предыдущее значение интеграла
    double current_result = 0;

    do {
        prev_result = current_result;
        double h = (b - a) / n;  // Шаг по t
        current_result = 0.0;

        // Вычисляем сумму по трапециям с текущим количеством разбиений
        for (int i = 1; i < n; ++i) {
            current_result += transformed_f(a + i * h);
        }
        current_result += (transformed_f(a) + transformed_f(b)) / 2.0;  // Добавляем концы
        current_result *= h;

        n *= 2;  // Удваиваем число разбиений для повышения точности
    } while (fabs(current_result - prev_result) > tolerance);  // Проверка на сходимость

    return current_result;
}

// Аналитическое значение интеграла
double analytical_integral() {
    return 7.0;
}

int main() {
    double epsilon = 1e-12;         // Фиксированное значение epsilon для нижнего предела
    double tolerance = 1e-7;       // Заданная погрешность для проверки сходимости
    double a = sqrt(epsilon); // Начальное значение для переменной t

    cout << fixed << setprecision(8);
    
    // Вычисляем интеграл с проверкой на сходимость
    double numerical_result = integral(a, 1.0, tolerance);
    cout << "Численное значение интеграла: " << numerical_result << endl;

    // Вывод аналитического значения
    double analytical_result = analytical_integral();
    cout << "Аналитическое значение интеграла: " << analytical_result << endl;

    return 0;
}

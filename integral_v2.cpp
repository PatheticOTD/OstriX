#include <iostream>
#include <cmath>
#include <iomanip>

// Функция, которую нужно интегрировать
double f(double x) {
    return std::pow(x, -6.0 / 7.0);
}

// Реализация метода Симпсона для интегрирования
double simpson_integral(double a, double b, int n) {
    // Проверяем, что n чётное
    if (n % 2 != 0) {
        std::cerr << "Число разбиений должно быть чётным для метода Симпсона." << std::endl;
        return 0;
    }

    double h = (b - a) / n;   // Размер шага
    double I = f(a) + f(b);   // Начальные значения f(x0) и f(xn)

    // Суммируем значения с коэффициентами
    for (int i = 1; i < n; ++i) {
        double x = a + i * h;
        if (i % 2 == 0) {
            I += 2 * f(x);  // Чётные индексы
        } else {
            I += 4 * f(x);  // Нечётные индексы
        }
    }

    I *= h / 3.0;  // Завершаем формулу Симпсона
    return I;
}

// Аналитическое значение интеграла для сравнения
double analytical_integral() {
    return 7.0;
}

int main() {
    double a = 1e-10;          // Нижний предел интегрирования (очень малое число вместо 0)
    double b = 1.0;           // Верхний предел интегрирования
    long int n = 5000000000;           // Число шагов (чётное для метода Симпсона)

    std::cout << std::fixed << std::setprecision(8);

    // Численный расчет интеграла методом Симпсона
    double numerical_result = simpson_integral(a, b, n);
    std::cout << "Численное значение интеграла (метод Симпсона): " << numerical_result << std::endl;

    // Аналитическое значение интеграла
    double analytical_result = analytical_integral();
    std::cout << "Аналитическое значение интеграла: " << analytical_result << std::endl;

    return 0;
}

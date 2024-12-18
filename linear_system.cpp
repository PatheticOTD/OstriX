#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

// Тип данных для удобства
using Matrix = vector<vector<double>>;
using Vector = vector<double>;

// Функция вывода матрицы
void displayMatrix(const Matrix &matrix, const Vector &rhs) {
    size_t size = matrix.size();
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            cout << setw(10) << matrix[i][j] << " ";
        }
        cout << "= " << setw(10) << rhs[i] << endl;
    }
    cout << endl;
}

// Прямой ход метода Гаусса с выбором главного элемента
void forwardElimination(Matrix &matrix, Vector &rhs) {
    size_t size = matrix.size();

    for (size_t pivot = 0; pivot < size; ++pivot) {
        // Поиск максимального элемента в подматрице
        size_t maxRow = pivot, maxCol = pivot;
        double maxValue = fabs(matrix[pivot][pivot]);

        for (size_t row = pivot; row < size; ++row) {
            for (size_t col = pivot; col < size; ++col) {
                if (fabs(matrix[row][col]) > maxValue) {
                    maxValue = fabs(matrix[row][col]);
                    maxRow = row;
                    maxCol = col;
                }
            }
        }

        // Перестановка строк и столбцов
        swap(matrix[pivot], matrix[maxRow]);
        swap(rhs[pivot], rhs[maxRow]);
        for (size_t row = 0; row < size; ++row) {
            swap(matrix[row][pivot], matrix[row][maxCol]);
        }

        // Нормализация строки
        for (size_t col = pivot + 1; col < size; ++col) {
            matrix[pivot][col] /= matrix[pivot][pivot];
        }
        rhs[pivot] /= matrix[pivot][pivot];
        matrix[pivot][pivot] = 1.0;

        // Обнуление элементов ниже текущего
        for (size_t row = pivot + 1; row < size; ++row) {
            double factor = matrix[row][pivot];
            for (size_t col = pivot + 1; col < size; ++col) {
                matrix[row][col] -= factor * matrix[pivot][col];
            }
            rhs[row] -= factor * rhs[pivot];
            matrix[row][pivot] = 0.0;
        }
    }
}

// Обратный ход метода Гаусса
Vector backwardSubstitution(const Matrix &matrix, const Vector &rhs) {
    size_t size = matrix.size();
    Vector solution(size);

    for (int row = size - 1; row >= 0; --row) {
        solution[row] = rhs[row];
        for (size_t col = row + 1; col < size; ++col) {
            solution[row] -= matrix[row][col] * solution[col];
        }
    }

    return solution;
}

int main() {
    size_t size = 15; // Размер системы

    // Генерация матрицы Гильберта
    Matrix matrix(size, Vector(size));
    Vector rhs(size);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            matrix[i][j] = 1.0 / (i + j + 1);
        }
        rhs[i] = static_cast<double>(i + 1); // Пример правой части
    }

    cout << "Начальная матрица:\n";
    displayMatrix(matrix, rhs);

    try {
        forwardElimination(matrix, rhs);
        cout << "Матрица после прямого хода:\n";
        displayMatrix(matrix, rhs);

        Vector solution = backwardSubstitution(matrix, rhs);
        cout << "Решение:\n";
        for (size_t i = 0; i < size; ++i) {
            cout << "x_" << i << " = " << solution[i] << endl;
        }
    } catch (const exception &e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    return 0;
}

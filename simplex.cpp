#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

// Функция для отображения таблицы
void displayMatrix(const vector<vector<double>>& matrix) {
    for (const auto& row : matrix) {
        for (double val : row) {
            cout << setw(10) << val << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Поиск ведущего столбца
int findEnteringColumn(const vector<vector<double>>& matrix) {
    int colIndex = -1;
    double minVal = 0;
    for (size_t j = 0; j < matrix[0].size() - 1; ++j) {
        if (matrix.back()[j] < minVal) {
            minVal = matrix.back()[j];
            colIndex = j;
        }
    }
    return colIndex;
}

// Поиск ведущей строки
int findLeavingRow(const vector<vector<double>>& matrix, int colIndex) {
    int rowIndex = -1;
    double minRatio = numeric_limits<double>::max();
    for (size_t i = 0; i < matrix.size() - 1; ++i) {
        double rhsValue = matrix[i].back();
        double coeff = matrix[i][colIndex];
        if (coeff > 0) {
            double ratio = rhsValue / coeff;
            if (ratio < minRatio) {
                minRatio = ratio;
                rowIndex = i;
            }
        }
    }
    return rowIndex;
}

// Выполнение шага приведения таблицы
void pivotOperation(vector<vector<double>>& matrix, int rowIndex, int colIndex) {
    double pivotElement = matrix[rowIndex][colIndex];
    for (size_t j = 0; j < matrix[0].size(); ++j) {
        matrix[rowIndex][j] /= pivotElement;
    }
    for (size_t i = 0; i < matrix.size(); ++i) {
        if (i != rowIndex) {
            double multiplier = matrix[i][colIndex];
            for (size_t j = 0; j < matrix[0].size(); ++j) {
                matrix[i][j] -= multiplier * matrix[rowIndex][j];
            }
        }
    }
}

// Основной метод симплекс-алгоритма
int simplexAlgorithm(vector<vector<double>>& matrix) {
    while (true) {
        int enteringCol = findEnteringColumn(matrix);
        if (enteringCol == -1) break; // Найдено оптимальное решение
        int leavingRow = findLeavingRow(matrix, enteringCol);
        if (leavingRow == -1) {
            break;
        }
        pivotOperation(matrix, leavingRow, enteringCol);
        displayMatrix(matrix); // Отображение промежуточных шагов для отладки
    }

    return matrix.back().back();
}

int main() {
    // Исходная симплекс-таблица для задачи минимизации
    vector<vector<double>> matrix = {
        {-5, 4, 1, 0, 0, 20}, // Ограничение 1
        {2, 3, 0, 1, 0, 24}, // Ограничение 2
        {1,-3, 0, 0, 1, 3},
        {-3, -2,0,0, 0, 0} // Целевая функция (переписана для максимизации)
    };

    cout << "Начальная таблица:" << endl;
    displayMatrix(matrix);

    double ans = simplexAlgorithm(matrix);
    cout <<"Решение: "<<ans<<endl;

    return 0;
}
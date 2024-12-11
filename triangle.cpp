#include <iostream>
#include <cmath>
#include <stdexcept>

using namespace std;

class Triangle {
private:
    double vertex1_x, vertex1_y; // Координаты
    double vertex2_x, vertex2_y;
    double vertex3_x, vertex3_y;

    // Функция для вычисления расстояния между двумя точками
    double calculateDistance(double point1_x, double point1_y, double point2_x, double point2_y) const {
        return sqrt(pow(point2_x - point1_x, 2) + pow(point1_y - point2_y, 2));
    }

    // Проверка на валидность треугольника (сумма двух сторон больше третьей)
    bool isValidTriangle(double side1, double side2, double side3) const {
        return (side1 + side2 > side3) && (side1 + side3 > side2) && (side2 + side3 > side1);
    }

public:
    // Конструктор, принимающий координаты трёх вершин
    Triangle(double coord1_x, double coord1_y, double coord2_x, double coord2_y, double coord3_x, double coord3_y)
            : vertex1_x(coord1_x), vertex1_y(coord1_y),
              vertex2_x(coord2_x), vertex2_y(coord2_y),
              vertex3_x(coord3_x), vertex3_y(coord3_y) {

        double side1 = calculateDistance(vertex1_x, vertex1_y, vertex2_x, vertex2_y);
        double side2 = calculateDistance(vertex2_x, vertex2_y, vertex3_x, vertex3_y);
        double side3 = calculateDistance(vertex3_x, vertex3_y, vertex1_x, vertex1_y);

        if (calculateArea() == 0) {
            throw invalid_argument("Вершины треугольника не должны быть коллинеарными.");
        }

        if (!isValidTriangle(side1, side2, side3)) {
            throw invalid_argument("Стороны треугольника не удовлетворяют неравенству треугольника.");
        }
    }

    // Метод для вычисления периметра
    double calculatePerimeter() const {
        double side1 = calculateDistance(vertex1_x, vertex1_y, vertex2_x, vertex2_y);
        double side2 = calculateDistance(vertex2_x, vertex2_y, vertex3_x, vertex3_y);
        double side3 = calculateDistance(vertex3_x, vertex3_y, vertex1_x, vertex1_y);
        return side1 + side2 + side3;
    }

    // Метод для вычисления площади с использованием формулы Герона
    double calculateArea() const {
        double side1 = calculateDistance(vertex1_x, vertex1_y, vertex2_x, vertex2_y);
        double side2 = calculateDistance(vertex2_x, vertex2_y, vertex3_x, vertex3_y);
        double side3 = calculateDistance(vertex3_x, vertex3_y, vertex1_x, vertex1_y);
        double semiPerimeter = (side1 + side2 + side3) / 2; // Полупериметр
        return sqrt(semiPerimeter * (semiPerimeter - side1) * (semiPerimeter - side2) * (semiPerimeter - side3));
    }

    // Метод для отображения свойств треугольника
    void displayProperties() const {
        cout << "Вершины треугольника: (" << vertex1_x << ", " << vertex1_y << "), ("
             << vertex2_x << ", " << vertex2_y << "), (" << vertex3_x << ", " << vertex3_y << ")\n";
        cout << "Периметр: " << calculatePerimeter() << "\n";
        cout << "Площадь: " << calculateArea() << "\n";
    }
};

int main() {
    try {
        // Пример использования
        Triangle triangle(0, 0, 1, 1, 4, 4); // Этот пример вызовет исключение
        triangle.displayProperties();
    } catch (const invalid_argument &e) {
        cerr << "Ошибка: " << e.what() << "\n";
    }

    return 0;
}

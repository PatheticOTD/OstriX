#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <cstdlib>

// Класс Circle
class Circle {
public:
    Circle(double x, double y, double radius)
        : x_(x), y_(y), radius_(radius) {}

    // Метод проверки точки на вхождение
    bool contains(double px, double py) const {
        double distSquared = (px - x_) * (px - x_) + (py - y_) * (py - y_);
        return distSquared <= radius_ * radius_;
    }

private:
    double x_, y_, radius_;
};

// Функция генерации точек
std::vector<std::pair<double, double>> generatePoints(int numPoints, double minX, double maxX, double minY, double maxY) {
    std::vector<std::pair<double, double>> points;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disX(minX, maxX);
    std::uniform_real_distribution<> disY(minY, maxY);

    for (int i = 0; i < numPoints; ++i) {
        points.emplace_back(disX(gen), disY(gen));
    }
    return points;
}

// Функция создания Gnuplot-скрипта
void createPlotScript(const Circle& circle, const std::vector<std::pair<double, double>>& points,
     double circleX, double circleY, double circleRadius) {
    std::ofstream plotScript("plot.gnuplot");
    std::ofstream inCircle("in_circle.dat");
    std::ofstream outCircle("out_circle.dat");

    // Сохранение точек в файлы
    for (const auto& point : points) {
        if (circle.contains(point.first, point.second)) {
            inCircle << point.first << " " << point.second << "\n";
        } else {
            outCircle << point.first << " " << point.second << "\n";
        }
    }

    // Настройка Gnuplot-скрипта
    plotScript << "set title 'Points and Circle'\n";
    plotScript << "set size ratio -1\n";
    plotScript << "set xrange [-10:10]\n";
    plotScript << "set yrange [-10:10]\n";

    // Построение окружности
    plotScript << "set object 1 circle at " << circleX << "," << circleY
               << " size " << circleRadius << " front fillstyle empty border lc rgb 'blue'\n";

    // Построение точек
    plotScript << "plot 'in_circle.dat' with points pt 7 lc rgb 'green' title 'Inside Circle', \\\n";
    plotScript << "     'out_circle.dat' with points pt 7 lc rgb 'red' title 'Outside Circle'\n";

    plotScript.close();
    inCircle.close();
    outCircle.close();
}



// Функция автоматического запуска Gnuplot
void runGnuplot() {
    int retCode = std::system("gnuplot -persist plot.gnuplot");
    if (retCode != 0) {
        std::cerr << "Ошибка: не удалось запустить Gnuplot. Убедитесь, что Gnuplot установлен и доступен в PATH.\n";
    }
}

int main() {
    // Параметры окружности
    double circleX = 0.0, circleY = 0.0, circleRadius = 8.0;
    Circle circle(circleX, circleY, circleRadius);

    // Генерация точек
    int numPoints = 10000;
    double minX = -10.0, maxX = 10.0, minY = -10.0, maxY = 10.0;
    auto points = generatePoints(numPoints, minX, maxX, minY, maxY);

    // Создание Gnuplot-скрипта и данных
    createPlotScript(circle, points, circleX = 0.0, circleY = 0.0, circleRadius = 8.0);

    // Автоматический запуск Gnuplot
    runGnuplot();

    return 0;
}

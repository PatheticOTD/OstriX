#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <vector>
#include <functional>
#include <cstdlib>

using namespace std;

double monteCarloIntegral(double xMin, double xMax, double yMin, double yMax, int pointCount, 
                          function<double(double)> graphFunc, 
                          vector<pair<double, double>>& underCurvePoints, 
                          vector<pair<double, double>>& aboveCurvePoints) {
    random_device rd;
    mt19937 generator(rd());
    uniform_real_distribution<> xDistribution(xMin, xMax);
    uniform_real_distribution<> yDistribution(yMin, yMax);

    int underCurveCount = 0;

    // Генерация точек
    for (int i = 0; i < pointCount; ++i) {
        double x = xDistribution(generator);
        double y = yDistribution(generator);

        if (y <= graphFunc(x)) {
            underCurveCount++;
            underCurvePoints.emplace_back(x, y);
        } else {
            aboveCurvePoints.emplace_back(x, y);
        }
    }

    // Расчёт площади под графиком
    double rectangleArea = (xMax - xMin) * (yMax - yMin);
    return rectangleArea * static_cast<double>(underCurveCount) / pointCount;
}

// Функция для генерации Gnuplot-скрипта
void generatePlotScript(const string& scriptName, double xMin, double xMax, double yMin, double yMax, 
                        const vector<pair<double, double>>& underCurvePoints, 
                        
                        const vector<pair<double, double>>& aboveCurvePoints) {
    ofstream scriptFile(scriptName);
    ofstream underCurveFile("under_curve_points.dat");
    ofstream aboveCurveFile("above_curve_points.dat");

    // Сохранение точек под графиком
    for (const auto& point : underCurvePoints) {
        underCurveFile << point.first << " " << point.second << "\n";
    }
    underCurveFile.close();

    // Сохранение точек над графиком
    for (const auto& point : aboveCurvePoints) {
        aboveCurveFile << point.first << " " << point.second << "\n";
    }
    aboveCurveFile.close();

    // Создание скрипта Gnuplot
    scriptFile << "set title 'Random Area Estimation'\n";
    scriptFile << "set xrange [" << xMin << ":" << xMax << "]\n";
    scriptFile << "set yrange [" << yMin << ":" << yMax << "]\n";
    scriptFile << "set size ratio -1\n";
    scriptFile << "set xlabel 'X axis'\n";
    scriptFile << "set ylabel 'Y axis'\n";
    scriptFile << "plot 'under_curve_points.dat' using 1:2 with points pt 7 lc rgb 'green' title '', \\\n";
    scriptFile << "     'above_curve_points.dat' using 1:2 with points pt 7 lc rgb 'red' title '', \\\n";
    scriptFile << "     x**0.333 with lines lw 2 lc rgb 'blue'\n";
    scriptFile.close();
}

// Функция для запуска Gnuplot
void executeGnuplot(const string& scriptName) {
    string command = "gnuplot -persist " + scriptName;
    int resultCode = system(command.c_str());
    if (resultCode != 0) {
        cerr << "Ошибка: не удалось запустить Gnuplot. Убедитесь, что Gnuplot установлен.\n";
    }
}

int main() {
    double xMin = 0.0, xMax = 1.0, yMin = 0.0, yMax = 1.0;
    int totalPoints = 100000;

    // Определение функций
    auto graphFunc = [](double x) -> double { return pow(x, 1.0 / 3.0); };
    auto exactSolution = []() -> double { return 3.0 / 4.0; };

    vector<pair<double, double>> underCurvePoints;
    vector<pair<double, double>> aboveCurvePoints;

    // Вычисление интеграла методом Монте-Карло
    double estimatedArea = monteCarloIntegral(xMin, xMax, yMin, yMax, totalPoints, graphFunc, underCurvePoints, aboveCurvePoints);
    double exactArea = exactSolution();
    double errorPercentage = fabs((estimatedArea - exactArea) / exactArea) * 100.0;

    cout << "Оценённая площадь под графиком: " << estimatedArea << "\n";
    cout << "Аналитическая площадь под графиком: " << exactArea << "\n";
    cout << "Относительная ошибка: " << errorPercentage << "%\n";

    // Генерация графика
    generatePlotScript("plot_graph.gnuplot", xMin, xMax, yMin, yMax, underCurvePoints, aboveCurvePoints);
    executeGnuplot("plot_graph.gnuplot");

    return 0;
}

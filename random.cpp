#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <cstdlib>
#include <functional>
//#include <vector>
//#include <system.> 
using namespace std;

void writePointsToFile(const string &filename, const vector<pair<double, double>> &points)
{
    ofstream outFile(filename);
    if (!outFile)
    {
        cerr << "Ошибка записи в файл: " << filename << endl;
        return;
    }

    for (const auto &[x, y] : points)
    {
        outFile << x << " " << y << "\n";
    }
    outFile.close();
    cout << "Данные сохранены в файл: " << filename << "\n";
}

// Генерация равномерного распределения
vector<pair<double, double>> generate_uniform(int num_points)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    vector<pair<double, double>> points;
    for (int i = 0; i < num_points; ++i)
    {
        points.emplace_back(dis(gen), dis(gen));
    }
    return points;
}

// A
vector<pair<double, double>> generate_norm_dist_A(int num_points, double mean, double stddev)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    vector<pair<double, double>> points;
    for (int i = 0; i < num_points / 2; ++i)
    {
        double u1 = dis(gen);
        double u2 = dis(gen);

        double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        double z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);

        points.emplace_back(dis(gen), mean + z0 * stddev);
        points.emplace_back(dis(gen), mean + z1 * stddev);
    }
    return points;
}

// D
vector<pair<double, double>> generate_norm_dist_B(int num_points, double mean, double stddev)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(-1.0, 1.0);

    vector<pair<double, double>> points;
    int count = 0;
    while (count < num_points)
    {
        double u1 = dist(gen);
        double u2 = dist(gen); 
        double s = u1 * u1 + u2 * u2;

        if (s > 0 && s < 1)
        {
            double z0 = u1 * sqrt(-2.0 * log(s) / s);
            double z1 = u2 * sqrt(-2.0 * log(s) / s);

            points.emplace_back(dist(gen), mean + z0 * stddev);
            points.emplace_back(dist(gen), mean + z1 * stddev);
            count += 2;
        }
    }
    return points;
}

// Создание скрипта для gnuplot
void createGnuplotScript(const string &scriptFile,
                         const vector<string> &dataFiles,
                         const vector<string> &titles,
                         const vector<string> &colors,
                         const vector<int> &pointTypes)
{
    ofstream script(scriptFile);
    if (!script)
    {
        cerr << "Ошибка создания скрипта: " << scriptFile << endl;
        return;
    }

    script << "set terminal wxt size 1800,600\n";
    script << "set multiplot layout 1," << dataFiles.size() << " title 'Распределения точек'\n";
    script << "set pointsize 0.5\n";
    script << "set style data points\n";

    for (size_t i = 0; i < dataFiles.size(); ++i)
    {
        script << "set title '" << titles[i] << "'\n";
        script << "plot '" << dataFiles[i] << "' using 1:2 with points pt " << pointTypes[i]
               << " lc '" << colors[i] << "' notitle\n";
    }

    script << "unset multiplot\n";
    script.close();
}

int main() {
    int num_points = 10000;

    // Генерация и запись данных
    auto uniformPoints = generate_uniform(num_points);
    writePointsToFile("uniform_points.txt", uniformPoints);

    auto normalPointsA = generate_norm_dist_A(num_points, 0.0, 1.0);
    writePointsToFile("normal_points_a.txt", normalPointsA);

    auto normalPointsB = generate_norm_dist_B(num_points, -1.0, 1.0);
    writePointsToFile("normal_points_b.txt", normalPointsB);

    // Создание и запуск gnuplot
    vector<string> dataFiles = {"uniform_points.txt", "normal_points_a.txt", "normal_points_b.txt"};
    vector<string> titles = {"Равномерное распределение", "Нормальное распределение (A)", "Нормальное распределение (B)"};
    vector<string> colors = {"green", "blue", "orange"};
    vector<int> pointTypes = {7, 7, 7};

    createGnuplotScript("plot_script.gp", dataFiles, titles, colors, pointTypes);

    cout << "Запуск графиков через gnuplot...\n";
    system("gnuplot -persist plot_script.gp");

    return 0;
}

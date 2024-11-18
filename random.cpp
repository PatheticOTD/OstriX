#include <iostream>
#include <fstream>
#include <random>

using namespace std;

void generate_uniform_points(const string& filename, int num_points) {
    ofstream file(filename);
    if (!file) {
        cerr << "Ошибка открытия файла: " << filename << endl;
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);

    file << "x" << "," << "y" << "\n";
    for (int i = 0; i < num_points; ++i) {
        double x = dist(gen);
        double y = dist(gen);
        file << x << "," << y << "\n";
    }

    file.close();
}
void generate_normal_points(const string& filename, int num_points, double mean, double stddev) {
    ofstream file(filename);
    if (!file) {
        cerr << "Ошибка открытия файла: " << filename << endl;
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);
    file << "x" << "," << "y" << "\n";

    for (int i = 0; i < num_points / 2; ++i) {
        double u1 = dist(gen);
        double u2 = dist(gen);
        double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        double z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);

        file << z0 * stddev + mean << ",";
        file << z1 * stddev + mean << "\n";
    }

    file.close();
}
int main() {
    const int num_points = 10000;

    // Генерация равномерного распределения
    generate_uniform_points("uniform_points.csv", num_points);

    // Генерация нормального распределения
    generate_normal_points("normal_points.csv", num_points, 0.0, 1.0);

    

    return 0;
}

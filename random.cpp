#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <cstdlib>

using namespace std;

double normal_distribution_func(double x, double mu, double sigma) {
    return (1.0 / (sigma * sqrt(2 * M_PI))) * exp(-0.5 * pow((x - mu) / sigma, 2));
}
void generate_norm_plot(const string& filename, int num_points, double mean, double stddev, double min_x, double max_x){
    ofstream file(filename);
    if (!file) {
        cerr << "Ошибка открытия файла: " << filename << endl;
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(min_x, max_x);

    for (int i = 0; i < num_points; ++i) {
        double x = dist(gen);
        double y = dist(gen);
        file << normal_distribution_func(x, mean, stddev) << " " << x << "\n";
    }

    file.close();
}

void generate_uniform_points(const string& filename, int num_points) {
    ofstream file(filename);
    if (!file) {
        cerr << "Ошибка открытия файла: " << filename << endl;
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(0.0, 1.0);

    for (int i = 0; i < num_points; ++i) {
        double x = dist(gen);
        double y = dist(gen);
        file << x << " " << y << "\n";
    }

    file.close();
}
void generate_normal_points_a(const string& filename, int num_points, double mean, double stddev) {
    ofstream file(filename);
    if (!file) {
        cerr << "Ошибка открытия файла: " << filename << endl;
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(.0, 1.);
    for (int i = 0; i < num_points / 2; ++i) {
        double u1 = dist(gen);
        double u2 = dist(gen);
        double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        double z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);

        file <<dist(gen) << ' '<< z0 * stddev + mean<<'\n'
        <<dist(gen)<< ' '<<z1 * stddev + mean<<'\n';
    }

    file.close();
}

void generate_normal_points_b(const string& filename, int num_points, double mean, double stddev) {
    ofstream file(filename);
    if (!file) {
        cerr << "Ошибка открытия файла: " << filename << endl;
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(-1.0, 1.0);
    for (int i = 0; i < num_points / 2; ++i) {
        double u1 = dist(gen);
        double u2 = dist(gen);
        double s = u1*u1 + u2*u2;
        double z0 = u1 * sqrt(-2.0 * log(s) / s);
        double z1 = u2 * sqrt(-2.0 * log(s) / s);

        file <<dist(gen) << ' '<< z0 * stddev + mean<<'\n'
        <<dist(gen)<< ' '<<z1 * stddev + mean<<'\n';
    }
    file.close();
}

void gnuplot_script()
{
    ofstream script("plot_all.gp");

    script << "set term wxt size 1800,600\n"; //  making window
    script << "set multiplot layout 1,4 title 'Distributions'\n"; // making row of 3 plots

    script << "set title 'Uniform Distribution'\n";
    script << "plot 'uniform_points.txt' using 1:2 with points pointtype 7 pointsize 0.5 linecolor rgb 'red'\n";

    script << "set title 'Normal Distribution A'\n";
    script << "plot 'normal_points_a.txt' using 1:2 with points pointtype 7 pointsize 0.5 linecolor rgb 'blue'\n";

    script << "set title 'Normal Distribution B'\n";
    script << "plot 'normal_points_b.txt' using 1:2 with points pointtype 7 pointsize 0.5\n";

    script << "set title 'Normal Distribution plot'\n";
    script << "plot 'normal_plot.txt' using 1:2 with points pointtype 7 pointsize 0.5 linecolor rgb 'blue'\n";
    // Завершаем multiplot
    script << "unset multiplot\n";

    script.close();
}

int main() {
    const int num_points = 10000;


    generate_uniform_points("uniform_points.txt", num_points);

    generate_normal_points_a("normal_points_a.txt", num_points, .0, 1.5);
    generate_normal_points_b("normal_points_b.txt", num_points, 0, 1.5);
    generate_norm_plot("normal_plot.txt", num_points, .0, 1.5, -6., 6.);

    gnuplot_script();
    system("gnuplot -persist plot_all.gp");
    return 0;
}

// CMakeProject1.cpp: определяет точку входа для приложения.
//

#include "CMakeProject1.h"
#include <tabulate/table.hpp>

#include <iostream>
#include <iomanip>
#include <format>
#include <vector>
#include <functional>
#include <string>

using namespace std;
using namespace tabulate;

//TODO: сверить с точным решением

//Структура для хранения результата
struct SolutionResult {
    vector<double> x;
    vector<double> A;
    vector<double> B;
    vector<double> C;
    vector<double> D;
    vector<double> m;
    vector<double> k;
    vector<double> y;
};

//Реализация алгоритма прогонки
SolutionResult Solution(function<double(double)> p,
    function<double(double)> q,
    function<double(double)> r,
    function<double(double)> f,
    double a, double alpha_0, double alpha_1, double Alpha,
    double b, double beta_0, double beta_1, double Beta,
    size_t n)
{
    if (abs(alpha_0) + abs(alpha_1) + abs(beta_0) + abs(beta_1) == 0)
    {
        throw runtime_error("Некорректные граничные условия");
    }

    double h = (b - a) / n;

    vector<double> x(n + 1);
    vector<double> A(n + 1);
    vector<double> B(n + 1);
    vector<double> C(n + 1);
    vector<double> D(n + 1);
    vector<double> m(n + 1);
    vector<double> k(n + 1);
    vector<double> y(n + 1);

    x[0] = a;
    A[0] = nan("");
    B[0] = h * alpha_0 - alpha_1;
    C[0] = alpha_1;
    D[0] = h * Alpha;
    for (int i = 1; i < n; i++)
    {
        double x_i = a + i * h;
        x[i] = x_i;

        A[i] = p(x_i) - h * q(x_i) / 2;
        B[i] = -2 * p(x_i) + h * h * r(x_i);
        C[i] = p(x_i) + h * q(x_i) / 2;
        D[i] = h * h * f(x_i);
    }
    x[n] = b;
    A[n] = -beta_1;
    B[n] = h * beta_0 + beta_1;
    C[n] = nan("");
    D[n] = h * Beta;

    m[0] = 0;
    k[0] = 0;
    m[1] = -C[0] / B[0];
    k[1] = D[0] / B[0];
    for (int i = 1; i < n; i++)
    {
        m[i + 1] = -C[i] / (B[i] + A[i] * m[i]);
        k[i + 1] = (D[i] - A[i] * k[i]) / (B[i] + A[i] * m[i]);
    }

    y[n] = (D[n] - A[n] * k[n]) / (B[n] + A[n] * m[n]);
    for (int i = n - 1; i >= 0; i--)
    {
        y[i] = m[i + 1] * y[i + 1] + k[i + 1];
    }

    return { x, A, B, C, D, m, k, y };
}

//Точное решение
double exact_y(double x) {
    const double sqrt3 = sqrt(3);
    const double sin_sqrt3 = sin(sqrt3);
    double y = (sin(sqrt3 * x) / (3 * sin_sqrt3)) - (x / 3);
    return y;
}

//ВЫвод результата в консоль
void PrintResult(SolutionResult result)
{
    Table table;

    // Заголовки
    //table.add_row({ "№", "x", "A", "B", "C", "D", "m", "k", "y" });
    table.add_row({ "№", "x", "A", "B", "C", "D", "m", "k", "y", "Error Rate"});

    // Данные
    for (size_t i = 0; i < result.A.size(); ++i) {

        std::stringstream y_stream;
        y_stream << std::fixed << std::setprecision(12) << result.y[i];

        std::stringstream error_rate_stream;
        error_rate_stream << std::scientific << abs(result.y[i] - exact_y(result.x[i]));

        table.add_row({
            to_string(i), // Индекс
            to_string(result.x[i]), //x
            to_string(result.A[i]), // A
            to_string(result.B[i]), // B
            to_string(result.C[i]), // C
            to_string(result.D[i]), // D
            to_string(result.m[i]), // m
            to_string(result.k[i]), // k
            y_stream.str(),  // y
            error_rate_stream.str()
            });
    }

    // Настройка форматирования
    table.format()
        .font_style({ FontStyle::bold })
        .border_top("-")
        .border_bottom("-")
        .border_left("|")
        .border_right("|")
        .corner("+");

    // Вывод таблицы
    cout << table << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    system("chcp 1251");

    //Вывод условий задачи
    cout << "Задание 1. Решение ОДУ методом прогонки" << endl;
    function<double(double)> p = [](double) { return 1.0; };
    function<double(double)> q = [](double) { return 0.0; };
    function<double(double)> r = [](double) { return 3.0; };
    function<double(double)> f = [](double x) { return -x; };
    cout << "Краевое уравнение " << "y'' = -3y - x" << endl;
    double a = 0;
    double alpha_0 = 1;
    double alpha_1 = 0;
    double Alpha = 0;
    double b = 1;
    double beta_0 = 1;
    double beta_1 = 0;
    double Beta = 0;
    cout << format("Граничные условия y({}) = {}  y({}) = {}", a, Alpha, b, Beta) << endl;

    cout << endl;

    //Получение входных данных
    size_t n;
    string answer_n = "";
    do
    {
        cout << "Введите значение n: ";
        cin >> n;

        //Решение задачи
        SolutionResult result = Solution(p, q, r, f, a, alpha_0, alpha_1, Alpha, b, beta_0, beta_1, Beta, n);
        //Вывод результата
        PrintResult(result);

        setlocale(LC_ALL, "Russian");
        cout << "Хотите ввести другое значение n? Y/N" << endl;
        cin >> answer_n;
        cout << endl;
    } while (answer_n != "N");

	return 0;
}

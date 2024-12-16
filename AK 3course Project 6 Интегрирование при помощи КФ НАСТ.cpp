#include <iostream>
#include <vector>
#include <functional>
#include "functions.h"
#include "Solution_4_1.h"
#include <chrono>

using namespace std;

vector<pair<double, double>> separate_roots(function<double(double)> f, double A, double B, double H)
{
    vector<pair<double, double>> root_intervals = vector<pair<double, double>>();
    double X1, X2, Y1, Y2;
    X1 = A;
    X2 = A + H;
    Y1 = f(X1);
    while (X2 <= B)
    {
        Y2 = f(X2);
        if (Y1 * Y2 <= 0)
        {
            root_intervals.push_back({ X1, X2 });
        }
        X1 = X2;
        X2 = X1 + H;
        Y1 = Y2;
    }
    return root_intervals;
}

//Метод бисекции
double method_bisection(function<double(double)> f, double a, double b, double e)
{
    double c;
    while (b - a > 2 * e)
    {
        c = (a + b) / 2;
        if (f(a) * f(c) <= 0)
        {
            b = c;
        }
        else {
            a = c;
        }
    }
    return (a + b) / 2;
}

//Метод секущих
double method_secant(function<double(double)> f, double a, double b, double e)
{
    double x, x_prev, x_preprev;
    x = a + (b - a) / 3;
    x_prev = b - (b - a) / 3;
    while (abs(x - x_prev) >= e)
    {
        x_preprev = x_prev;
        x_prev = x;
        x = x_prev - f(x_prev) / (f(x_prev) - f(x_preprev)) * (x_prev - x_preprev);
    }
    return x;
}

void Check_accuracy(function<double(double)> rho, double a, double b, vector<double> nodes, vector<double> A, function<double(double)> g, string print_g)
{
    cout << "ПРОВЕРКА: " << "Вычисляем значение ИКФ на многочлене " << print_g << endl;
    auto integrand = [g, rho](double x) { return g(x) * rho(x); };
    double test_exact_integral = Exact_Integral(integrand, a, b);
    cout << "   Истинное значение интеграла:  " << fixed << setprecision(16) << test_exact_integral << endl;

    unsigned n = nodes.size();
    double test_integral = 0;
    for (int k = 0; k < n; k++)
    {
        test_integral += A[k] * g(nodes[k]);
    }
    cout << "   Найденное значение интеграла: " << fixed << setprecision(16) << test_integral << endl;
    cout << "   Абсолютная погрешность: " << scientific << abs(test_integral - test_exact_integral) << endl;
    cout << "   Относительная погрешность: " << scientific << abs(test_integral - test_exact_integral) / abs(test_exact_integral) << endl;
    cout << endl;
}

void print_vector_w(vector<double> vec, string name)
{
    int n = vec.size();
    int precision = 12;
    int width = 22;
    int width0 = 6;
    vector<string> numbers_in_string = vector<string>(vec.size());
    for (int i = 0; i < vec.size(); i++)
    {
        numbers_in_string[i] = double_to_string(vec[i], precision);
        if (numbers_in_string[i].size() >= width)
        {
            width = numbers_in_string[i].size() + 2;
        }
    }
    int w1 = (width - name.length()) / 2;
    int w2 = width - name.length() - w1;
    cout << "+" << string(width0, '-') << "+" << string(width, '-') << "+" << endl;
    cout << "|" << string(width0, ' ') << "|" << string(w1, ' ') << name << string(w2, ' ') << "|" << endl;
    cout << "+" << string(width0, '-') << "+" << string(width, '-') << "+" << endl;
    for (int i = 0; i < vec.size(); i++)
    {
        //cout << "Число: " << vec[i] << endl;
        int n0 = width0 - 3 - (i / 10 + 1);
        int n1 = (width - numbers_in_string[i].size()) / 2;
        int n2 = width - numbers_in_string[i].size() - n1;
        cout << "|" << " x^" << i << string(n0, ' ') << "|" << string(n1, ' ') << numbers_in_string[i] << string(n2, ' ') << "|" << endl;
    }
    int n0 = width0 - 3 - (n / 10 + 1);
    int n1 = (width - 1) / 2;
    int n2 = width - 1 - n1;
    cout << "|" << " x^" << n << string(n0, ' ') << "|" << string(n1, ' ') << "1" << string(n2, ' ') << "|" << endl;
    cout << "+" << string(width0, '-') << "+" << string(width, '-') << "+" << endl;
}

void Solution(function<double(double)> f, 
              function<string()> print_f, 
              function<double(double)> rho, 
              function<string()> print_rho)
{
    cout << "ЭТАП ВВОДА ДАНЫХ" << endl;
    cout << endl;

    cout << "Исследуемая функция f(x) = " << print_f() << endl;
    cout << "Весовая функция ro(x) = " << print_rho() << endl;
    cout << endl;

    string answer_a_b = "";
    do
    {
        double a, b;
        cout << "Введите пределы интегрирования a и b: " << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> a >> b;
        cout << endl;

        string answer_n = "";

        do
        {
            unsigned n;
            cout << "Введите количество узлов n:" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> n;
            cout << endl;

            auto integrand = [f, rho](double x)
                {
                    return rho(x) * f(x);
                };
            double exact_integral = Exact_Integral(integrand, a, b);
            cout << "Истинное значение интеграла:  " << fixed << setprecision(12) << exact_integral << endl << endl << endl;

            cout << "ЭТАП 1. Вычисление коэффициентов ИКФ для равноотстоящих узлов" << endl;
            double h = (b - a) / n;
            vector<double> nodes_equidistant = vector<double>(n);
            for (int i = 0; i < n; i++)
            {
                nodes_equidistant[i] = a + h * i;
            }
            vector<double> A_equidistant = Solution_4_1(f, rho, a, b, nodes_equidistant);

            auto degree_n_1 = [n](double x) -> double { return pow(x, n - 1); };
            Check_accuracy(rho, a, b, nodes_equidistant, A_equidistant, degree_n_1, "g(x) = x^(n-1)");
            cout << endl << endl;


            cout << "ЭТАП 2. Вычисление интеграла при помощи КФ НАСТ" << endl;

            //Строим матрицу из мю                                                     ВЫВЕСТИ
            Eigen::MatrixXd matrix_mu(n, n);
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    matrix_mu(i, j) = mu(rho, i + j, a, b);
                }
            }
            //Строим вектор из мю                                                      ВЫВЕСТИ
            Eigen::VectorXd vector_mu = Eigen::VectorXd(n);
            for (int i = 0; i < n; i++)
            {
                vector_mu(i) = -1 * mu(rho, n + i, a, b);
            }
            //Решаем систему
            Eigen::VectorXd result_a = matrix_mu.colPivHouseholderQr().solve(vector_mu);
            vector<double> vector_a = vector<double>(result_a.data(), result_a.data() + result_a.size());

            cout << "Найденный ортогональный многочлен w(x)" << endl;
            /*for (int i = 0; i < n; i++)
            {
                cout << fixed << setprecision(8) << vector_a[i] << " * x^" << i << " + " << endl;
            }
            cout << "x^n" << endl;*/
            auto w = [vector_a](double x)
                {
                    int n = vector_a.size();
                    double result = 0.0;
                    for (int i = 0; i < n; ++i) {
                        result += vector_a[i] * pow(x, i);
                    }
                    result += pow(x, n);
                    return result;
                };
            print_vector_w(vector_a, "Коэффициенты w(x)");
            //Поиск корней ортогонального многочлена
            double h_nodes = (b - a) / (10 * n);
            vector<pair<double, double>> intervals;
            const auto time_limit = chrono::milliseconds(5000);
            auto start_time = chrono::steady_clock::now();
            do
            {
                auto current_time = chrono::steady_clock::now();
                auto elapsed_time = current_time - start_time;
                if (elapsed_time > time_limit)
                {
                    cout << "Достигнут лимит времени!" << endl << endl;
                    throw exception();
                }
                intervals = separate_roots(w, a, b, h_nodes);
                if (intervals.size() < n)
                {
                    cout << "Уменьшаем h" << endl;
                    h_nodes = h_nodes / 10;
                }
            } while (intervals.size() < n);

            double e = 1e-14;
            vector<double> nodes_nast = vector<double>(n);
            for (int i = 0; i < n; i++)
            {
                double a_i = intervals[i].first;
                double b_i = intervals[i].second;
                nodes_nast[i] = method_bisection(w, a_i, b_i, e);
            }
            vector<double> A_nast = Solution_4_1(f, rho, a, b, nodes_nast);

            auto polynomial_1 = [](double x) { return pow(x, 3) + pow(x, 2) - x - 1; };
            Check_accuracy(rho, a, b, nodes_nast, A_nast, polynomial_1, "g(x) = x^3 + x^2 - x - 1");

            auto polynomial_2 = [n](double x) { return 0.175 * pow(x, 2 * n - 1) - 2.55 * x + 1.125; };
            Check_accuracy(rho, a, b, nodes_nast, A_nast, polynomial_2, "g(x) = 0.175 * x^(n-1) - 2.55 * x + 1.125");

            cout << endl;

            cout << string(100, '-') << endl << endl;

            cout << "Хотите ввести другое число узлов n? Y/N" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> answer_n;

        } while (answer_n != "N");

        cout << "Хотите ввести другие границы отрезка a и b? Y/N" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> answer_a_b;
        cout << endl << endl;

    } while (answer_a_b != "N");
}

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "ЗАДАНИЕ 6. Приближенное вычисление интегралов при помощи КФ НАСТ" << endl << endl;

    vector<function<double(double)>> rho_array = { rho1, rho2, rho3 };
    vector<function<string()>> print_rho_array = {print_rho1, print_rho2, print_rho3};
    
    string answer_repeat;

    do
    {
        cout << "Выберете функцию rho(x): " << endl;
        for (int i = 0; i < rho_array.size(); i++)
        {
            cout << " " << i + 1 << ") " << print_rho_array[i]() << endl;
        }
        int number_of_rho;
        do
        {
            cout << "Введите номер выбранной функции: " << endl;
            cin >> number_of_rho;
            if (0 < number_of_rho && number_of_rho <= rho_array.size())
            {
                cout << endl;
                try
                {
                    Solution(f0, print_f0, rho_array[number_of_rho - 1], print_rho_array[number_of_rho - 1]);
                }
                catch(exception ex) { }

            }
            else
            {
                cout << "ОШИБКА: Функции с таким номером не обнаружено. Попробуйте выбрать ещё раз." << endl << endl;
                //cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (number_of_rho <= 0 || number_of_rho > rho_array.size());

        cout << "Хотите выбрать другую весовую функцию rho? Y/N" << endl;
        cin >> answer_repeat;
        cout << endl << endl;
    } while (answer_repeat != "N");
}

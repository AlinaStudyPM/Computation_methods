#include <functional>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "functions.h"

using namespace std;

double ExactIntegral(function < double(double)> pf, double a, double b)
{
    return pf(b) - pf(a);
}

double method_left_rectangle(function < double(double)> f, double a, double b)
{
    return (b - a) * f(a);
}
double method_right_rectangle(function < double(double)> f, double a, double b)
{
    return (b - a) * f(b);
}
double method_middle_rectangle(function < double(double)> f, double a, double b)
{
    return (b - a) * f((a + b) / 2);
}
double method_trapeze(function < double(double)> f, double a, double b)
{
    return (b - a) / 2 * (f(a) + f(b));
}
double method_Simpson(function < double(double)> f, double a, double b)
{
    return (b - a) / 6 * (f(a) + 4 * f((a + b) / 2) + f(b));
}
double method_3_8(function < double(double)> f, double a, double b)
{
    double h = (b - a) / 3;
    return (b - a) * ((1.0 / 8) * f(a) + (3.0 / 8) * f(a + h) + (3.0 / 8) * f(a + 2 * h) + (1.0 / 8) * f(b));
}
string double_to_string(double value, int prec = -1)
{
    ostringstream out;
    if (prec != -1)
    {
        out << fixed << std::setprecision(prec) << value;
        string str_value = out.str();
        return str_value;
    }
    out << fixed << std::setprecision(16) << value;
    string str_value = out.str();

    size_t decimal_pos = str_value.find('.');
    if (decimal_pos == std::string::npos) {
        return str_value;
    }
    else
    {
        int i = str_value.length() - 1;
        while (str_value[i] == '0' && i > decimal_pos)
        {
            i--;
        }
        if (str_value[i] == '.')
        {
            i--;
        }
        return str_value.substr(0, i + 1);
    }
}

void print_result(double result, double J)
{
    unsigned prec = 8;
    double R_abs = abs(J - result);
    double R_otn = abs(J - result) / abs(J);
    cout << " Значение интеграла: " << double_to_string(result, prec) << endl;
    cout << " Абсолютная погрешность: " << scientific << R_abs << endl;
    cout << " Относительная погрешность: " << scientific << R_otn << endl;
    cout << endl;
}


void Solution(function < double(double)> f, function < string()> print_f, function < double(double)> pf)
{
    cout << "Исследуемая функция f(x) = " << print_f() << endl;
    cout << endl;

    double a, b;
    cout << "Введите пределы интегрирования a и b: " << endl;
    cin >> a >> b;
    cout << endl << endl;

    cout << "ЭТАП вычислений" << endl;
    cout << fixed << "Отрезок [" << double_to_string(a) << ", " << double_to_string(b) << "]" << endl;
    cout << endl;

    double J = ExactIntegral(pf, a, b);
    cout << "Истинное значение интеграла: " << fixed << J << endl;
    cout << endl;

    cout << "КФ левого прямоугольника" << endl;
    double result_left_rectangle = method_left_rectangle(f, a, b);
    print_result(result_left_rectangle, J);

    cout << "КФ правого прямоугольника" << endl;
    double result_right_rectangle = method_right_rectangle(f, a, b);
    print_result(result_right_rectangle, J);

    cout << "КФ среднего прямоугольника" << endl;
    double result_middle_rectangle = method_middle_rectangle(f, a, b);
    print_result(result_middle_rectangle, J);

    cout << "КФ трапеции" << endl;
    double result_trapeze = method_trapeze(f, a, b);
    print_result(result_trapeze, J);

    cout << "КФ Симпсона" << endl;
    double result_Simpson = method_Simpson(f, a, b);
    print_result(result_Simpson, J);

    cout << "КФ 3/8" << endl;
    double result_3_8 = method_3_8(f, a, b);
    print_result(result_3_8, J);
}


int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "ЗАДАНИЕ 4.2. Приближенное вычисление интеграла по квадратурным формулам." << endl;

    vector<function<double(double)>> functions = { f0, f1, f2, f3, f4, f5 };
    vector<function<string()>> print_func = { print_f0, print_f1, print_f2, print_f3, print_f4, print_f5 };
    vector<function<double(double)>> primitive_func = { pf0, pf1, pf2, pf3, pf4, pf5 };


    string answer = "";
    do
    {
        unsigned k;
        do
        {
            for (int i = 0; i < functions.size(); i++)
            {
                cout << i + 1 << ") " << print_func[i]() << endl;
            }
            cout << "Введите номер исследуемой функции: " << endl;
            cin >> k;
            if (k <= 0 || k > functions.size())
            {
                cout << "Функция с таким номером не обнаружена. Попробуйте ввести ещё раз." << endl;
            }
        } while (k <= 0 || k > functions.size());
        cout << endl;

        Solution(functions[k - 1], print_func[k - 1], primitive_func[k - 1]);

        cout << "Хотите исследовать другую функцию? Y/N" << endl;
        cin >> answer;
        cout << endl << endl;
    } while (answer != "N");
    return 0;
}

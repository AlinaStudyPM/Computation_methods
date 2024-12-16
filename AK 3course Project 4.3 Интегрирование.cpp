#include <iostream>
#include <iomanip>
#include <sstream>
#include <functional>
#include <vector>
#include "functions.h"

using namespace std;

//Точное значение интеграла по формуле Ньютона-Лейбница
double ExactIntegral(function<double(double)> pf, double A, double B)
{
    return pf(B) - pf(A);
}
//Вычисление уточнённого значения по Рунге
double runge(double J, double J_l, unsigned l, unsigned r)
{
    return (pow(l, r) * J_l - J) / (pow(l, r) - 1);
}

//Функции для отображения вещественных чисел
string double_to_string(double value, int prec = -1);
string scientific_to_string(double value);

//Вывод результата для одного метода
void print_result(double result, double J, int width)
{
    unsigned prec = 8;
    double R_abs = abs(J - result);
    double R_otn = abs(J - result) / abs(J);

    string result_in_string = double_to_string(result, prec);
    string R_abs_in_string = scientific_to_string(R_abs);
    string R_otn_in_string = scientific_to_string(R_otn);
    int w1 = width - 22 - result_in_string.size();
    int w2 = width - 26 - R_abs_in_string.size();
    int w3 = width - 29 - R_otn_in_string.size();
    cout << "|  Значение интеграла: " << result_in_string << string(w1, ' ') << "|" << endl;
    cout << "|  Абсолютная погрешность: " << R_abs_in_string << string(w2, ' ') << "|" << endl;
    cout << "|  Относительная погрешность: " << R_otn_in_string << string(w3, ' ') << "|" << endl;
}

//Вывод результатов для всех методов
void print_all_results(vector<string> methods, vector<double> results, double J)
{
    int width = 60;
    cout << "+" << string(width, '-') << "+" << endl;

    for (int i = 0; i < methods.size(); i++)
    {
        int w = width - methods[i].size() - 1;
        cout << "| " << methods[i] << string(w, ' ') << "|" << endl;
        print_result(results[i], J, width);
        cout << "+" << string(width, '-') << "+" << endl;
    }
}

//Главные вычисления
vector<double> CalculateIntegrals(function<double(double)> f, function<string()> print_f, function<double(double)> pf, double A, double B, unsigned m, double J)
{
    //Выводим исходные данные
    cout << "Вычисляем интегралы по СКФ." << endl;
    cout << "Отрезок [" << double_to_string(A) << ", " << double_to_string(B) << "]" << endl;
    cout << "Количество промежутков m = " << m << endl;
    double h;
    h = (B - A) / m;
    cout << fixed << "Величина шага h = " << double_to_string(h) << endl;

    //Вычисляем вспомогательные значения
    double W = 0;
    for (int i = 1; i < m; i++)
    {
        W += f(A + i * h);
    }
    double Q = 0;
    for (int i = 0; i < m; i++)
    {
        Q += f(A + h / 2 + i * h);
    }
    double P = f(A) + f(B);

    //Вычисляем интегралы
    double result_left_rectangle = h * (f(A) + W);
    double result_right_rectangle = h * (W + f(B));
    double result_middle_rectangle = h * Q;
    double result_trapeze = h * (P / 2 + W);
    double result_Simpson = h / 6 * (P + 2 * W + 4 * Q);

    //Возвращаем результат
    vector<double> results = { result_left_rectangle,
                               result_right_rectangle,
                               result_middle_rectangle,
                               result_trapeze,
                               result_Simpson };
    return results;
}

//Главная функция программы
void Solution(function<double(double)> f, function<string()> print_f, function<double(double)> pf)
{
    cout << "ЭТАП ВВОДА ДАНЫХ" << endl;
    cout << endl;

    cout << "Исследуемая функция f(x) = " << print_f() << endl;
    cout << endl;

    string answer_A_B;

    do
    {

        double A, B;
        cout << "Введите пределы интегрирования A и B: " << endl;
        cin >> A >> B;
        cout << endl;

        string answer_m;

        do
        {

            unsigned m;
            cout << "Введите число промежутков m: " << endl;
            cin >> m;
            cout << endl;

            cout << endl;

            cout << "ЭТАП ВЫЧИСЛЕНИЙ" << endl;
            double J = ExactIntegral(pf, A, B);
            cout << "Истинное значение интеграла: " << double_to_string(J) << endl;
            cout << endl;
            vector<string> methods = { "СКФ левого прямоугольника",
                                       "СКФ правого прямоугольника",
                                       "СКФ среднего прямоугольника",
                                       "СКФ трапеции",
                                       "СКФ Симпсона" };
            vector<double> first_result = CalculateIntegrals(f, print_f, pf, A, B, m, J);
            print_all_results(methods, first_result, J);
            cout << endl;

            string answer_runge = "";
            cout << "Хотите уточнить значение по методу Рунге? Y/N" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> answer_runge;
            cout << endl;

            if (answer_runge == "Y")
            {
                cout << "ЭТАП Уточнение по методу Рунге" << endl;
                unsigned l;
                cout << "Введите, во сколько раз цвеличить m = " << m << ": " << endl;
                cin >> l;
                cout << endl;

                vector<double> second_result = CalculateIntegrals(f, print_f, pf, A, B, m * l, J);
                print_all_results(methods, second_result, J);
                cout << endl;

                cout << "Уточнённые значения: " << endl;

                cout << "Истинное значение интеграла: " << double_to_string(J) << endl;

                double runge_left_rectangle = runge(first_result[0], second_result[0], l, 1);
                double runge_right_rectangle = runge(first_result[1], second_result[1], l, 1);
                double runge_middle_rectangle = runge(first_result[2], second_result[2], l, 2);
                double runge_trapeze = runge(first_result[3], second_result[3], l, 2);
                double runge_Simpson = runge(first_result[4], second_result[4], l, 4);

                vector<string> methods = { "СКФ левого прямоугольника",
                                           "СКФ правого прямоугольника",
                                           "СКФ среднего прямоугольника",
                                           "СКФ трапеции",
                                           "СКФ Симпсона" };
                vector<double> runge_results = { runge_left_rectangle,
                                                 runge_right_rectangle,
                                                 runge_middle_rectangle,
                                                 runge_trapeze,
                                                 runge_Simpson };

                print_all_results(methods, runge_results, J);
            }
            cout << "Хотите ввести другое число промежутков m? Y/N" << endl;
            cin >> answer_m;
            cout << endl;

        } while (answer_m != "N");

        cout << "Хотите ввести другие пределы интегрирования A и B? Y/N" << endl;
        cin >> answer_A_B;
        cout << endl;
    } while (answer_A_B != "N");
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

string double_to_string(double value, int prec)
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
string scientific_to_string(double value)
{
    ostringstream out;
    out << scientific << value;
    string str_value = out.str();
    return str_value;
}


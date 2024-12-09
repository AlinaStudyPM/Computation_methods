#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <Eigen/Dense>
#include <iomanip>

using namespace std;

double ro(double x)
{
    return abs(x - 0.5);
}
string print_ro()
{
    return "|x - 0.5|";

}
//Функция f(x) = sin(x)
double f4(double x)
{
    return sin(x);
}
string print_f4()
{
    return "sin(x)";
}
double pf4(double x, bool sign)
{
    
    if (sign)
    {
        return sin(x) - (x - 0.5) * cos(x);
    }
    else
    {
        return -sin(x) + (x - 0.5) * cos(x);
    }
}

double p1(double x, int k, bool sign)
{
    if (sign)
        return  (pow(x, k + 2) / (k + 2) - pow(x, k + 1) / (2 * (k + 1)));
    else
        return - (pow(x, k + 2) / (k + 2) - pow(x, k + 1) / (2 * (k + 1)));
}
double mu(int k, double a, double b)
{
    if ((a - 0.5) * (b - 0.5) > 0)
    {
        return p1(b, k, b - 0.5 > 0) - p1(a, k, a - 0.5 > 0);
    }
    else
    {
        return p1(b, k, b - 0.5 >= 0) - p1(0.5, k, b - 0.5 >= 0) + p1(0.5, k, a - 0.5 >= 0) - p1(a, k, a - 0.5 >= 0);
    }
}
//Представление вещественных чисел в виде строки (значащие цифры)
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
void print_result_vector(Eigen::VectorXd vec, string name)
{
    int width = 22;
    int w1 = (width - name.length()) / 2;
    int w2 = width - name.length() - w1;
    cout << "+" << string(width, '-') << "+" << endl;
    cout << "|" << string(w1, ' ') << name << string(w2, ' ') << "|" << endl;
    cout << "+" << string(width, '-') << "+" << endl;
    for (int i = 0; i < vec.size(); i++)
    {
        //cout << "Число: " << vec[i] << endl;
        string number = double_to_string(vec[i], 16);
        int n1 = (width - number.length()) / 2;
        int n2 = width - number.length() - n1;
        cout << "|" << string(n1, ' ') << number << string(n2, ' ') << "|" << endl;
    }
    cout << "+" << string(width, '-') << "+" << endl;
}


double ExactIntegral(function<double(double, bool)> pf, double a, double b)
{
    if ((a - 0.5) * (b - 0.5) > 0)
    {
        return pf4(b, b - 0.5 > 0) - pf4(a, a - 0.5 > 0);
    }
    else
    {
        return pf4(b, b - 0.5 >= 0) - pf4(0.5, b - 0.5 >= 0) + pf4(0.5, a - 0.5 >= 0) - pf4(a, a - 0.5 >= 0);
    }
}

void Solution(function<double(double)> f, function<string()> print_f, function<double(double, bool)> pf)
{
    cout << "ЗАДАНИЕ 4.1. Приближенное вычисление интегралов при помощи интерполяционных формул (ИКФ)" << endl;
    cout << "Исследуемая функция f(x) = " << print_f() << endl;
    cout << "Весовая функция ro(x) = " << print_ro() << endl;
    cout << endl;

    string answer = "";

    do
    {
        double a, b;
        cout << "Введите пределы интегрирования a и b: " << endl;
        cin >> a >> b;
        cout << endl;

        unsigned n;
        Eigen::VectorXd nodes;
        bool different;
        do
        {
            n = NULL;
            cout << "Введите количество узлов n:" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> n;


            nodes = Eigen::VectorXd(n);
            different = true;
            cout << "Введите ваши n=" << n << " различных узлов через пробел: " << endl;
            for (int i = 0; i < n; i++)
            {
                double t;
                cin >> t;
                if (find(nodes.begin(), nodes.end(), t) != nodes.end())
                {
                    different = false;
                    cout << "Узлы должны быть различными. Попробуйте ввести еще раз" << endl;
                    break;
                }
                else
                {
                    nodes(i) = t;
                }
            }
            cout << endl;
        } while (!different);
        sort(nodes.begin(), nodes.end());

        cout << "ЭТАП 1. Вычисление коэффициентов ИКФ" << endl;
        cout << "Отрезок: [" << double_to_string(a) << ", " << double_to_string(b) << "]" << endl;
        cout << "Набор узлов x_i:";
        cout << "{";
        for (int i = 0; i < n; i++)
        {
            if (i > 0) cout << ", ";
            cout << double_to_string(nodes[i]);
        }
        cout << "}" << endl;

        //Создание матрицы
        Eigen::MatrixXd matrix(n, n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                matrix(i, j) = pow(nodes[j], i);
            }
        }

        //Столбец свободных членов + надо вывести на экран
        Eigen::VectorXd vector_mu(n);
        cout << "Вектор Мю из моментов весовой функции: " << endl;
        for (int i = 0; i < n; ++i)
        {
            vector_mu(i) = mu(i, a, b);
        }
        print_result_vector(vector_mu, "mu");

        //Решаем систему
        Eigen::VectorXd vector_A = matrix.colPivHouseholderQr().solve(vector_mu);
        cout << "Найденные коэффициенты A_i: " << endl;
        print_result_vector(vector_A, "A_i");

        //Считаем КФ
        double myIntegral = 0;
        for (int k = 0; k < n; k++)
        {
            myIntegral += vector_A(k) * f(nodes(k));
        }
        cout << "Найденное значение интеграла: " << fixed << setprecision(16) << myIntegral << endl;
        double integral = ExactIntegral(pf, a, b);
        cout << "Истинное значение интеграла: " << integral << endl;
        cout << "Абсолютная погрешность: " << scientific << abs(myIntegral - integral) << endl;
        cout << endl;

        //Сравниваем с точным значением

        //Проверка на многочлене
        auto a_func = [n](double x) -> double { return pow(x, n - 1); };
        auto p_a_func = [n](double x, bool sign) -> double
            {
                if (sign == true)
                {
                    return pow(x, n + 1) / (n + 1) - 0.5 * pow(x, n) / n;
                }
                else
                {
                    return -pow(x, n + 1) / (n + 1) + 0.5 * pow(x, n) / n;
                }
            };
        double test_integral = 0;
        for (int k = 0; k < n; k++)
        {
            test_integral += vector_A(k) * a_func(nodes(k));
        }
        cout << "Значение интеграла для x^(n-1): " << fixed << setprecision(16) << test_integral << endl;
        double test_exact_integral;
        if ((a - 0.5) * (b - 0.5) > 0)
        {
            test_exact_integral = p_a_func(b, b - 0.5 > 0) - p_a_func(a, a - 0.5 > 0);
        }
        else
        {
            test_exact_integral = p_a_func(b, b - 0.5 >= 0) - p_a_func(0.5, b - 0.5 >= 0) + p_a_func(0.5, a - 0.5 >= 0) - p_a_func(a, a - 0.5 >= 0);
        }

        cout << "Истинное значение интеграла: " << test_exact_integral << endl;
        cout << "Абсолютная погрешность: " << scientific << abs(test_integral - test_exact_integral) << endl;
        cout << endl;

        cout << "Хотите ввести другой отрезок [a, b]? Y/N" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> answer;
    } while (answer != "N");
}

int main()
{
    setlocale(LC_ALL, "Russian");
    Solution(f4, print_f4, pf4);
    return 0;
}

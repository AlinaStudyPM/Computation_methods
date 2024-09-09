#include <iostream>
#include <iomanip>
#include <set>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

void separate_roots(double A, double B, double H, set<pair<double, double>>& root_intervals); //Локализация корней
void method_bisection(double a, double b, double e, double& x, double& d, unsigned int& m, double& last_step, vector<double>& x_0); //Метод бисекции
void method_Newton(double a, double b, double e, double& x, double& d, unsigned int& m, double& last_step, vector<double>& x_0); //Метод Ньютона (или метод секущих)
void method_Newton_modified(double a, double b, double e, double& x, double& d, unsigned int& m, double& last_step, vector<double>& x_0); //Модифицированный метод Ньютона
void method_secant(double a, double b, double e, double& x, double& d, unsigned int& m, double& last_step, vector<double>& x_0); //Метод секущих
void input_with_default(double default_value, double& t); //метод для ввода значений параметров
void print_result(double x, double d, double m, vector<double> first_approx, double last_step); //вывод результатов

int main()
{
    //setlocale(LC_ALL, "Russian");
    //system("chcp 1251");

    double A, B, e, h;
    set<pair<double, double>> root_intervals;
    set <double> roots;
    unsigned roots_counter = 0, N = 100;

    cout << "ЗАДАНИЕ: Найти все корни нечётной кратности уравнения 10 * cos(x) - 0,1 * x^2 на отрезке [A, B] с точностью e" << endl << endl;
    const double A_default = -8.0,
        B_default = 2.0,
        e_default = 1e-5;
    do
    {

        cout << "Введите значение левой границы поиска A (по умолчанию " << A_default << "): ";
        input_with_default(A_default, A);
        cout << "Введите значение правой границы поиска B (по умолчанию " << B_default << "): ";
        input_with_default(B_default, B);
        if (B - A < 0)
        {
            cout << "Некорректные границы поиска. Попробуйте ввести ещё раз." << endl;
        }
    } while (B - A < 0);
    cout << endl;

    cout << "ЭТАН 1. Отделение корней" << endl;
    cout << "Отрезок поиска: [" << A << "; " << B << "]" << endl << endl;
    string answer;
    do
    {
        do
        {
            cout << "Введите длину шага h (по умолчанию 1/" << N << " длины отрезка [A, B]): ";
            input_with_default((B - A) / N, h);
            if (h <= 0)
            {
                cout << "Длина шага h должна быть положительной. Попробуйте ввести ещё раз" << endl;
            }
        } while (h <= 0);
        cout << "Величина шага поиска: " << h << endl;
        root_intervals = set<pair<double, double>>();
        separate_roots(A, B, h, root_intervals);
        cout << "Количество найденных отрезков: " << root_intervals.size() << endl;
        for (auto interval : root_intervals)
        {
            cout << "[" << interval.first << "; " << interval.second << "]" << endl;
        }
        cout << "Хотите ввести другое значение h? Y/N" << endl;
        cin >> answer;
        cout << endl;
    } while (answer != "N");
    cout << endl;

    cout << "ЭТАП 2. Уточнение корней" << endl << endl;
    do
    {
        do
        {
            cout << "Введите необходимую точность e (? ????? " << e_default << "): ";
            input_with_default(e_default, e);
            if (e <= 0)
            {
                cout << "Некорректная точность. Попробуйте ввести ещё раз." << endl;
            }
        } while (e <= 0);
        cout << "Заданная точность e = " << e << endl << endl;
        roots = set<double>();
        int k = 1;
        for (auto interval : root_intervals)
        {
            cout << k << ") Уточнение корня на отрезке [" << interval.first << "; " << interval.second << "]:" << endl << endl;

            cout << "Метод бисекции:" << endl;
            double x_method1, d_method1, last_step_method1;
            vector<double> x_0_method1 = vector<double>(1);
            unsigned m_method1 = 0;
            method_bisection(interval.first, interval.second, e, x_method1, d_method1, m_method1, last_step_method1, x_0_method1);
            print_result(x_method1, d_method1, m_method1, x_0_method1, last_step_method1);

            cout << "Метод Ньютона (метод касательных):" << endl;
            double x_method2, d_method2 = -1, last_step_method2;
            vector<double> x_0_method2 = vector<double>(1);
            unsigned m_method2 = 0;
            method_Newton(interval.first, interval.second, e, x_method2, d_method2, m_method2, last_step_method2, x_0_method2);
            print_result(x_method2, d_method2, m_method2, x_0_method2, last_step_method2);

            cout << "Модифицированный метод Ньютона:" << endl;
            double x_method3, d_method3 = -1, last_step_method3;
            vector<double> x_0_method3 = vector<double>(1);
            unsigned m_method3 = 0;
            method_Newton_modified(interval.first, interval.second, e, x_method3, d_method3, m_method3, last_step_method3, x_0_method3);
            print_result(x_method3, d_method3, m_method3, x_0_method3, last_step_method3);

            cout << "Метод секущих:" << endl;
            double x_method4, d_method4 = -1, last_step_method4;
            vector<double> x_0_method4 = vector<double>(2);
            unsigned m_method4 = 0;
            method_secant(interval.first, interval.second, e, x_method4, d_method4, m_method4, last_step_method4, x_0_method4);
            print_result(x_method4, d_method4, m_method4, x_0_method4, last_step_method4);

            cout << endl;
            k++;
        }
        cout << "Хотите ввести другое значение e? Y/N" << endl;
        cin >> answer;
        cout << endl;
    } while (answer != "N");
}

//Исследуемая функция
double f(double x)
{
    return 10 * cos(x) - 0.1 * x * x;
}

//Производная исследуемой функции
double d_f(double x)
{
    return -10 * sin(x) - 0.2 * x;
}

//Локализация корней
void separate_roots(double A, double B, double H, set<pair<double, double>>& root_intervals)
{
    double X1, X2, Y1, Y2;
    X1 = A;
    X2 = A + H;
    Y1 = f(X1);
    while (X2 <= B)
    {
        Y2 = f(X2);
        if (Y1 * Y2 <= 0)
        {
            root_intervals.insert({ X1, X2 });
        }
        X1 = X2;
        X2 = X1 + H;
        Y1 = Y2;
    }
}

//Метод бисекции
void method_bisection(double a, double b, double e, double& x, double& d, unsigned int& m, double& last_step, vector<double>& x_0)
{
    m = 0;
    double c;
    x_0[0] = (a + b) / 2;
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

        m++;
        last_step = b - a;
    }
    x = (a + b) / 2;
    d = (b - a) / 2;
}

//Метод Ньютона (или метод секущих)
void method_Newton(double a, double b, double e, double& x, double& d, unsigned int& m, double& last_step, vector<double>& x_0)
{
    x_0[0] = (a + b) / 2;
    double x_prev;
    m = 0;
    x = x_0[0];
    x_prev = INFINITY;
    while (abs(x - x_prev) >= e)
    {
        x_prev = x;
        x = x - f(x) / d_f(x);

        m++;
        last_step = abs(x - x_prev);
    }
}

//Модифицированный метод Ньютона
void method_Newton_modified(double a, double b, double e, double& x, double& d, unsigned int& m, double& last_step, vector<double>& x_0)
{
    x_0[0] = (a + b) / 2;
    double x_prev;
    double der_x_0 = d_f(x_0[0]);
    m = 0;
    x = x_0[0];
    x_prev = INFINITY;
    while (abs(x - x_prev) >= e)
    {
        x_prev = x;
        x = x - f(x) / der_x_0;

        m++;
        last_step = abs(x - x_prev);
    }
}

//Метод секущих
void method_secant(double a, double b, double e, double& x, double& d, unsigned int& m, double& last_step, vector<double>& x_0)
{
    x_0[0] = a + (b - a) / 3;
    x_0[1] = b - (b - a) / 3;
    double x_prev, x_preprev;
    m = 0;
    x = x_0[0];
    x_prev = x_0[1];
    while (abs(x - x_prev) >= e)
    {
        x_preprev = x_prev;
        x_prev = x;
        x = x_prev - f(x_prev) / (f(x_prev) - f(x_preprev)) * (x_prev - x_preprev);
        
        m++;
        last_step = abs(x - x_prev);
    }
}

//Метод для ввода значений параметров
void input_with_default(double default_value, double& t)
{
    string input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, input);
    if (input.empty()) {
        t = default_value;
    }
    else {
        t = stod(input);
    }
}

//Вывод результатов
void print_result(double x, double d, double m, vector<double> x_0, double last_step)
{
    //cout.fill('.');
    cout << "\t" << setiosflags(ios::left) << setw(40) << "Начальное приближение к корню ";
    for (double x : x_0)
    {
        cout << x << "  ";
    }
    cout << endl;
    cout << "\t" << setiosflags(ios::left) << setw(40) << "Количество шагов " << m << endl;
    cout << "\t" << setiosflags(ios::left) << setw(40) << "Модуль разности |x_m - x_(m-1)| " << last_step << endl;
    cout << "\t" << setiosflags(ios::left) << setw(40) << "Корень " << std::setprecision(15) << x << endl;                                                   //<< std::setprecision(15)
    cout << "\t" << setiosflags(ios::left) << setw(40) << "Абсолютная величина невязки " << abs(f(x)) << endl;

}






/*К методу Ньютона
    Второго порядка сходимости для корня кратности p можно достичь,
    применяя расчетную формулу вида
    x_k = x_(k-1) - p * f(x_(k-1)) / d_f(x_(k-1))
*/
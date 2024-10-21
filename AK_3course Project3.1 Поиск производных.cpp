#include <iostream>
#include <set>
#include <vector>
#include <random>
#include <iomanip>
#include <string>

using namespace std;

//исследуемуюя функция
double f(double x, unsigned k)
{
    if (k == 1)
        return exp(-1 * x) - x * x / 2;
    if (k == 2)
        return exp(1.5 * x);
}
string write_function(int k)
{
    if (k == 1)
        return "exp(-1 * x) - x * x / 2";
    if (k == 2)
        return "exp(1.5 * x)";
}
//настоящая производная f(x)
double df(double x, unsigned k)
{
    if (k == 1)
        return -(1 + x * exp(x)) * exp(-1 * x);
    if (k == 2)
        return 1.5 * exp(1.5 * x);
}
//настоящая вторая производная f(x)
double d2f(double x, unsigned k)
{
    if (k == 1)
        return exp(-1 * x) - 1;
    if (k == 2)
        return 2.25 * exp(1.5 * x);
}
struct result_line
{
    double x;
    double f;
    double df;
    double r_df;
    double new_df;
    double r_new_df;
    double d2f;
    double r_d2f;
    const int width = 8;
};
struct runge_line
{
    double x;
    double f;
    double J_h;
    double r_J_h;
    double J_h2;
    double r_J_h2;
    double J;
    double r_J;
};
//Функция для ввода вещественных чисел
void input_with_default(double default_value, double& t)
{
    string input;
    cin >> input;
    if (input == ".")
    {
        t = default_value;
    }
    else
    {
        replace(input.begin(), input.end(), '.', ',');
        t = stod(input);
    }
}
//Функция для ввода целых чисел
void input_with_default(unsigned default_value, unsigned& t)
{
    string input;
    cin >> input;
    if (input == ".")
    {
        t = default_value;
    }
    else
    {
        t = stoi(input);
    }
}
//ввод параметра m
void input_m(unsigned& m)
{
    double m_default = 15;
    do
    {
        cout << "Введите число значений в таблице m >= 4: ";
        input_with_default(m_default, m);
        if (m < 4)
        {
            cout << "Неправильное значение m. Значение должно быть не меньше 4. Попробуйте ввести ещё раз." << endl;
        }
    } while (m < 4);
    cout << endl;
}
//ввод параметра x_0
void input_x_0(double& x_0)
{
    double x_0_default = 0;
    cout << "Введите начальное значение x_0 в таблице: ";
    input_with_default(x_0_default, x_0);
    cout << endl;
}
//выбор точки x_j для Рунге-Ромберга
void input_x_j(unsigned& j, vector<result_line> table_f)
{
    do
    {
        cout << "Выберете номер j точки для уточнения: ";
        cin >> j;
        if (j < 0 || j >= table_f.size())
        {
            cout << "Такого номера j не обнаружено в таблице. Попробуйте ввести ещё раз." << endl;
        }
    } while (j < 0 || j >= table_f.size());
    cout << endl;
}
//ввод параметра h
void input_h(double& h)
{
    //double h_default = 1;
    do
    {
        string input;
        cout << "Введите длину шага h > 0: ";
        cin >> input;
        replace(input.begin(), input.end(), '.', ',');
        h = stod(input);
        cout << endl;
        if (h <= 0)
        {
            cout << "Неправильное значение h. Значение должно быть больше 0. Попробуйте ввести ещё раз." << endl;
        }
    } while (h <= 0);
    cout << endl;
}
//Вывод табличной функции в консоль для result_line
void print_table_vertical(vector<result_line> table_f)
{
    cout << "+-----+---------------+---------------+" << endl;
    cout << "| №   |       x       |      f(x)     |" << endl;
    cout << "+-----+---------------+---------------+" << endl;

    // Печатаем значения с нумерацией
    for (size_t i = 0; i < table_f.size(); ++i) {
        cout << "| " << left << setw(4) << i
            << "| " << left << setw(14) << fixed << setprecision(6) << table_f[i].x
            << "| " << left << setw(13) << table_f[i].f << " |" << endl;
    }

    cout << "+-----+---------------+---------------+" << endl;
}
//Вывод табличной функции в консоль для runge_line
void print_table_vertical(vector<runge_line> table_f)
{
    cout << "+-----+---------------+---------------+" << endl;
    cout << "| №   |       x       |      f(x)     |" << endl;
    cout << "+-----+---------------+---------------+" << endl;

    // Печатаем значения с нумерацией
    for (size_t i = 0; i < table_f.size(); ++i) {
        cout << "| " << left << setw(4) << i
            << "| " << left << setw(14) << fixed << setprecision(6) << table_f[i].x
            << "| " << left << setw(13) << table_f[i].f << " |" << endl;
    }

    cout << "+-----+---------------+---------------+" << endl;
}
//основные вычисления первых и вторых производных
void calculate_formulas(vector<result_line>& tf, double h, unsigned k)
{
    unsigned m = tf.size() - 1;
    for (int i = 0; i < m + 1; i++)
    {
        if (i == 0)
        {
            tf[0].df = (-3 * tf[0].f + 4 * tf[1].f - tf[2].f) / (2 * h);

            tf[0].d2f = (2 * tf[0].f - 5 * tf[1].f + 4 * tf[2].f - tf[3].f) / (h * h);

            tf[0].new_df = (-25 * tf[0].f + 48 * tf[1].f - 36 * tf[2].f + 16 * tf[3].f - 3 * tf[4].f) / (12 * h);
        }
        else if (i == m)
        {
            tf[m].df = (3 * tf[m].f - 4 * tf[m - 1].f + tf[m - 2].f) / (2 * h);

            tf[m].d2f = (2 * tf[m].f - 5 * tf[m - 1].f + 4 * tf[m - 2].f - tf[m - 3].f) / (h * h);

            tf[m].new_df = (25 * tf[m].f - 48 * tf[m - 1].f + 36 * tf[m - 2].f - 16 * tf[m - 3].f + 3 * tf[m - 4].f) / (12 * h);
        }
        else
        {
            tf[i].df = (tf[i + 1].f - tf[i - 1].f) / (2 * h);

            tf[i].d2f = (tf[i + 1].f - 2 * tf[i].f + tf[i - 1].f) / (h * h);

            if (i == 1)
            {
                tf[1].new_df = (-3 * tf[0].f - 10 * tf[1].f + 18 * tf[2].f - 6 * tf[3].f + tf[4].f) / (12 * h);
            }
            else if (i == m - 1)
            {
                tf[m - 1].new_df = (3 * tf[m].f + 10 * tf[m - 1].f - 18 * tf[m - 2].f + 6 * tf[m - 3].f - tf[m - 4].f) / (12 * h);
            }
            else
            {
                tf[i].new_df = (tf[i - 2].f - 8 * tf[i - 1].f + 8 * tf[i + 1].f - tf[i + 2].f) / (12 * h);
            }
        }
        tf[i].r_df = abs(df(tf[i].x, k) - tf[i].df);
        tf[i].r_d2f = abs(d2f(tf[i].x, k) - tf[i].d2f);
        tf[i].r_new_df = abs(df(tf[i].x, k) - tf[i].new_df);

        cout << i << ") Погрешность новой производной: " << df(tf[i].x, k) << " - " << tf[i].new_df << " = " << df(tf[i].x, k) - tf[i].new_df << endl;
    }
}
//Вывод результата в консоль
void print_result_table(vector<result_line> result_table)
{
    //---------------
    cout << "+-----+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+" << endl;
    cout << "| №   |       x       |      f(x)     |   f'(x)_ЧД1   |  Погрешность  |   f'(x)_ЧД2   |  Погрешность  |   f''(x)_ЧД   |  Погрешность  |" << endl;
    cout << "|     |               |               |  (6),(7),(8)  |   f'(x)_ЧД1   |  (из файла)   |   f'(x)_ЧД2   | (9)+авторские |   f''(x)_ЧД   |" << endl;
    cout << "+-----+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+" << endl;

    // Печатаем значения с нумерацией
    for (size_t i = 0; i < result_table.size(); ++i) {
        cout << "| " << left << setw(4) << i
            << "| " << left << setw(14) << fixed << setprecision(6) << result_table[i].x
            << "| " << left << setw(14) << fixed << result_table[i].f
            << "| " << left << setw(14) << fixed << result_table[i].df
            << "| " << left << setw(14) << scientific << result_table[i].r_df
            << "| " << left << setw(14) << fixed << result_table[i].new_df
            << "| " << left << setw(14) << scientific << result_table[i].r_new_df
            << "| " << left << setw(14) << fixed << result_table[i].d2f
            << "| " << left << setw(13) << scientific << result_table[i].r_d2f
            << " |" << endl;
    }

    cout << "+-----+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+" << endl;
}
//вывод результата для Рунге-Ромберга
void print_runge_line(runge_line r)
{
    cout << "+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+" << endl;
    cout << "|       x       |      f(x)     |     J(h)      |  Погрешность  |    J(h/2)     |  Погрешность  |  Уточнённое   |  Погрешность  |" << endl;
    cout << "|               |               |  (6),(7),(8)  |     J(h)      |  (6),(7),(8)  |     J(h/2)    |       J       |       J       |" << endl;
    cout << "+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+" << endl;

    cout << "| " << left << setw(14) << fixed << setprecision(6) << r.x
        << "| " << left << setw(14) << fixed << r.f
        << "| " << left << setw(14) << fixed << r.J_h
        << "| " << left << setw(14) << scientific << r.r_J_h
        << "| " << left << setw(14) << fixed << r.J_h2
        << "| " << left << setw(14) << scientific << r.r_J_h2
        << "| " << left << setw(14) << fixed << r.J
        << "| " << left << setw(13) << scientific << r.r_J
        << " |" << endl;

    cout << "+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+" << endl;
}
//Построение табличной функции в точке x_0 на основе f(x) для result_line
void create_table(unsigned k, double x_0, double h, int m, vector<result_line>& table_f)
{
    double x;
    for (int i = 0; i <= m; ++i)
    {
        x = x_0 + i * h;
        table_f[i].x = x;
        table_f[i].f = f(x, k);
    }
}
//Построение табличной функции в точке x_0 на основе f(x) для runge_line
void create_table(unsigned k, double x_0, double h, int m, vector<runge_line>& table_f)
{
    double x;
    for (int i = 0; i <= m; ++i)
    {
        x = x_0 + i * h;
        table_f[i].x = x;
        table_f[i].f = f(x, k);
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    unsigned m;
    double x_0, h;

    cout << "ЗАДАНИЕ 3: Задача численного дифференцирования" << endl;
    cout << "1) " << write_function(1) << endl;
    cout << "2) " << write_function(2) << endl;
    cout << endl;

    string answer_2;
    do
    {
        int k;
        cout << "Введите порядковый номер исследуемой функции: ";
        cin >> k;
        cout << endl;

        string answer_1;
        do
        {
            input_m(m);
            input_x_0(x_0);
            input_h(h);

            cout << "ЭТАП 1. Подготовительный" << endl;
            cout << "Исследуемая функция f(x) = " << write_function(k) << endl;
            cout << "Начальное значение x_0 = " << x_0 << endl;
            cout << "Длина шага h = " << h << endl;
            cout << "Число разбиений m = " << m << endl;
            vector<result_line> tf = vector<result_line>(m + 1);
            create_table(k, x_0, h, m, tf);
            print_table_vertical(tf);
            cout << endl;


            cout << "ЭТАП 2. Нахождение первой и второй производной" << endl;
            calculate_formulas(tf, h, k);
            print_result_table(tf);
            cout << endl;

            cout << "Хотите ввести другие значения m, x_0, h для данной функции? Y/N" << endl;
            cin >> answer_1;
        } while (answer_1 == "Y");

        cout << "Хотите выбрать другую функцию для исследования? Y/N" << endl;
        cin >> answer_2;
    } while (answer_2 == "Y");


    string answer_3;
    cout << "Хотите уточнить значения, используя метод Рунге-Ромберга? Y/N" << endl;
    cin >> answer_3;
    if (answer_3 == "Y")
    {
        cout << endl << endl;
        cout << "ЭТАП 3. Метод Рунге-Ромберга уточнения значений." << endl;

        cout << "1) " << write_function(1) << endl;
        cout << "2) " << write_function(2) << endl;
        cout << endl;

        int k;
        cout << "Введите порядковый номер исследуемой функции: ";
        cin >> k;
        cout << endl;

        input_m(m);
        input_x_0(x_0);
        input_h(h);

        vector<result_line> rf = vector<result_line>(m + 1);
        vector<result_line> rf2 = vector<result_line>(2 * m + 1);
        create_table(k, x_0, h, m, rf);
        create_table(k, x_0, h / 2, 2 * m, rf2);
        calculate_formulas(rf, h, k);
        calculate_formulas(rf2, h / 2, k);

        print_table_vertical(rf);
        cout << endl;
        unsigned j;
        input_x_j(j, rf);
        runge_line r1;
        r1.x = rf[j].x;
        r1.f = rf[j].f;
        r1.J_h = rf[j].df;
        r1.r_J_h = rf[j].r_df;
        r1.J_h2 = rf2[2 * j].df;
        r1.r_J_h2 = rf2[2 * j].r_df;

        r1.J = r1.J_h2 + (r1.J_h2 - r1.J_h) / (4 - 1);
        r1.r_J = abs(r1.J - df(r1.x, k));

        runge_line r2;
        r2.x = rf[j].x;
        r2.f = rf[j].f;
        r2.J_h = rf[j].d2f;
        r2.r_J_h = rf[j].r_d2f;
        r2.J_h2 = rf2[2 * j].d2f;
        r2.r_J_h2 = rf2[2 * j].r_d2f;

        r2.J = r2.J_h2 + (r2.J_h2 - r2.J_h) / (4 - 1);
        r2.r_J = abs(r2.J - d2f(r2.x, k));

        cout << "Уточнение значений производных f(x) = " << write_function(k) <<" в точке x_" << j << " = " << rf2[2 * j].x << " по Рунге - Ромбергу" << endl;
        cout << "Для первой производной: " << endl;
        print_runge_line(r1);
        cout << "Для второй производной: " << endl;
        print_runge_line(r2);
    }
}
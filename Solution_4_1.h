#pragma once


#ifndef SOLUTION_4_1_H
#define SOLUTION_4_1_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <functional>
#include <Eigen/Dense>
#include <boost/math/quadrature/gauss_kronrod.hpp>
#include <algorithm>

using namespace std;

double Exact_Integral(function<double(double)> f, double a, double b)
{
    return boost::math::quadrature::gauss_kronrod<double, 16>::integrate(f, a, b);
}

double mu(function<double(double)> rho, int k, double a, double b) //Заменить на вычисление через библиотеку
{
    auto degree = [k](double x) { return pow(x, k); };
    auto integrand = [degree, rho](double x) { return degree(x) * rho(x); };
    return Exact_Integral(integrand, a, b);
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

void print_result_vector(Eigen::VectorXd vec, string name)
{
    int precision = 12;
    int width = 22;
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
    cout << "+" << string(width, '-') << "+" << endl;
    cout << "|" << string(w1, ' ') << name << string(w2, ' ') << "|" << endl;
    cout << "+" << string(width, '-') << "+" << endl;
    for (int i = 0; i < vec.size(); i++)
    {
        //cout << "Число: " << vec[i] << endl;
        int n1 = (width - numbers_in_string[i].size()) / 2;
        int n2 = width - numbers_in_string[i].size() - n1;
        cout << "|" << string(n1, ' ') << numbers_in_string[i] << string(n2, ' ') << "|" << endl;
    }
    cout << "+" << string(width, '-') << "+" << endl;
}

vector<double> Solution_4_1(function<double(double)> f, function<double(double)> rho, double a, double b, vector<double> x)
{
    int n = x.size();
    Eigen::VectorXd nodes = Eigen::VectorXd::Map(x.data(), x.size());

    cout << "Отрезок: [" << double_to_string(a) << ", " << double_to_string(b) << "]" << endl;
    cout << "Набор узлов x_i:" << endl;
    print_result_vector(nodes, "x_i");
    /*cout << "{";
    for (int i = 0; i < n; i++)
    {
        if (i > 0) cout << ", ";
        cout << double_to_string(nodes[i]);
    }
    cout << "}" << endl;*/

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
        vector_mu(i) = mu(rho, i, a, b);
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

    //Выводим результат
    cout << "РЕЗУЛЬТАТ" << endl;
    auto integrand = [f, rho](double x) { return f(x) * rho(x); };
    double exact_integral = Exact_Integral(integrand, a, b);
    cout << "   Истинное значение интеграла:  " << fixed << setprecision(16) << exact_integral << endl;
    cout << "   Найденное значение интеграла: " << fixed << setprecision(16) << myIntegral << endl;
    cout << "   Абсолютная погрешность: " << scientific << abs(myIntegral - exact_integral) << endl;
    cout << "   Относительная погрешность: " << scientific << abs(myIntegral - exact_integral) / abs(exact_integral) << endl;
    cout << endl;

    return vector<double>(vector_A.data(), vector_A.data() + vector_A.size());
}



#endif // FUNCTIONS_H
#include "functions.h"

#include <iostream>
#include <algorithm>

using namespace std;

//Функция f(x) = x
double f0(double x)
{
    return 1;
}
string print_f0()
{
    return "1";
}
double pf0(double x)
{
    return x;
}

//Функция f(x) = x
double f1(double x)
{
    return x;
}
string print_f1()
{
    return "x";
}
double pf1(double x)
{
    return pow(x, 2) / 2;
}


//Функция f(x) = x^2
double f2(double x)
{
    return pow(x, 2);
}
string print_f2()
{
    return "x^2";
}
double pf2(double x)
{
    return pow(x, 3) / 3;
}


//Функция f(x) = x^3
double f3(double x)
{
    return pow(x, 3);
}
string print_f3()
{
    return "x^3";
}
double pf3(double x)
{
    return pow(x, 4) / 4;
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
double pf4(double x)
{
    return -cos(x);
}


//Функция f(x) = e^x
double f5(double x)
{
    return exp(x);
}
string print_f5()
{
    return "e^x";
}
double pf5(double x)
{
    return exp(x);
}
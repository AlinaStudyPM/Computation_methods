#include <iostream>
#include <iomanip>
#include <Eigen/Dense>
#include <utility>
#include <tabulate/table.hpp>

using namespace std;
using namespace Eigen;
using namespace tabulate;

pair<double, VectorXd> find_eigenvalue_and_eigenvector_by_power_method(const MatrixXd& A, double eps)
{
    if (A.rows() != A.cols())
    {
        throw invalid_argument("Matrix must be square!");
    }
    int n = A.rows();

    //Выбираем рандомный стартовый вектор и нормализуем его
    VectorXd eigenvector = VectorXd::Random(n);
    eigenvector.normalize();

    //Заводим переменные для собственного значения на текущей и предыдущей итерации
    double eigenvalue = 0.0;
    double eigenvalue_prev = 0.0;

    //Нулевая итерация
    eigenvector = A * eigenvector;
    eigenvector.normalize();
    eigenvalue = eigenvector.dot(A * eigenvector) / eigenvector.norm();
    
    int count = 0;
    do
    {
        eigenvalue_prev = eigenvalue;
        eigenvector = A * eigenvector;
        eigenvector.normalize();
        eigenvalue = eigenvector.dot(A * eigenvector) / eigenvector.norm();
        count++;

    } while(abs(eigenvalue - eigenvalue_prev) > eps);
    cout << "Степенной метод: " << count << " итераций." << endl;
    return {eigenvalue, eigenvector};
}

pair<VectorXd, MatrixXd> find_eigenvalues_and_eigenvectors_by_Jacobi_rotation_method(const MatrixXd& A, double eps)
{
    if (A.rows() != A.cols())
    {
        throw invalid_argument("Matrix must be square!");
    }
    int n = A.rows();

    MatrixXd eigenvectors = MatrixXd::Identity(n, n);
    MatrixXd B = A;

    int count = 0;
    do
    {
        //Ищем наибольший элемент вне диагонали и его индексы i_max и j_max
        double max_offdiag = 0;
        int i_max = 0, j_max = 0;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i != j && abs(B(i, j)) > max_offdiag)
                {
                    max_offdiag = abs(B(i, j));
                    i_max = i;
                    j_max = j;
                }
            }
        }
        if (max_offdiag < eps)
        {
            cout << "Метод вращений Якоби: " << count  << " итераций" << endl;
            break;
        }

        // Вычисляем угол вращения
        double theta;
        if (abs(B(i_max, i_max) - B(j_max, j_max)) < 1e-10)
        {
            theta = M_PI / 4;
        }
        else
        {
            theta = 0.5 * atan2(2 * B(i_max, j_max), B(i_max, i_max) - B(j_max, j_max));
        }
        double c = cos(theta);
        double s = sin(theta);
        //Составляем матрицу вращений T
        MatrixXd T = MatrixXd::Identity(n, n);
        T(i_max, i_max) = c;
        T(j_max, j_max) = c;
        T(i_max, j_max) = -s;
        T(j_max, i_max) = s;

        B = T.transpose() * B * T;
        eigenvectors = eigenvectors * T;

        count++;
    } while (count < 1e6);

    //Собственные числа
    VectorXd eigenvalues = B.diagonal();
    return {eigenvalues, eigenvectors};
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

Table print_matrix(const Eigen::MatrixXd& A) {
    Table table;
    
    // Настройка стиля таблицы
    table.format()
        .border_top("-")
        .border_bottom("-")
        .border_left("|")
        .border_right("|")
        .corner("+");
    
    // Добавляем строки с данными
    for (int i = 0; i < A.rows(); ++i) {
        Table::Row_t row;
        for (int j = 0; j < A.cols(); ++j) {
            row.push_back(double_to_string(A(i,j)));
        }
        table.add_row(row);
    }
    
    // Выравнивание по центру
    for (auto& cell : table[0]) {
        cell.format().font_align(FontAlign::center);
    }
    
    return table;
}

tabulate::Table create_eigen_table(const VectorXd& eigenvalues, 
                                 const MatrixXd& eigenvectors,
                                 int value_prec = 4, 
                                 int vector_prec = 8) {
    tabulate::Table table;
    
    // Настройка стиля
    table.format()
        .border_top("-")
        .border_bottom("-")
        .border_left("|")
        .border_right("|")
        .corner("+");
    
    // Заголовки с собственными значениями
    tabulate::Table::Row_t header;
    for (int i = 0; i < eigenvalues.size(); ++i) {
        header.push_back("a = " + double_to_string(eigenvalues[i], value_prec) + " ");
    }
    table.add_row(header);
    
    // Компоненты векторов
    for (int row = 0; row < eigenvectors.rows(); ++row) {
        tabulate::Table::Row_t table_row;
        for (int col = 0; col < eigenvectors.cols(); ++col) {
            table_row.push_back(double_to_string(eigenvectors(row, col), vector_prec));
        }
        table.add_row(table_row);
    }

    // Автоподбор ширины
    for (size_t col = 0; col < eigenvalues.size(); ++col) {
        size_t max_width = 0;
        for (size_t row = 0; row < table.size(); ++row) {
            max_width = max(max_width, table[row][col].get_text().length());
        }
        table.column(col).format().width(max_width + 2);
    }
    
    // Выравнивание
    for (auto& cell : table[0]) {
        cell.format().font_align(tabulate::FontAlign::center);
    }
    for (size_t i = 1; i < table.size(); ++i) {
        for (auto& cell : table[i]) {
            cell.format().font_align(tabulate::FontAlign::right);
        }
    }
    
    return table;
}

int main()
{
    cout << endl << endl << "ЗАДАНИЯ 6 и 7. Поиск собственных чисел и собственных векторов." << endl;
    MatrixXd A;
    MatrixXd D(4, 4);
    D << 1, -3, 5, 7,
         -3, 9, 11, 13,
         5, 11, 15, -17,
         7, 13, -17, 19;
    MatrixXd C(4, 4);
    C << 0.5, 0, 1, 1,
         0, 0.5, 0, 0,
         1, 0, 0.5, 0,
         1, 0, 0, 0.5;
    double gamma = 7;
    A = D + gamma * C;
    cout << "Matrix A:" << endl;
    cout << print_matrix(A) << endl;

    cout << endl;

    int prec_values = 6;
    int prec_vectors = 8;

    //Степенной метод
    auto [eigenvalue_1, eigenvector_1] = find_eigenvalue_and_eigenvector_by_power_method(A, 1e-6);
    VectorXd eigenvalues_1(1);
    eigenvalues_1 << eigenvalue_1;
    MatrixXd eigenvectors_1 = eigenvector_1;
    cout << create_eigen_table(eigenvalues_1, eigenvectors_1, prec_values, prec_vectors) << endl;

    cout << endl;

    //Метод вращений Якоби
    auto [eigenvalues_2, eigenvectors_2] = find_eigenvalues_and_eigenvectors_by_Jacobi_rotation_method(A, 1e-6);
    cout << create_eigen_table(eigenvalues_2, eigenvectors_2, prec_values, prec_vectors) << endl;

    cout << endl;

    cout << "3) Проверка" << endl;
    EigenSolver<MatrixXd> solver(A);
    VectorXd eigenvalues_True = solver.eigenvalues().real();
    MatrixXd eigenvectors_True = solver.eigenvectors().real();
    cout << create_eigen_table(eigenvalues_True, eigenvectors_True, prec_values, prec_vectors) << endl;

    cout << endl;
    return 0;
}

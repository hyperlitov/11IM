#include <iostream>
#include <vector>
#include <cmath>


class Matrix
{
protected:                    
    int row;
    int col;
    std::vector<std::vector<float>> values;

public:
//конструкторы
    Matrix(int r, int c, float number = 0.);
    Matrix(int r, int c, std::vector<float> data);
    Matrix(int r, int c, std::vector<std::vector<float>> data);
//методы и операторы
    Matrix operator + (const Matrix& M);
    Matrix operator + (const float& num);
    Matrix operator - (const Matrix& M);
    Matrix operator - (const float& num);
    Matrix operator * (const Matrix& M);
    Matrix operator * (const float& num);
    Matrix operator - ();
    Matrix T();
    void print();
    int getRow() const { return row; }
    int getCol() const { return col; }
//вспомогательные методы для Sq_Matrix
protected:
    float det();          
    Matrix inverse();      
};

//квадратные матрицы
class Sq_Matrix: public Matrix
{
public:
//конструкторы
    Sq_Matrix(int size, float number = 0)
        : Matrix(size, size, number) {}

    Sq_Matrix(int size, std::vector<float> data)
        : Matrix(size, size, data) {}

    Sq_Matrix(int size, std::vector<std::vector<float>> data)
        : Matrix(size, size, data) {}
//методы
    Sq_Matrix(const Matrix& m);
    Sq_Matrix operator !();
    Sq_Matrix inverse();
    float det();
};



// Векторные матрицы
class Row_Vector: public Matrix
{
public:
    Row_Vector(int size, std::vector<float> data)
        : Matrix(1, size, data) {}

    Row_Vector(int size, float number = 0)
        : Matrix(1, size, number) {}

    float abs();
};

// Векторные матрицы
class Col_Vector: public Matrix
{
public:
    Col_Vector(int size, std::vector<float> data)
        : Matrix(size, 1, data) {}

    Col_Vector(int size, float number = 0)
        : Matrix(size, 1, number) {}

    float abs();
};


float Matrix::det()
{
    if(row != col){
        throw "Matrix must be square to calculate determinant";
    }

    if(row == 1){
        return values[0][0];
    }

    if(row == 2){
        return values[0][0] * values[1][1] - values[0][1] * values[1][0];
    }

    float determinant = 0;
    
    for(int c = 0; c < col; ++c){
        std::vector<std::vector<float>> sub_data;
        for(int i = 1; i < row; ++i){
            std::vector<float> sub_row;
            for(int j = 0; j < col; ++j){
                if(j == c) continue;
                sub_row.push_back(values[i][j]);
            }
            sub_data.push_back(sub_row);
        }
        Matrix subMatrix(row - 1, col - 1, sub_data);
        determinant += (c % 2 == 0 ? 1 : -1) * values[0][c] * subMatrix.det();
    }
    return determinant;
}

Matrix::Matrix(int r, int c, float number)
{
    values = std::vector<std::vector<float>>(r, std::vector<float>(c, number));
    row = r;
    col = c;
}

Matrix::Matrix(int r, int c, std::vector<float> data)
{
    values = std::vector<std::vector<float>>(r, std::vector<float>(c, 0));
    row = r;
    col = c;
    if(data.size() != (size_t)r*c){
        throw "Data size does not match matrix size";
    }
    int i = 0;
    for(auto& str: values)
        for(float& n: str)
            n = data[i++];
}

Matrix::Matrix(int r, int c, std::vector<std::vector<float>> data)
{
    if(data.size() != (size_t)r || data.empty() || data[0].size() != (size_t)c){
        throw "Data size does not match matrix size";
    }
    values = data;
    row = r;
    col = c;
    int i = 0;
    for(auto& elem: values)
        elem = data[i++];
}

Matrix Matrix::operator + (const Matrix& M)
{
    if(row != M.row || col != M.col){
        throw "Matrices dimensions don't match for addition";
    }
    Matrix result(row, col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            result.values[i][j] = values[i][j] + M.values[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator + (const float& num)
{
    Matrix result(row, col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            result.values[i][j] = values[i][j] + num;
        }
    }
    return result;
}

Matrix Matrix::operator - (const Matrix& M)
{
    if(row != M.row || col != M.col){
        throw "Matrices dimensions don't match for subtraction";
    }
    Matrix result(row, col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            result.values[i][j] = values[i][j] - M.values[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator - (const float& num)
{
    Matrix result(row, col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            result.values[i][j] = values[i][j] - num;
        }
    }
    return result;
}

Matrix Matrix::operator * (const Matrix& M)
{
    if(col != M.row){
        throw "Matrix's size doesn't match for multiplication";
    }
    Matrix result(row, M.col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < M.col; ++j){
            result.values[i][j] = 0;
            for(int k = 0; k < col; ++k){
                result.values[i][j] += values[i][k] * M.values[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::operator * (const float& num)
{
    Matrix result(row, col);
    for(int i = 0; i < row; ++i){
        for( int j = 0; j < col; ++j){
            result.values[i][j] = values[i][j] * num;
        }
    }
    return result;
}

Matrix Matrix::operator - ()
{
    Matrix result(row, col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            result.values[i][j] = -values[i][j];
        }
    }
    return result;
}

Matrix Matrix::T()
{
    Matrix result(col, row);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            result.values[j][i] = values[i][j];
        }
    }
    return result;
}

void Matrix::print()
{
    for(const auto& elem : values)
    {
        for(const auto& n : elem)
        {
            std::cout << n << ' ';
        }
        std::cout << '\n';
    }
}

Matrix Matrix::inverse()
{
    if(row != col){
        throw "Matrix must be square for inverse";
    }
    float d = det();
    if(d == 0){
        throw "Error! det = 0";
    }
    Matrix cof(row, col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            std::vector<std::vector<float>> minor;
            for(int mi = 0; mi < row; ++mi){
                if(mi == i) continue;
                std::vector<float> minor_row;
                for(int mj = 0; mj < col; ++mj){
                    if(mj == j) continue;
                    minor_row.push_back(values[mi][mj]);
                }
                minor.push_back(minor_row);
            }
            Matrix m(row-1, col-1, minor);
            cof.values[i][j] = ((i+j)%2==0 ? 1 : -1) * m.det();
        }
    }
    Matrix adj = cof.T();
    return adj * (1.0f/d);
}



// Реализация методов Sq_Matrix
Sq_Matrix::Sq_Matrix(const Matrix& m) 
    : Matrix(m)
{
    if(m.getRow() != m.getCol()) {
        throw "Matrix must be square!";
    }
}

float Sq_Matrix::det()
{
    return Matrix::det();
}

Sq_Matrix Sq_Matrix::operator!()
{
    Matrix inv = Matrix::inverse();  
    return Sq_Matrix(inv);            
}

Sq_Matrix Sq_Matrix::inverse()
{
    Matrix inv = Matrix::inverse();
    return Sq_Matrix(inv);
}

float Row_Vector::abs()
{
    float sum = 0.0f;
    if(row != 1) throw "Row_Vector internal size mismatch";
    for(int j = 0; j < col; ++j) sum += values[0][j] * values[0][j];
    return sqrt(sum);
}

float Col_Vector::abs()
{
    float sum = 0.0f;
    if(col != 1) throw "Col_Vector internal size mismatch";
    for(int i = 0; i < row; ++i) sum += values[i][0] * values[i][0];
    return std::sqrt(sum);
}

// Примеры использования
int main()
{
    try {
        std::cout << "___ constructors and print ___\n";
        Matrix m_const(2, 2, 1.5f);
        std::cout<<"Matrix initialized with constant 1.5:\n";
        m_const.print();

        std::vector<float> flat = {1,2,3,4,5,6};
        std::cout<<"\nMatrix initialized with vector {1,2,3,4,5,6}:\n";
        Matrix m_flat(2, 3, flat);
        m_flat.print();

        std::vector<std::vector<float>> mat2x2 = {{1,2},{3,4}};
        Matrix A(2,2,mat2x2);
        std::cout << "A:\n"; A.print();


        Matrix B(2,2, std::vector<float>{5,6,7,8});
        std::cout << "\nA + B:\n"; (A + B).print();
        std::cout << "\nA + 1.0:\n"; (A + 1.0f).print();


        std::cout << "\nB - A:\n"; (B - A).print();
        std::cout << "\n-A:\n"; (-A).print();


        std::cout << "\nA^T:\n"; A.T().print();

        
        std::vector<std::vector<float>> bdata = {{1,2,3},{4,5,6}};
        Matrix C(2,3,bdata);
        std::cout << "\nA * C:\n";
        Matrix AC = A * C; // A(2x2) * C(2x3)
        AC.print();

        
        try {
            Matrix D(3,2,bdata); // 3x2 matrix
            Matrix E = A * D;   
            E.print();
        }
        catch (const char* msg) {
            std::cout << "Error! (mul): " << msg << '\n';
        }

        std::cout << "\nSq_Matrix examples:\n";
        Sq_Matrix S(A);
        float d = S.det();
        std::cout << "det(A) = " << d << '\n';

        Sq_Matrix Sinv = S.inverse();
        std::cout << "inverse(A):\n"; Sinv.print();

        std::cout << "A * A^{-1}:\n";
        (A * Sinv).print();

        std::cout << "operator! -> inverse:\n";
        ( !S ).print();

        std::cout << "\nRow_Vector / Col_Vector:\n";
        Row_Vector rv(3, std::vector<float>{1,2,3});
        Col_Vector cv(3, std::vector<float>{1,2,3});
        std::cout << "Row vector: "; rv.print();
        std::cout << "Col vector: "; cv.print();
        std::cout << "||rv|| = " << rv.abs() << '\n';
        std::cout << "||cv|| = " << cv.abs() << '\n';

        std::cout << "\nA * 2.0:\n"; (A * 2.0f).print();

    }
    catch (const char* msg) {
        std::cout << "Error: " << msg << std::endl;
        return -1;
    }
    catch (...) {
        std::cout << "Unknown error" << std::endl;
        return -1;
    }

    return 0;
}
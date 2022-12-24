// БЛОК II - ЗАДАЧИ 6 -10 

//Athor:Kashapoova Dilyara, 11-001

#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <string>
#include <thread>
#include <cassert>

using namespace std;

int const ARR_SIZE = 10;
const int MIN_VALUE = -10; // минимальное значение элементов матрицы
const int MAX_VALUE = 10; // максимальное значение

void show_arr(int* arr, int size)
{
    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;
}

void show_vec(int* arr, int size)
{
    string str = "(";
    for (int i = 0; i < size; i++) {
        if (i < size - 1) {
            str = str + std::to_string(arr[i]) + ", ";
        }
        else {
            str = str + std::to_string(arr[i]);
        }
    }

    str += ")";
    cout << str << endl;
}

void show_matrix(int** arr, int x, int y)
{
    int* buf = new int[y];
    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {

            buf[j] = arr[i][j];
        }
        show_vec(buf, y);
        buf = new int[y];
    }
    delete[]buf;
}

void mult_vec(int* arrX, int size, int* arrY)
{
    for (int i = 0; i < size; i++) arrX[i] = arrX[i] * arrY[i];
}

void reverce_vecS(int* arrX, int size, int* arrY)
{
    int q;
    for (int i = 0; i < size; i++) {
        q = arrX[i];
        arrX[i] = arrY[i];
        arrY[i] = q;
    }
}


// Написать программу вычисления нормы вектора (сумма моодулей элементов вектора, n>100), Для распределения элементов вектора x по процессам 
// использовать функцию MPI_Scatter.Для получения итоговой суммы на нулевом процессе использовать функцию MPI_Reduce с операцией MPI_Sum..
namespace Task11
{
    int main(int argc, char* argv[])
    {
        int size = 4, rank;
        int rc;
        int block = 3;
        MPI_Status mpi_stat;

        if (rc = MPI_Init(&argc, &argv))
        {
            cout << "Ошибка запуска, выполнение остановлено " << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        int N = size * block;

        //вектор Х, который является потоки * блок
        int* x = new int[N];
        // принимающий блок
        int* y = new int[block];

        srand(time(NULL));

        // первый поток послал значения
        if (rank == 0) {
            cout << " MPI - TASK 11 \n\n";

            for (int i = 0; i < N; ++i) x[i] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);

            cout << "x : ";
            show_vec(x, N);  
        }

        MPI_Scatter(&x[0], block, MPI_INT, &y[0], block, MPI_INT, 0, MPI_COMM_WORLD);

        int sum = 0;
        for (int i = 0; i < block; ++i) sum += abs(y[i]);

        int result;

        //MPI_SUM — это предопределенная операция сокращения, которая вычисляет сумму приведенных значений.
        MPI_Reduce(&sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        if (rank == 0) cout << "\n|sum{vec}| = " << result << endl;

        MPI_Finalize();

        return MPI_SUCCESS;
    }
}

// Написать программу вычисления скалярного произведения двух векторов.Для распределения элементов вектора x по процессам использовать 
// функцию MPI_Scatter.Для получения итоговой суммы на нулевом процессе использовать функцию MPI_Reduce с операцией MPI_Sum.
namespace Task12
{
    int main(int argc, char* argv[])
    {
        int size = 4, rank;
        int rc;
        int block = 2;
        MPI_Status mpi_stat;

        if (rc = MPI_Init(&argc, &argv))
        {
            cout << "Ошибка запуска, выполнение остановлено " << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        int N = size * block;

        int* x = new int[N];
        int* y = new int[N]; 
        int* xBuff = new int[block];
        int* yBuff = new int[N];

        srand(time(NULL));

        if (rank == 0) {
            cout << " MPI - TASK 12 \n\n";

            for (int i = 0; i < N; ++i) {
                x[i] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
                y[i] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
            }

            cout << "vect x : ";
            show_vec(x, N);
            cout << "vect y : ";
            show_vec(y, N);
            cout << "\n";
        }

        MPI_Scatter(&x[0], block, MPI_INT, &xBuff[0], block, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(&y[0], block, MPI_INT, &yBuff[0], block, MPI_INT, 0, MPI_COMM_WORLD);

        int sum = 0;
        for (int i = 0; i < block; ++i) sum += xBuff[i] * yBuff[i];

        int result;

        MPI_Reduce(&sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        // собраили все значения, и потом 1 поток распечатал его
        if (rank == 0) cout << "result (sum[xi * yi]) = " << result << endl;

        MPI_Finalize();

        return MPI_SUCCESS;
    }
}

// Написать программу умножения матрицы на вектор C = A * b. Распределить матрицу равномерно вдоль строк, вектор передать всем процессам.После
// выполнения операции умножения на каждом из процессов необходимо выполнить сбор полученных локальных частей результирующего вектора на
// один процесс(с номером 0).Использовать функцию MPI_Bcast для рассылки вектора для распределения элементов матрицы использовать функцию
// MPI_Scatter, для сбора локальных частей результирующего вектора в глобальный вектор использовать функцию MPI_Gather.
namespace Task13
{
    int main(int argc, char* argv[])
    {
        int size = 4, rank;
        int rc;
        int const Y = 5;
        MPI_Status mpi_stat;
        MPI_Datatype rtype;
        if (rc = MPI_Init(&argc, &argv))
        {
            cout << "Ошибка запуска, выполнение остановлено " << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        int N = size;
        int** a = new int* [N];
        for (int i = 0; i < N; i++) a[i] = new int[Y];

        int* b = new int[Y];

        int* aBuff = new int[Y];
        int* bBuff = new int[Y];

        srand(time(NULL));

        if (rank == 0) {
            cout << " MPI - TASK 13 \n\n";

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < Y; ++j) {
                    a[i][j] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
                }
            }

            cout << "matrix A:" << endl;
            show_matrix(a, N, Y);

            for (int i = 0; i < Y; ++i) b[i] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);

            cout << "\nvect b : ";
            show_vec(b, Y);
            cout << endl;
        }
        //MPI_Type_vector(N - size, 1, Y, MPI_INT, &rtype);
        //MPI_Type_commit(&rtype);
        MPI_Scatter(&a[0][0], Y, MPI_INT, &aBuff[0], Y, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(&b[0], Y, MPI_INT, &bBuff[0], Y, MPI_INT, 0, MPI_COMM_WORLD);

        for (int i = 0; i < Y; ++i) {
                aBuff[i] *= bBuff[i];
        }

        MPI_Gather(&aBuff[0], Y, MPI_INT, &a[rank][0], Y, MPI_INT, 0, MPI_COMM_WORLD);


        if (rank == 0) {
            cout << "\nmatrix C :\n";
            show_matrix(a, N, Y);
        }
        

        MPI_Finalize();

        return MPI_SUCCESS;
    }
}

//. Написать программу вычисления нормы матрицы по формуле max|xij|где матрица A =n*m.Для распределения элементов матрицы А по процессам использовать функцию 
//MPI_Scatter.Для получения итогового значения использовать функцию MPI_Reduce с операцией MPI_MAX.
namespace Task14
{
    int main(int argc, char* argv[])
    {
        int size = 4, rank;
        int rc;
        int const M = 5;
        MPI_Status mpi_stat;

        if (rc = MPI_Init(&argc, &argv))
        {
            cout << "Ошибка запуска, выполнение остановлено " << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        int N = 14;
        int block = N/size;

        int** a = new int* [N];
        for (int i = 0; i < N; i++) a[i] = new int[M];

        int* aBuff = new int[M];

        srand(time(NULL));

        if (rank == 0) {
            cout << " MPI - TASK 13 \n\n";

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < M; ++j) {
                    a[i][j] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
                }
            }

            cout << "matrix A:" << endl;
            show_matrix(a, N, M);
        }

        MPI_Scatter(&a[0][0], block * M, MPI_INT, &aBuff[0], block * M, MPI_INT, 0, MPI_COMM_WORLD);

        int max = MIN_VALUE - 1;
        for (int i = 0; i < M; ++i) if(aBuff[i] > max) max = aBuff[i];
        show_vec(aBuff, M);

        int result;

        MPI_Reduce(&max, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
        // собраили все значения, и потом 1 поток распечатал его
        if (rank == 0) cout << "result (max|xij|) = " << result << endl;

        MPI_Finalize();

        return MPI_SUCCESS;
    }
}

// Написать программу вычисления поэлементного умножения матриц .Использовать функции MPI_Scatter для распределения элементов матриц A и B, и MPI_Gather для сбора вычисленных 
// данных в матрицу C.

namespace Task15
{
    int main(int argc, char* argv[]) {
        int rank, size;
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        int N = 2, M = 2;
        assert(N * M % size == 0);
        int block = N * M / size;
        int* aBuff = new int[block];
        int* bBuff = new int[block];
        int** a = new int* [N];
        for (int i = 0; i < N; i++) a[i] = new int[N];

        int** b = new int* [N];
        for (int i = 0; i < N; i++) b[i] = new int[N];

        srand(time(NULL));

        if (rank == 0) {
            cout << " MPI - TASK 15 \n\n";

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < M; ++j) {
                    a[i][j] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
                    b[i][j] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
                }
            }

            cout << "matrix A:" << endl;
            show_matrix(a, N, M);

            cout << "matrix B:" << endl;
            show_matrix(b, N, M);
        }

        MPI_Scatter(&(a[0][0]), block, MPI_INT, &aBuff[0], block, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(&(b[0][0]), block, MPI_INT, &bBuff[0], block, MPI_INT, 0, MPI_COMM_WORLD);
        for (int i = 0; i < block; ++i) aBuff[i] *= bBuff[i];
        MPI_Gather(&aBuff[0], block, MPI_INT, &(a[0][0]), block, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            cout << "matrix C:" << endl;
            show_matrix(a, N, M);
        }

        MPI_Finalize();
        return MPI_SUCCESS;
    }
}

int main(int argc, char* argv[])
{
    Task13::main(argc, argv);
}

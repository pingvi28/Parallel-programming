// БЛОК II - ЗАДАЧИ 6 -10 

//Athor:Kashapoova Dilyara, 11-001

#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <string>
#include <thread>

using namespace std;

int const ARR_SIZE = 10;
const int MIN_VALUE = 0; // минимальное значение элементов матрицы
const int MAX_VALUE = 20; // максимальное значение

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


//Пусть матрицы вещественных чисел A, B заданы на нулевом процессе. Написать программу, в которой матрицы равными кусками с нулевого процесса 
//пересылаются остальным процессам. Пересылка данных осуществляется функцией MPI_Send. Все процессы по формуле вычисляют свои элементы искомых 
//матриц. Каждый процесс отправляет на нулевой процесс подсчитанные элементы матриц.

//Написать программу, вычисляющую элементы вектора по формуле z = x * y.Векторы задаются на нулевом процессе и равными блоками пересылаются 
//остальным процессам.Пересылка данных осуществляется функцией MPI_Send.Все процессы по формуле вычисляют свои элементы массива 
//Каждый процесс отправляет на нулевой процесс подсчитанные элементы.
namespace Task6
{
    int main(int argc, char* argv[])
    {
        int size = 4, rank;
        int rc;
        MPI_Status mpi_stat;

        if (rc = MPI_Init(&argc, &argv))
        {
            cout << "Ошибка запуска, выполнение остановлено " << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);


        // убираем root поток
        size--;
        int countSendElements = ARR_SIZE / size;

        srand(time(NULL));

        // главный поток == root
        if (rank == 0) {
            cout << " MPI - TASK 6 \n\n";

            // счетчик потоков
            int thread_id = 1;
            int x[ARR_SIZE];
            int y[ARR_SIZE];

            //заполенение векторов
            for (int i = 0; i < ARR_SIZE; ++i) {
                x[i] = rand() % 10;
                y[i] = rand() % 10;
            }

            cout << "x = ";
            show_vec(x, ARR_SIZE);
            cout << "y = ";
            show_vec(y, ARR_SIZE);
            cout << "\n";

            int lastCount = ARR_SIZE % (size);
            int interval = countSendElements;

            int t = countSendElements;
            for (int i = 0; i < ARR_SIZE; i += countSendElements) {

                if (thread_id == size && lastCount != 0) interval = lastCount;

                int s = 0;
                for (int q = 0; q < t; q++, s++) {
                    MPI_Send(&x[i + s], interval, MPI_INT, thread_id, 11000, MPI_COMM_WORLD);
                    MPI_Send(&y[i + s], interval, MPI_INT, thread_id, 11000, MPI_COMM_WORLD);
                }

                thread_id++;
                if (thread_id == size && lastCount != 0) t = lastCount;
            }

            interval = countSendElements;
            int index = 0;
            for (int i = 1; i <= size; i++) {
                if (i == size && lastCount != 0) interval = lastCount;

                int* buf = new int[interval];

                MPI_Recv(&buf[0], interval, MPI_INT, i, 203023024, MPI_COMM_WORLD, &mpi_stat);

                for (int j = 0; j < interval; j++, index++) {
                    if (index < ARR_SIZE) {
                        y[index] = buf[j];
                    }
                }

            }

            cout << "z = xi * yi\n\nz = ";
            show_vec(y, ARR_SIZE);
        }
        else {
            MPI_Probe(0, 11000, MPI_COMM_WORLD, &mpi_stat);
            int buf_size;
            MPI_Get_count(&mpi_stat, MPI_INT, &buf_size);

            int* x = new int[buf_size];
            int* y = new int[buf_size];
            MPI_Recv(&x[0], buf_size, MPI_INT, 0, 11000, MPI_COMM_WORLD, &mpi_stat);
            MPI_Recv(&y[0], buf_size, MPI_INT, 0, 11000, MPI_COMM_WORLD, &mpi_stat);

            mult_vec(x, buf_size, y);

            MPI_Send(&x[0], buf_size, MPI_INT, 0, 203023024, MPI_COMM_WORLD);
        }

        finish_process:MPI_Finalize();

        return MPI_SUCCESS;
    }
}

//Написать программу, которая меняет местами элементы векторов x, y .Векторы задаются на нулевом процессе и равными блоками пересылаются 
//остальным процессам.Пересылка данных осуществляется функцией MPI_Send.Каждый процесс отправляет на нулевой процесс подсчитанные элементы.
namespace Task7
{
    int main(int argc, char* argv[])
    {
        int size = 4, rank;
        int rc;
        MPI_Status mpi_stat;

        if (rc = MPI_Init(&argc, &argv))
        {
            cout << "Ошибка запуска, выполнение остановлено " << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);


        // убираем root поток
        size--;
        int countSendElements = ARR_SIZE / size;

        srand(time(NULL));

        // главный поток == root
        if (rank == 0) {
            cout << " MPI - TASK 7 \n\n";

            // счетчик потоков
            int thread_id = 1;
            int x[ARR_SIZE];
            int y[ARR_SIZE];

            //заполенение векторов
            for (int i = 0; i < ARR_SIZE; ++i) {
                x[i] = rand() % 10;
                y[i] = rand() % 10;
            }

            cout << "(before) x = ";
            show_vec(x, ARR_SIZE);
            cout << "(before) y = ";
            show_vec(y, ARR_SIZE);
            cout << "\n";

            int lastCount = ARR_SIZE % (size);
            int interval = countSendElements;

            int t = countSendElements;
            for (int i = 0; i < ARR_SIZE; i += countSendElements) {

                if (thread_id == size && lastCount != 0) interval = lastCount;

                int s = 0;
                for (int q = 0; q < t; q++, s++) {
                    MPI_Send(&x[i + s], interval, MPI_INT, thread_id, 11000, MPI_COMM_WORLD);
                    MPI_Send(&y[i + s], interval, MPI_INT, thread_id, 11000, MPI_COMM_WORLD);
                }

                thread_id++;
                if (thread_id == size && lastCount != 0) t = lastCount;
            }

            interval = countSendElements;
            int index = 0;
            for (int i = 1; i <= size; i++) {
                if (i == size && lastCount != 0) interval = lastCount;

                int* bufX = new int[interval];
                int* bufY = new int[interval];

                MPI_Recv(&bufX[0], interval, MPI_INT, i, 11001, MPI_COMM_WORLD, &mpi_stat);
                MPI_Recv(&bufY[0], interval, MPI_INT, i, 11002, MPI_COMM_WORLD, &mpi_stat);

                for (int j = 0; j < interval; j++, index++) {
                    if (index < ARR_SIZE) {
                        x[index] = bufX[j];
                        y[index] = bufY[j];
                    }
                }

            }

            cout << "(after) x = ";
            show_vec(x, ARR_SIZE);
            cout << "(after) y = ";
            show_vec(y, ARR_SIZE);
            cout << "\n";
        }
        else {
            MPI_Probe(0, 11000, MPI_COMM_WORLD, &mpi_stat);
            int buf_size;
            MPI_Get_count(&mpi_stat, MPI_INT, &buf_size);

            int* x = new int[buf_size];
            int* y = new int[buf_size];
            MPI_Recv(&x[0], buf_size, MPI_INT, 0, 11000, MPI_COMM_WORLD, &mpi_stat);
            MPI_Recv(&y[0], buf_size, MPI_INT, 0, 11000, MPI_COMM_WORLD, &mpi_stat);

            reverce_vecS(x, buf_size, y);

            MPI_Send(&x[0], buf_size, MPI_INT, 0, 11001, MPI_COMM_WORLD);
            MPI_Send(&y[0], buf_size, MPI_INT, 0, 11002, MPI_COMM_WORLD);
        }

    finish_process:MPI_Finalize();

        return MPI_SUCCESS;
    }
}

//В программе реализовать поэлементное умножение матриц:
namespace Task8
{
    int main(int argc, char** argv)
    {
        int size = 4, rank;
        int rc;
        int const len = 9;
        MPI_Status mpi_stat;

        if (rc = MPI_Init(&argc, &argv))
        {
            cout << "Ошибка запуска, выполнение остановлено " << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        int N = size;
        N--;

        // убираем root поток
        size--;

        srand(time(NULL));

        // главный поток == root
        if (rank == 0) {
            cout << " MPI - TASK 8 \n\n";
            // счетчик потоков
            int thread_id = 1;
            const int* q = new int[len];

            int** x = new int* [N];
            int** y = new int* [N];
            int** c = new int* [N];
            for (int i = 0; i < N; ++i) {
                x[i] = new int[len];
                y[i] = new int[len];
                c[i] = new int[len];
            }

            //заполенение векторов
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < len; ++j) {
                    x[i][j] = rand() % 10;
                    y[i][j] = rand() % 10;
                }
            }

            cout << "A: \n";
            show_matrix(x, N, len);
            cout << "\nB \n";
            show_matrix(y, N, len);
            cout << "\n";

            for (int i = 1; i <= size; i ++) {
                MPI_Send(x[i-1], len, MPI_INT, i, 11000, MPI_COMM_WORLD);
                MPI_Send(y[i-1], len, MPI_INT, i, 11001, MPI_COMM_WORLD);
            }

            for (int i = 1; i <= size; i ++) {

                int* buf = new int[len];

                MPI_Recv(&buf[0], len, MPI_INT, i, 11002, MPI_COMM_WORLD, &mpi_stat);

                c[i-1] = buf;
            }

            cout << "\nC: \n";
            show_matrix(c, N, len);
        }
        else {

            int* x = new int[len];
            int* y = new int[len];
            MPI_Recv(x, len, MPI_INT, 0, 11000, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(y, len, MPI_INT, 0, 11001, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            mult_vec(x, len, y);

            MPI_Send(x, len, MPI_INT, 0, 11002, MPI_COMM_WORLD);
        }

        MPI_Finalize();

        return MPI_SUCCESS;
    }
}

//В программе реализовать умножение матриц
namespace Task9
{
    int main(int argc, char** argv)
    {
        int size = 4, rank;
        int rc;
        MPI_Status mpi_stat;

        if (rc = MPI_Init(&argc, &argv))
        {
            cout << "Ошибка запуска, выполнение остановлено " << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        int N = size;
        N--;

        srand(time(NULL));

        if (rank == 0) {
            cout << " MPI - TASK 9 \n\n";
            int** a = new int* [N];
            int** b = new int* [N];
            for (int i = 0; i < N; i++)
            {
                a[i] = new int[N];
                b[i] = new int[N];
            }

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    a[i][j] = rand() % 10;
                    b[i][j] = rand() % 10;
                }
            }

            cout << "A:" << endl;
            show_matrix(a, N, N);

            cout << "\nB:" << endl;
            show_matrix(b, N, N);

            for (int i = 1; i <= N; i++) {
                MPI_Send(a[i - 1], N, MPI_INT, i, 11000, MPI_COMM_WORLD);
                for (int j = 0; j < N; j++)
                {
                    MPI_Send(b[j], N, MPI_INT, i, 11001 + j, MPI_COMM_WORLD);
                }
            }

            for (int i = 1; i <= N; i++) {
                MPI_Recv(a[i - 1], N, MPI_INT, i, 11003, MPI_COMM_WORLD, &mpi_stat);
            }

            cout << "\nC:" << endl;
            show_matrix(a, N, N);

        }
        else {
            int sum = 0;
            int* a = new int[N];
            int** b = new int* [N];

            for (int i = 0; i < N; i++)
            {
                b[i] = new int[N];
            }

            MPI_Recv(a, N, MPI_INT, 0, 11000, MPI_COMM_WORLD, &mpi_stat);

            for (int i = 0; i < N; i++)
            {
                MPI_Recv(b[i], N, MPI_INT, 0, 11001 + i, MPI_COMM_WORLD, &mpi_stat);
            }
            int* bufA = new int[N];
            int* bufB = new int[N];
            int* buf = new int[N];

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    bufA[j] = a[j];
                }

                for (int j = 0; j < N; ++j) {
                    bufB[j] = b[j][i];
                }

                mult_vec(bufA, N, bufB);

                for (int j = 0; j < N; ++j) {
                    sum += bufA[j];
                }
                buf[i] = sum;
                sum = 0;
            }
            MPI_Send(buf, N, MPI_INT, 0, 11003, MPI_COMM_WORLD);
        }

        MPI_Finalize();
        return MPI_SUCCESS;
    }
}

//В программе реализовать транспонирование матрицы
namespace Task10
{
    int main(int argc, char** argv)
    {
        int size = 4, rank;
        int rc;
        MPI_Status mpi_stat;

        if (rc = MPI_Init(&argc, &argv))
        {
            cout << "Ошибка запуска, выполнение остановлено " << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        int N = size;
        N--;

        srand(time(NULL));

        if (rank == 0) {
            cout << " MPI - TASK 10 \n\n";
            int** a = new int* [N];
 
            //Инициализация матрицы
            for (int i = 0; i < N; i++)
            {
                a[i] = new int[N];

            }

            // заполенние матрицы
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    a[i][j] = rand() % 10;
                }
            }

            cout << "A:" << endl;
            show_matrix(a, N, N);

            // отправка элементов
            for (int i = 1; i <= N; ++i) {
                for (int j = 0; j < N; ++j) {
                    MPI_Send(&a[j][i-1], 1, MPI_INT, i, 11001 + i, MPI_COMM_WORLD);
                }
            }

            // приним
            for (int i = 1; i <= N; i++) {
                MPI_Recv(a[i - 1], N, MPI_INT, i, 11003, MPI_COMM_WORLD, &mpi_stat);
            }

            cout << "\nC:" << endl;
            show_matrix(a, N, N);

        }
        else {
            int sum = 0;
            int* a = new int[N];
            int buf = 0;

            for (int i = 0; i < N; i++)
            {
                MPI_Recv(&buf, 1, MPI_INT, 0, 11001 + rank, MPI_COMM_WORLD, &mpi_stat);
                a[i] = buf;
            }

            MPI_Send(a, N, MPI_INT, 0, 11003, MPI_COMM_WORLD);
        }

        MPI_Finalize();
        return MPI_SUCCESS;
    }
}

int main(int argc, char* argv[])
{
    Task9::main(argc, argv);
}

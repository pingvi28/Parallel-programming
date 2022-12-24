// БЛОК IV 

//Athor:Kashapoova Dilyara, 11-001

#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <string>
#include <thread>
#include <cassert>
#include <random>

using namespace std;

int const N = 8;
const int MIN_VALUE = -10; // минимальное значение элементов матрицы
const int MAX_VALUE = 10; // максимальное значение
const int process = 5;

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


// На процессе с номером 0 объявить и заполнить значениями матрицу a[8][8]. Создать новый тип данных для рассылки нечетных строк матрицы а в матрицу
//b[4][8], и для рассылки четных строк матрицы в матрицу c[4][8], используя функцию MPI_Type_vector.
namespace Task1
{
    int main(int argc, char* argv[]) {
        int rank, size;
        int rc;
        MPI_Datatype newRowType;
        MPI_Status status;

        if (rc = MPI_Init(&argc, &argv))
        {
            cout << "Ошибка запуска, выполнение остановлено " << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        int** a = new int* [N];
        int* b = new int[N];
        int* c = new int[N];
        for (int i = 0; i < N; i++) a[i] = new int[N];

        srand(time(NULL));
        MPI_Type_vector(N / 2, N, 2 * N, MPI_INT, &newRowType);
        MPI_Type_commit(&newRowType);

        if (rank == 0) {
            cout << " MPI - TASK 5.1 \n\n";

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    a[i][j] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
                }
                b[i] = 0;
                c[i] = 0;
            }

            cout << "matrix A:" << endl;
            show_matrix(a, N, N);
            MPI_Send(a, 1, newRowType, 1, 123, MPI_COMM_WORLD);
        }
        else
        {
            if (rank % 2 == 0) {
                MPI_Recv(c, 1, newRowType, 0, 123, MPI_COMM_WORLD, &status);
                cout << rank << " ";
                show_vec(c, N);
            }
            else
            {
                MPI_Recv(b, 1, newRowType, 0, 123, MPI_COMM_WORLD, &status);
                cout << rank << " ";
                show_vec(b, N);
            }
        }

        MPI_Finalize();
        return MPI_SUCCESS;
    }
}

//Создать новую группу из процессов с номерами 8, 3, 9, 1, 6. Используя конструктор MPI_Comm_create(…), создать коммуникатор группы.На нулевом
//процессе созданной группы объявить и заполнить числовыми значениями одномерный массив вещественных чисел и вывести его на экран.Используя
//функцию MPI_Bcast передать массив всем процессам созданной группы. Полученные массивы вывести на экран.Передать полученный массив из
//последнего процесса новой группы на нулевой процесс исходной группы. Выполнить программу на 10 процессах.
namespace Task2
{
    int main(int argc, char** argv)
    {
        int rc;
        int rank, size;
        int arr[N];
        MPI_Comm comm;
        MPI_Group wgroup, group_1, group_2;
        MPI_Comm my_com_1, my_com_2;
        int* ranks = new int[5]{ 8, 3, 9, 1, 6 };

        if (rc = MPI_Init(&argc, &argv))
        {
            cout << "Ошибка запуска, выполнение остановлено " << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        MPI_Comm_group(MPI_COMM_WORLD, &wgroup);
        MPI_Group_incl(wgroup, process, ranks, &group_1);
        MPI_Group_excl(wgroup, process, ranks, &group_2);

        MPI_Comm_create(MPI_COMM_WORLD, group_1, &my_com_1);
        MPI_Comm_create(MPI_COMM_WORLD, group_2, &my_com_2);

        if (my_com_1 != MPI_COMM_NULL) {
            MPI_Comm_rank(my_com_1, &rank);
        }
        if (my_com_2 != MPI_COMM_NULL) {
            MPI_Comm_rank(my_com_2, &rank);
        }

        if (my_com_1 != MPI_COMM_NULL && rank == 0) {
            cout << "MPI - TASK 5.3 \n\n";

            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dist(0, 100);

            printf("main array: ");
            for (int i = 0; i < N; i++) {
                arr[i] = dist(gen);
                printf("%d ", arr[i]);
            }
            printf("\n\n");
        }

        if (my_com_1 != MPI_COMM_NULL) {
            MPI_Bcast(&arr[0], N, MPI_INT, 0, my_com_1);

            printf("process id: %d\n", rank);
            for (int i = 0; i < N; i++) {
                cout << arr[i] << " ";
            }
            printf("\n\n");
        }

        if (my_com_1 != MPI_COMM_NULL && rank == process - 1) {
            MPI_Send(&arr[0], N, MPI_INT, 0, 1, my_com_1);
        }

        if (my_com_1 != MPI_COMM_NULL && rank == 0) {
            int b[N];

            MPI_Recv(&b[0], N, MPI_INT, process - 1, 1, my_com_1, MPI_STATUSES_IGNORE);

            printf("received array:\n");
            for (int i = 0; i < N; i++) {
                printf("%d ", b[i]);
            }
            cout << "\n\n";
        }

        if (my_com_1 != MPI_COMM_NULL) {
            MPI_Comm_free(&my_com_1);
        }
        if (my_com_2 != MPI_COMM_NULL) {
            MPI_Comm_free(&my_com_2);
        }

        MPI_Group_free(&group_1);
        MPI_Group_free(&group_2);

        MPI_Finalize();
        return MPI_SUCCESS;
    }
}

int main(int argc, char* argv[])
{
    Task2::main(argc, argv);
}

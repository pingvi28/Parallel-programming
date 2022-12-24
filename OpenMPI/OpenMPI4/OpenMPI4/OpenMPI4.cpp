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

int const ARR_SIZE = 12;
const int MIN_VALUE = -10; // минимальное значение элементов матрицы
const int MAX_VALUE = 10; // максимальное значение

typedef unsigned char byte_t;


const int MESSAGE_LENGTH = 1048576; 
const int BENCHMARK_RUNS = 10000;
const int MAIN_RANK = 0;


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

struct BenchmarkResult {
    double average_time;
    unsigned int runs;
    double total_time;
};


// Написать программу и запустить ее на p (= 2, 3, 4, 6, 8) процессах. На нулевом  процессе объявить и задать массив из 12 элементов.С помощью функции
// MPI_Send разослать блоки массива на остальные процессы.Размер блока массива(12 / p) + 1. В результате на нулевом процессе должны быть элементы
//массива с 0 до 12 / p, на первом процессе с 12 / p + 1 до 2×(12 / p), на 3м процессе с 2×(12 / p) + 1 до 3×(12 / p) и т.д.Вывести элементы массива на экран на каждом
// процессе.Обратите внимание, что не на все процессы хватит элементов.

namespace Task4
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
        int block = ARR_SIZE;

        //вектор Х, который является потоки * блок
        int* x = new int[ARR_SIZE];

        srand(time(NULL));

        // первый поток послал значения
        if (rank == 0) {
            cout << " MPI - TASK 4 \n\n";

            for (int i = 0; i < ARR_SIZE; ++i) x[i] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);

            cout << "x : ";
            show_vec(x, ARR_SIZE);
            cout << "\n";

            for (int q = 1; q < size; q++) {
                MPI_Send(&x, block, MPI_INT, q, 11000, MPI_COMM_WORLD);
            }

            for (int q = 0; q < ARR_SIZE / size; q++) {
                cout << " rank: " << rank << " => " << x[q] + "\n";
            }
        }
        else {
            int arr2[ARR_SIZE];
            MPI_Status st;

            cout << "\n";
            MPI_Recv(arr2, ARR_SIZE, MPI_INT, 0, 11000, MPI_COMM_WORLD, &st);
            try {
                for (int i = rank * ARR_SIZE / size; i < (rank + 1) * (ARR_SIZE / size); i++)
                {
                    cout << " rank: " << rank << " => " << arr2[i] + "\n";
                }
            }
            catch (const char* msg) {}
        }


        MPI_Finalize();

        return MPI_SUCCESS;
    }
}

int benchmark(MPI_Comm global_comm, int master_rank, bool f(MPI_Comm),
    unsigned int runs, BenchmarkResult& result) {
    int global_rank;
    MPI_Comm_rank(global_comm, &global_rank);

    int global_size;
    MPI_Comm_size(global_comm, &global_size);

    if (global_size < 2) {
        cerr << "Cannot run a benchmark with a single process!" << endl;
        return 1;
    }

    MPI_Comm slave_comm;

    const int color = global_rank != master_rank ? 0 : MPI_UNDEFINED;
    MPI_Comm_split(global_comm, color, global_rank, &slave_comm);

    int slave_comm_size = global_size - 1;

    double total_time = 0;
    double* total_times = nullptr;

    if (global_rank != master_rank) {
        for (int i = 0; i < runs; i++) {
            MPI_Barrier(slave_comm);
            auto start = chrono::high_resolution_clock::now();

            bool include = f(slave_comm);

            MPI_Barrier(slave_comm);
            auto end = chrono::high_resolution_clock::now();

            if (include) {
                auto delta_ns = chrono::duration_cast<chrono::nanoseconds>(end - start);
                total_time += delta_ns.count() / 1000000000.0;
            }
        }
    }

    if (global_rank == master_rank) {
        total_times = new double[slave_comm_size];
    }

    MPI_Gather(&total_time, 1, MPI_DOUBLE, total_times, 1, MPI_DOUBLE, master_rank, global_comm);

    if (global_rank == master_rank) {
        for (int i = 0; i < slave_comm_size; i++) {
            total_time += total_times[i];
        }

        delete[] total_times;

        double avg_time = total_time / runs / slave_comm_size;

        result.total_time = total_time;
        result.runs = runs;
        result.average_time = avg_time;
    }

    return 0;
}

//Разработать программу, в которой два процесса многократно обмениваются сообщениями длиной n байт.Выполнить эксперименты и оценить зависимость
//времени выполнения операции от длины сообщения.Рассмотреть два варианта обмена сообщениями : PingPong - двунаправленная передача : Выполняется
//передача данных от одного процесса другому с последующим возвратом переданных данных в исходный процесс.Время выполнение замеряется после
//получения обратного ответа на исходном процессе.PingPing - однонаправленная передача : Два процесса одновременно выполняют
//передачу данных друг другу.Время выполнение замеряется после получения сообщения каждым процессом
namespace Task41
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

        BenchmarkResult result;

        if (benchmark(MPI_COMM_WORLD, MAIN_RANK, ping_pong, BENCHMARK_RUNS, result)) {
            cerr << "PingPong benchmark failed @ rank " << rank << endl;
            MPI_Finalize();
            return 1;
        }
        if (rank == MAIN_RANK)
            print_benchmark_result("PingPong", MESSAGE_LENGTH, result);

        if (benchmark(MPI_COMM_WORLD, MAIN_RANK, ping_ping, BENCHMARK_RUNS, result)) {
            cerr << "PingPing benchmark failed @ rank " << rank << endl;
            MPI_Finalize();
            return 1;
        }

        if (rank == MAIN_RANK)
            print_benchmark_result("PingPing", MESSAGE_LENGTH, result);

        MPI_Finalize();
        return 0;

    }
}



void print_benchmark_result(string function_identifier, unsigned int message_size, BenchmarkResult& result) {
    cout << function_identifier << ": " << message_size << "-byte message, " << result.runs << " runs, avg. " << result.average_time << "s, "
        << result.total_time << "s total" << endl;
}

bool ping_pong(MPI_Comm comm) {
    int global_rank;
    MPI_Comm_rank(comm, &global_rank);

    int global_size;
    MPI_Comm_size(comm, &global_size);

    if (global_size < 2) {
        cerr << "PingPong: not enough processes! Need at least 2" << endl;
        return false;
    }

    MPI_Comm local_comm;

    int color;
    if (global_rank == 0 || global_rank == 1) {
        color = 0;
    }
    else {
        return false;
    }

    MPI_Comm_split(comm, color, global_rank, &local_comm);

    int local_rank;
    MPI_Comm_rank(local_comm, &local_rank);

    byte_t buffer[MESSAGE_LENGTH];

    if (local_rank == 0) {
        MPI_Send(buffer, MESSAGE_LENGTH, MPI_BYTE, 1, 0, local_comm);
        MPI_Recv(buffer, MESSAGE_LENGTH, MPI_BYTE, 1, 0, local_comm,
            MPI_STATUS_IGNORE);
    }
    else {
        MPI_Recv(buffer, MESSAGE_LENGTH, MPI_BYTE, 0, 0, local_comm,
            MPI_STATUS_IGNORE);
        MPI_Send(buffer, MESSAGE_LENGTH, MPI_BYTE, 0, 0, local_comm);
    }

    return true;
}

bool ping_ping(MPI_Comm comm) {
    int global_rank;
    MPI_Comm_rank(comm, &global_rank);

    int global_size;
    MPI_Comm_size(comm, &global_size);

    if (global_size < 2) {
        cerr << "PingPing: not enough processes! Need at least 2" << endl;
        return false;
    }

    MPI_Comm local_comm;

    int color;
    if (global_rank == 0 || global_rank == 1) {
        color = 0;
    }
    else {
        return false;
    }

    MPI_Comm_split(comm, color, global_rank, &local_comm);

    int local_rank;
    MPI_Comm_rank(local_comm, &local_rank);

    int other_rank = local_rank == 0 ? 1 : 0;

    byte_t send_buffer[MESSAGE_LENGTH];
    byte_t recv_buffer[MESSAGE_LENGTH];

    MPI_Request requests[2];

    MPI_Irecv(recv_buffer, MESSAGE_LENGTH, MPI_BYTE, other_rank, 0, local_comm, &requests[1]);
    MPI_Isend(send_buffer, MESSAGE_LENGTH, MPI_BYTE, other_rank, 0, local_comm, &requests[0]);
    MPI_Waitall(2, requests, MPI_STATUS_IGNORE);

    return true;
}


int main(int argc, char* argv[])
{
    Task4::main(argc, argv);
}

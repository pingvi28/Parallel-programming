// БЛОК I - ЗАДАЧИ 1 -5 

//Athor:Kashapoova Dilyara, 11-001

#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <string>

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

void mult_vec(int* arr, int size, int c)
{
    for (int i = 0; i < size; i++) arr[i] *= c;
}


//Написать mpi программу, печатающую "Hello, World!". Запустить программу на 4 процессах.
namespace Task1
{
    int main(int argc, char* argv[])
    {
        int numtasks = 4, rank;
        int rc;

        // инициализация процесса
        //В результате выполнения этой функции создается группа процессов, 
        //в которую помещаются все процессы приложения, и создается область связи, 
        //описываемая предопределенным коммуникатором MPI_COMM_WORLD
        if (rc = MPI_Init(&argc, &argv))
        {
            cout << "Ошибка запуска, выполнение остановлено " << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        //Функция возвращает номер процесса, вызвавшего эту функцию. Номера процессов лежат в диапазоне 0..size-1
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        //Функция определения числа процессов в области связи 
        MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

        if (rank == 0) {
            cout << "\n MPI - TASK 1 \n\n";
        }

        printf(" Hello MPI from process = %d\n", rank);

        //обрывает связи и убивает процессы;
        MPI_Finalize();

        return MPI_SUCCESS;
    }
}

//Написать программу, печатающую номер процесса и общее количество процессов в коммуникаторе MPI_COMM_WORLD.
namespace Task2
{
    int main(int argc, char* argv[])
    {
        int numtasks, rank;
        int rc;

        if (rc = MPI_Init(&argc, &argv))
        {
            cout << "Ошибка запуска, выполнение остановлено " << endl;
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

        if (rank == 0) {
            cout << "\n MPI - TASK 2 \n\n";
        }

        printf("MPI 2: process = %d, total number of process: %d\n", rank, numtasks);

        MPI_Finalize();
        return 0;
    }
}

//Написать программу, запустить ее на 2-х процессах. На нулевом процессе задать массив а из 10 элементов, значения сгенерировать случайным образом. 
//Переслать этот массив целиком первому процессу с помощью функции MPI_Send. Распечатать на первом процессе принятый массив.
namespace Task3
{
    // int MPI_Send(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
    // 
    // Функция выполняет посылку count элементов типа datatype сообщения с идентификатором tag процессу dest в области связи коммуникатора comm. 
    // Переменная buf - это, как правило, массив или скалярная переменная.
    // 
    //IN buf - адрес начала расположения пересылаемых данных;
    //IN count - число пересылаемых элементов;
    //IN datatype - тип посылаемых элементов;
    //IN dest - номер процесса - получателя в группе, связанной с коммуникатором comm;
    //IN tag - идентификатор сообщения(аналог типа сообщения функций nread и nwrite PSE nCUBE2);
    //IN comm - коммуникатор области связи.

    //int MPI_Recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status* status)
    //
    // Функция выполняет прием count элементов типа datatype сообщения с идентификатором tag от процесса source в области связи коммуникатора comm.
    // 
    //OUT	buf - адрес начала расположения принимаемого сообщения;
    //IN	count - максимальное число принимаемых элементов;
    //IN	datatype - тип элементов принимаемого сообщения;
    //IN	source - номер процесса - отправителя;
    //IN	tag - идентификатор сообщения;
    //IN	comm - коммуникатор области связи;
    //OUT	status - атрибуты принятого сообщения.

    int main(int argc, char** argv)
    {
        int arr[ARR_SIZE];
        int rank, size;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        srand(time(NULL));

        if (rank == 0) {
            cout << " MPI - TASK 3 \n\n";
            cout << " array before MPI_Send\n";

            for (int j = 0; j < ARR_SIZE; ++j)
                arr[j] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);

            show_arr(arr, ARR_SIZE);

            //передали массив
            MPI_Send(arr, ARR_SIZE, MPI_INT, 1, 1000, MPI_COMM_WORLD);
        }
        else
        {
            int arr2[ARR_SIZE];
            MPI_Status st;

            MPI_Recv(arr2, ARR_SIZE, MPI_INT, 0, 1000, MPI_COMM_WORLD, &st);

            cout << "\n array after MPI_Send\n";
            show_arr(arr2, ARR_SIZE);
            cout << "\n";
        }

        MPI_Finalize();

        return MPI_SUCCESS;
    }
}

//4. Используя блокирующую операцию передачи сообщений (MPI_Send и MPI_Recv) выполнить пересылку данных одномерного 
//массива, из процесса с номером 1 на остальные процессы группы. На процессах получателях до выставления функции
//приема (MPI_Recv) определить количество принятых данных (MPI_Probe). Выделить память под размер приемного буфера, 
//после чего вызвать функцию MPI_Recv. Полученное сообщение выдать на экран.
namespace Task4
{
    int main(int argc, char** argv)
    {
        int rank, size, count;
        int arr[ARR_SIZE];
        MPI_Status status;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        if (rank == 0) {
            cout << " MPI - TASK 4 \n\n";

            for (int j = 0; j < ARR_SIZE; ++j)
                arr[j] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);

            show_arr(arr, ARR_SIZE);
            cout << "\n\n";

            for (int i = 1; i < size; i++) MPI_Send(&arr, i, MPI_INT, i, 11001, MPI_COMM_WORLD);
        }
        else
        {
            //Она позволяет получить информацию о сообщении которое ожидает в очереди на прием не получая самого сообщения
            // 1 цифра - от кого
            MPI_Probe(0, 11001, MPI_COMM_WORLD, &status);
            //По структуре status процедура определяет сколько данных типа datatype передано соответствующим сообщением и записывает результат по адресу count.
            MPI_Get_count(&status, MPI_INT, &count);

            MPI_Recv(&arr, count, MPI_INT, 0, 11001, MPI_COMM_WORLD, &status);

            cout << "Process:" << rank << " || Count: " << count << " || Array: ";
            show_arr(arr, count);
        }

        MPI_Finalize();

        return MPI_SUCCESS;
    }
}

//5. Написать программу, вычисляющую элементы вектора Z по формуле zi = axi + byi. 
//Векторы задаются на нулевом процессе и равными блоками пересылаются остальным процессам, заданные числа.
//Пересылка данных осуществляется функцией MPI_Send.Все процессы по формуле вычисляют свои элементы массива 
//Каждый процесс отправляет на нулевой процесс подсчитанные элементы.
namespace Task5
{
    void main(int argc, char** argv) {
        int rank, size;
        int a = 2, b = 1;
        MPI_Status mpi_stat;

        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        // убираем root поток
        size--;
        // принимает только int == найдем сколько нужно отправлять
        int countSendElements = ARR_SIZE / size;

        srand(time(NULL));

        // главный поток == root
        if (rank == 0) {
            cout << " MPI - TASK 5 \n\n";
            cout << "a = " << a << ", b = " << b << "\n\n";

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

            cout << "z = axi + byi\n\nz = ";
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

            mult_vec(x, buf_size, a);
            mult_vec(y, buf_size, b);

            for (int i = 0; i < buf_size; ++i) y[i] += x[i];
            //cout << "z" << rank << " ";
            //show_vec(y, buf_size);
            MPI_Send(&y[0], buf_size, MPI_INT, 0, 203023024, MPI_COMM_WORLD);
        }
    finish_process:MPI_Finalize();
    }
}

int main(int argc, char* argv[])
{
    Task1::main(argc, argv);
}

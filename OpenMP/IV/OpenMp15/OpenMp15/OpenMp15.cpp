//Написать программу, в которой, используя возможности OpenMP найти все простые числа в заданном с клавиатуры диапазоне.
//Количество потоков определить самостоятельно. Результат выдать на экран.

// athor: Kashapova Dilyara, 11-001

#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <windows.h>
#include <valarray>

using namespace std;

int thread_count;
const int ARR_SiZE = 30;
const int range_shift = 10;

int main() {
    int const number = 210;
    int res = 0;
    int start_range, end_range = 0;
    vector<int> result;

    cout << "Enter start range:" << endl;
    cin >> start_range;

    cout << "Enter end range:" << endl;
    cin >> end_range;

    if (end_range < start_range) {
        end_range = start_range + range_shift;
    }

    cout << "\nEnter thread count: ";
    try
    {
        cin >> thread_count;
    }
    catch (const std::exception&)
    {
        thread_count = 4;
    }

    omp_set_dynamic(0);
    omp_set_num_threads(thread_count);


    bool *vizited = new bool[end_range + 1];

    for (size_t i = 0; i <= end_range; i++)
    {
        vizited[i] = false;
    }

    for (int i = 2; i <= end_range; ++i) {
        if (!vizited[i]) {
            if (i >= start_range) {
                result.push_back(i);
            }

#pragma omp parallel for
            for (int j = 2 * i; j <= end_range; j += i) {
                vizited[j] = true;
            }
        }
    }

    printf("\nRange start = %d, range end = %d\n\nResult:\n", start_range, end_range);

    for (int x : result) {
        printf("%d ", x);
    }
    printf("\n");

    return 0;
}

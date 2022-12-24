//На нулевом процессе задан массив целых чисел, который является двоичным представлением десятичного числа. 
//Написать параллельную программу вычисления десятичного числа из заданного двоичного представления.
//int a[30] = { 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1 };     //756893125
//Число x может быть представлено в системе с основанием p, как x = an * pn + an - 1 * pn - 1 + a1 * p1 + a0 * p0, где an ... a0 - цифры в представлении данного числа.

// athor: Kashapova Dilyara, 11-001

#include <iostream>
#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <string> 

using namespace std;

int thread_count;
const int ARR_SiZE = 30;

int main() {
    int a[ARR_SiZE] = { 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1 };
    double res = 0;

    string bin_number = "";
    for (size_t i = 0; i < ARR_SiZE; i++)
    {
        bin_number += to_string(a[i]);
    }

    omp_set_dynamic(0);
    
    cout << "Enter thread count: ";
    try
    {
        cin >> thread_count;
    }
    catch (const std::exception&)
    {
        thread_count = 3;
    }

    omp_set_num_threads(thread_count);
#pragma omp parallel for reduction(+:res)
    for (int i = 0; i < ARR_SiZE; i++) {
        if (a[i] == 1) {
            res = res + a[i] * pow(2, ARR_SiZE - i - 1);
        }
    }

    cout << "\nnumber in bn = " << bin_number;
    printf("\nresult (in decimal) = %d\n", (int)res);

    return 0;
}

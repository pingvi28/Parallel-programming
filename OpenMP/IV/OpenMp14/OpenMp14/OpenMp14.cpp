//Написать параллельную программу возведения числа 210 в квадрат без операции умножения. //44100
//Пояснение: Квадрат натурального числа N равен сумме первых N нечетных чисел.Например, 32 = 9 это 1 + 3 + 5 = 9;  52 = 25 это 1 + 3 + 5 + 7 + 9 = 25;

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
    int const number = 210;
    int res = 0;

    omp_set_dynamic(0);

    cout << "Enter thread count: ";
    try
    {
        cin >> thread_count;
    }
    catch (const std::exception&)
    {
        thread_count = 4;
    }

    omp_set_num_threads(thread_count);
#pragma omp parallel for reduction(+:res)
    for (int i = 0; i < number; i++) {
        res += 2 * i + 1;
    }

    printf("\nresult = %d\n", res);

    return 0;
}

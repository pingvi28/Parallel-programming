// Oбъявить и присвоить начальные значения массиву целых чисел a[30], для инициализации значений использовать генератор случайных чисел. 
//Используя конструкцию omp parallel for и omp atomic вычислить количество элементов массива, числовые значения которых кратны 9. 
//Количество нитей задать самостоятельно. Результат выдать на экран.

// author : Kashapova Dilyara, 11-001

#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

using namespace std;

const int MIN_VALUE = -20; // минимальное значение
const int MAX_VALUE = 30; // максимальное значение элементов матрицы

int main() {
	int threadCount;
	int n = 30;
	int max = MIN_VALUE - 1, min = MAX_VALUE + 1;
	int* a = new int [n];

	srand(time(NULL));

	for (int i = 0; i < n; ++i)
	{
		a[i] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);

		//для проверки корректности
		if (a[i] % 9 == 0) cout << "[" << a[i] << "]" << " ";
		else cout << a[i] << " ";
	}

	cout << "\n\nEnter thread count: ";
	try
	{
		cin >> threadCount;
	}
	catch (const std::exception&)
	{
		threadCount = 3;
	}

	int count9 = 0;
#pragma omp parallel for num_threads(threadCount)
	for (int i = 0; i < n; i++)
	{
			if (a[i] % 9 == 0)
			{
			#pragma omp atomic
				count9++;
			}

		if (i == 0) printf("\nthread count: %d", omp_get_num_threads());
	}

	printf("\ncount: %d", count9);
}

// Oбъявить и присвоить начальные значения элементам двумерного массива d[6][8], для инициализации значений использовать генератор случайных чисел. 
// Используя конструкцию директивы omp parallel for и omp critical определить минимальное и максимальное значения элементов двумерного массива. 
// Количество нитей задать самостоятельно. Результат выдать на экран.

// author : Kashapova Dilyara, 11-001

#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

using namespace std;

const int MIN_VALUE = -20; // минимальное значение
const int MAX_VALUE = 30; // максимальное значение элементов матрицы

void fill_matrix(int** a, int n, int m) {
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			a[i][j] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
			cout << a[i][j] << " ";
		}
		cout << "\n";
	}
}

int main() {
	int threadCount;
	int n = 6, m = 8;
	int max = MIN_VALUE - 1, min = MAX_VALUE + 1;
	int** d = new int* [n];

	for (int i = 0; i < n; i++)
		d[i] = new int[m];

	srand(time(NULL));
	fill_matrix(d, n, m);

	cout << "Enter thread count: ";
	try
	{
		cin >> threadCount;
	}
	catch (const std::exception&)
	{
		threadCount = 3;
	}


#pragma omp parallel for num_threads(threadCount)
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (d[i][j] > max)
			#pragma omp critical
			{
				max = d[i][j];
			}

			if (d[i][j] < min)
			#pragma omp critical
			{
				min = d[i][j];
			}

		}

		if (i == 0) printf("\nthread cout: %d", omp_get_num_threads());
	}

	printf("\nminimum: %d \nmaximum: %d", min, max);
}



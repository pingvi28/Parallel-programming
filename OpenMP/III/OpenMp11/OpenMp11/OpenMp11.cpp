//Написать программу, в которой, объявить и заполнить случайными значениями массив целых чисел. 
//Используя возможности OpenMP найти максимальное числовое значение элементов массива, кратное 7. 
//Длину массива и количество потоков определить самостоятельно. Результат выдать на экран. 
//Для синхронизации числовых значений максимума используется механизм критических секций.

// author : Kashapova Dilyara, 11-001

#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

using namespace std;

const int MIN_VALUE = -20; // минимальное значение
const int MAX_VALUE = 30; // максимальное значение элементов матрицы
int checkMax = MIN_VALUE - 1;

int main() {
	int threadCount;
	int n = 30;
	int max7 = MIN_VALUE - 1;
	int* a = new int[n];

	srand(time(NULL));

	for (int i = 0; i < n; ++i)
	{
		a[i] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);

		//для проверки корректности
		if (a[i] % 7 == 0) cout << "[" << a[i] << "]" << " ";
		else cout << a[i] << " ";

		if (a[i] % 7 == 0 && checkMax < a[i]) {
			{
				checkMax = a[i];
			}
		}
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

#pragma omp parallel for shared(max)
	for (int i = 0; i < n; i++) {
		if (a[i] % 7 == 0 && max7 < a[i]) {
		#pragma omp critical
			{
				max7 = a[i];
			}
		}
		if (i == 0) printf("\nthread count: %d", omp_get_num_threads());
	}

	printf("\nmax % 7: %d", max7);

	if (max7 == (MIN_VALUE - 1)) {
		cout << "\ndon't find number % 7 = 0";
	}
	else {
		if (checkMax == max7) {
			printf("\ncorrect\n");
		}
	}
}

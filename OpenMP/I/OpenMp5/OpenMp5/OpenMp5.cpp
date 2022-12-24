//Oбъявить и присвоить начальные значения элементам двумерного массива d[6][8], для инициализации значений использовать генератор случайных чисел. 
//Используя конструкцию директивы sections…section определить три секции для выполнения следующих операций:
//	первая секция выполняет вычисление среднего арифметического значения элементов двумерного массива,
//	вторая секция выполняет вычисление минимального и максимального значений элементов двумерного массива,
//	третья секция выполняет вычисление количества элементов массива, числовые значения которых кратны 3.
//В каждой секции определить и выдать на экран номер исполняющей нити и результат выполнения вычислений.

// author : Kashapova Dilyara, 11-001

#include <omp.h>
#include <stdio.h>
#include <iostream>


int main() {
	const int MIN_VALUE = -200; // максимальное значение элементов матрицы
	const int MAX_VALUE = 100; // минимальное значение
	int d[6][8];

	omp_set_num_threads(3);

	for (int i = 0; i < 6; ++i)
	{
		srand(time(NULL));
		for (int j = 0; j < 8; ++j)
		{
			d[i][j] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
			//printf("%d ", d[i][j]);
		}
	}

#pragma omp parallel sections 
	{
#pragma omp section
		{
			double average = 0;
			int elementsInArray = 0;

			for (int i = 0; i < 6; ++i)
			{
				for (int j = 0; j < 8; ++j)
				{
					average += d[i][j];
					elementsInArray += 1;
				}
			}
			printf("thread id = %d, 	Average %.3f\n", omp_get_thread_num(), average/elementsInArray);
		}

#pragma omp section
		{
			int max = MIN_VALUE - 1, min = MAX_VALUE + 1;

			for (int i = 0; i < 6; ++i)
			{
				for (int j = 0; j < 8; ++j)
				{
					if (d[i][j] < min)
					{
						min = d[i][j];
					}

					if (d[i][j] > max)
					{
						max = d[i][j];
					}
				}
			}

			printf("\nthread id = %d,	Min = %d, Max = %d\n", omp_get_thread_num(), min, max);
		}

#pragma omp section
		{
			int count3 = 0;

			for (int i = 0; i < 6; ++i)
			{
				for (int j = 0; j < 8; ++j)
				{
					if (d[i][j] % 3 == 0)
					{
						count3++;
					}
				}
			}

			printf("\nthread id = %d,	Count numbers of multiples of 3 = %d\n", omp_get_thread_num(), count3);
		}
	}
	std::cout << "\n";
	return 0;
}

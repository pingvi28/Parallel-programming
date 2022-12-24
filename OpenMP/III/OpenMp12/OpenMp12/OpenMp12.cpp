﻿//Написать программу где каждый поток печатает свой идентификатор, количество потоков всего и строчку «Hello World». 
//Запустить программу с 8 потоками. 

//Модифицируйте задачу выше так, что бы потоки распечатывали свои идентификаторы в обратном порядке. 
//Существует как минимум 5 способов решения. Постарайтесь найти как можно больше.

// author : Kashapova Dilyara, 11-001

#include <omp.h>
#include <stdio.h>

int num = 8;

int main() {
	// объявляем область
#pragma omp parallel num_threads(num)
	{
		for (int i = omp_get_num_threads(); i > 0; i--)
		{
		#pragma omp barrier
			{
				if (i == omp_get_thread_num())
				{
				#pragma omp critical
					printf("\"Hello World\" printed from thread %i\n", omp_get_thread_num());

				}
			}
		}
	}
}

//Директива omp barrier определяет точку синхронизации, в которой потоки в параллельном регионе будут ждать, пока все остальные потоки в этом разделе не достигнут той же точки.
//Выполнение оператора за точкой omp barrier затем продолжается параллельно.

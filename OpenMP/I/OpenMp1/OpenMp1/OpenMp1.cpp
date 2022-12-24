//Написать программу где каждый поток печатает свой идентификатор, количество потоков всего и строчку «Hello World». 
//Запустить программу с 8 потоками. 
//Всегда ли вывод идентичен? Почему? (нет, потоки работают хаотично, независимо друг от друга == любая последовательность)
// omp_get_num_threads - Возвращает число потоков в параллельной области.

// author : Kashapova Dilyara, 11-001

#include <omp.h>
#include <stdio.h>

int num = 8;

int main() {
	// объявляем область
#pragma omp parallel num_threads(num)
	printf("\"Hello World\" printed from thread %i\n", omp_get_thread_num());

	return 0;
}

//Jпределить две параллельные области, выполнение которых зависит от условного оператора #pragma omp parallel if(…)
//если заданное значение числа нитей больше 1, параллельная область выполняется, иначе не выполняется. 
//Число нитей перед первой областью задать равным 3, перед второй – равным 1. 
//Внутри параллельных областей определить количество нитей и номер каждой нити, результат выдать на экран. Убедиться в правильности работы программы.

// author : Kashapova Dilyara, 11-001
// 
//current thread 0, all threads 1

#include <omp.h>
#include <stdio.h>

int a = 3, b = 1;
int main() {
	// Функция включает или отключает динамическую настройку количества потоков, доступных для выполнения параллельных регионов.
	// 0 - набор потоков можно было повторно, 1 - удаляет и перерсоздает
	omp_set_dynamic(0);

	omp_set_num_threads(a);

#pragma omp parallel if(omp_get_max_threads() > 1)
	printf("First region: current thread %d, all threads %d\n", omp_get_thread_num(), omp_get_num_threads());

	omp_set_num_threads(b);

#pragma omp parallel if(omp_get_max_threads() > 1)
	printf("Second region: current thread %d, all threads %d\n", omp_get_thread_num(), omp_get_num_threads());

	return 0;
}

/*
* второй вариант реализации в main
* 
* 
	int count_th_num = 2;

#pragma omp parallel num_threads(count_th_num) if(count_th_num > 1)
	printf("current thread %d, all threads %d\n", omp_get_thread_num(), omp_get_num_threads());
*/

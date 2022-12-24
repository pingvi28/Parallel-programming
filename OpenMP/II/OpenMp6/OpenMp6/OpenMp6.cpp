//Объявить и присвоить начальные значения целочисленным массивам a[10] и b[10]. 
//Используя конструкцию parallel for и reduction вычислить средние арифметические значения элементов массивов a и b
//сравнить полученные значения, результат выдать на экран.

// author : Kashapova Dilyara, 11-001

#include <omp.h>
#include <stdio.h>

//Директива OpenMP reduction позволяет собрать вместе в главном потоке результаты вычислений частичных сумм, разностей и т. п. 
//из параллельных потоков последующего параллельного структурного блока

int main() {
	int a[10] = { -1, 3, -6, -12, 4, 5, 6, 8, 3, 11 }; //21
	int b[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; //55

	int sum_ar_a = 0, sum_ar_b = 0;

#pragma omp parallel for reduction(+:sum_ar_a, sum_ar_b)
	for (int i = 0; i < 10; i++) {
		sum_ar_a += a[i];
		sum_ar_b += b[i];
	}

	printf("The sum of the elements a: %d\n The sum of the elements b: %d\n", sum_ar_a, sum_ar_b);
}

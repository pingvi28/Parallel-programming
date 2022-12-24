//Oбъявить и присвоить начальные значения целочисленным переменным: a и b, определить две параллельные области
//первая – с числом нитей 2, вторая – с числом нитей 4. 
//Для первой области переменные a и b объявить защищенными, с режимом доступа private и firstprivate соответственно
//внутри области определить номер нити и выполнить увеличение значения переменных на это величину. 
//Выдать на экран значения переменных до входа в параллельную область, внутри параллельной области и после выхода из параллельной области.  
//Для второй области переменную a объявить общей, переменную b объявить защищенной, с режимом доступа private,
//определить номер нити и выполнить уменьшение значения переменных на это величину. 
//Выдать на экран значения переменных до входа в параллельную область, внутри параллельной области и после выхода из параллельной области.

// author : Kashapova Dilyara, 11-001

/* вывод программы
First region: A before 0, B before 0
First region: A after 0, B after 0

Second region: A before 0, B before 0
Second region: A after -6, B after 0
*/

#include <omp.h>
#include <stdio.h>
#include <iostream>


int main() {
	int a = 0, b = 0;

	omp_set_dynamic(0);

	omp_set_num_threads(2);

	printf("First region: A before %d, B before %d\n\n",  a, b);

#pragma omp parallel private(a) firstprivate(b) 
	{
	#pragma omp critical
		{
			a += omp_get_thread_num();
			b += omp_get_thread_num();
			printf("	A = %d, thread count %d\n", a, omp_get_thread_num());
			printf("	B = %d, thread count %d\n", b, omp_get_thread_num());
		}
	}
	printf("\nFirst region: A after %d, B after %d\n", a, b);

	std::cout << "\n";

	omp_set_num_threads(4);

	printf("Second region: A before %d, B before %d\n\n", a, b);
#pragma omp parallel shared(a) private(b) 
	{
#pragma omp critical
		{
			a -= omp_get_thread_num();
			b -= omp_get_thread_num();
			printf("	A = %d, thread count %d\n", a, omp_get_thread_num());
			printf("	B = %d, thread count %d\n", b, omp_get_thread_num());
		}
	}
	printf("\nSecond region: A after %d, B after %d\n", a, b);

	return 0;
}

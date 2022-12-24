// Используя возможности OpenMP, написать программу умножения матрицы на вектор.Сравнить время выполнения последовательной и параллельных программ.

// author : Kashapova Dilyara, 11-001

// на малентких матрица быстрее последовательная, на больших параллельная

#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

using namespace std;

const int MIN_VALUE = 0; // максимальное значение элементов матрицы
const int MAX_VALUE = 5; // минимальное значение

void fill_matrix(int** a, int n, int m) {
	//cout << "matrix:\n";
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			a[i][j] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
			//cout << a[i][j];
		}
		//cout << "\n";
	}
}

void fill_vector(int* a, int m) {
	//cout << "\nvector:\n";
	for (int j = 0; j < m; ++j)
	{
		a[j] = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);
		//cout << a[j] << "\n";
	}
}

void multiply_matrix(int** a, int* b, int*c, int n, int m) {
	int count;

	for (int i = 0; i < n; ++i)
	{
		count = 0;
		for (int j = 0; j < m; ++j)
		{
			count += a[i][j] * b[j];
		}
		c[i] = count;
	}
}

void print_result(int* c,int n) {
	for (int j = 0; j < n; ++j)
	{
		cout << c[j] << "\n";
	}
}

int main() {
	int n, m;
	double time_a, time_b;

	cout << "Enter n: ";
	cin >> n;
	cout << "Enter m: ";
	cin >> m;
	cout << endl;

	int** a = new int* [n];
	for (int i = 0; i < n; i++)
		a[i] = new int[m];

	int* b = new int[m];
	int* c = new int[m];

	srand(time(NULL));

	fill_matrix(a, n, m);

	fill_vector(b, m);

	auto begin = chrono::high_resolution_clock::now();
	multiply_matrix(a, b, c, n, m);
	auto end = chrono::high_resolution_clock::now();
	cout << "time a (NO PARALLEL): " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << " ns" << endl;

	//print_result(final_vect, n);

	begin = chrono::high_resolution_clock::now();
#pragma omp parallel
	{
		int count;
#pragma omp for schedule(static)
		for (int i = 0; i < n; ++i)
		{
			count = 0;
			for (int j = 0; j < m; ++j)
			{
				count += a[i][j] * b[j];
			}
			c[i] = count;
		}
	}

	end = chrono::high_resolution_clock::now();
	cout << "time b (PARALLEL): " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << " ns" << endl;
}

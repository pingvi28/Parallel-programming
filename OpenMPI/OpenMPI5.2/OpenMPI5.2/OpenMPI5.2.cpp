#include <iostream>
#include<math.h> 
#include <ctime>
#include <chrono>
#include <time.h> 

using namespace std;

const double R = 1;
const int countPoints = 52428800;
const int MIN = -1;
const int MAX = 1;

bool inCircle(double x, double y, double radius);

bool inCircle(double x, double y, double radius) {

    return radius * radius >= x * x + y * y;
}

int main()
{
    //auto begin = chrono::high_resolution_clock::now();
    clock_t begin = clock();
    int rank = 0, size = 1;


    srand((unsigned)time(NULL) + rank);

    int iterat = countPoints / size;
    if (countPoints % size != 0 && size - 1 == rank) {
        iterat = countPoints - (size - 1) * iterat;
    }

    int countInCircle = 0;
    for (int i = 0; i < iterat; i++) {
        double x = MIN + (double)(rand()) / ((double)(RAND_MAX / (MAX - MIN)));
        double y = MIN + (double)(rand()) / ((double)(RAND_MAX / (MAX - MIN)));
        if (inCircle(x, y, R)) {
            countInCircle++;
        }
    }

    printf("\nPI = %f\n", (double)countInCircle * 4 / countPoints);
    clock_t end = clock();
    printf("The elapsed time is %f seconds", (double)(end - begin) / CLOCKS_PER_SEC);
    //auto end = chrono::high_resolution_clock::now();
    //cout << "time a (NO PARALLEL): " << chrono::duration_cast<chrono::nanoseconds>(end - begin).count() << " ns" << endl;
}

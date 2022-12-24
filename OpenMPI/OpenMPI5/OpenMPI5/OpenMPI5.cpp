#include <iostream>
#include <mpi.h>
#include<math.h> 
#include <ctime>

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
    double start, end;
    int rank, size;
    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
    start = MPI_Wtime();

    srand((unsigned)time(NULL) + rank);

    int iterat = countPoints / size;
    //последнему оставшееся
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

    //printf("The proccess - %d of %d, count of points in circle = %d", rank, size, countInCircle);

    int totalCountPoints = 0;
    MPI_Reduce(&countInCircle, &totalCountPoints, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (!rank) {
        printf("\nPI = %f\n", (double)totalCountPoints * 4 / countPoints);
    }

    MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
    end = MPI_Wtime();

    if (rank == 0) {
        printf("Elapsed time is %f second\n", end - start);
    }

    MPI_Finalize();
}



#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void matvec(double **matrix, double *vector, double *result, int rows, int cols)
{
    // start timer
    clock_t start = clock();
    for (int i = 0; i < rows; i++)
    {
        result[i] = 0.0;
        for (int j = 0; j < cols; j++)
        {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time spent without MPI version : %f\n", time_spent);
}

int main()
{
    
    int rank, size;
    // must ALWAYS be called to run an MPI program
    MPI_Init(&argc, &argv);
    // get process rank id
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // get total number of processes in communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows = 10000; // Example size
    int cols = 10000;

    // Allocate and initialize matrix and vectors
    double **matrix = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (double *)malloc(cols * sizeof(double));
    }
    // Initialize matrix with some values
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = 2.0;
        }
    }

    double *vector = (double *)malloc(cols * sizeof(double));
    // Initialize vector with some values
    for (int i = 0; i < cols; i++)
    {
        vector[i] = 3.0;
    }

    double *result = (double *)malloc(rows * sizeof(double));
#ifdef MPI
    for (int i = 1; i < size; i++)
    {
            MPI_Send(&matrix
    }
}
else
{
}
#else
    matvec(matrix, vector, result, rows, cols);
#endif
// for (int i = 0; i < 10; i++) {
//     printf("%f ", result[i]);
// }
double sumResult = 0.0;
for (int i = 0; i < rows; i++)
{
    sumResult += result[i];
}
printf("Sum result: %f\n", sumResult);

// Free memory
for (int i = 0; i < rows; i++)
{
    free(matrix[i]);
}
free(matrix);
free(vector);
free(result);

return 0;
}

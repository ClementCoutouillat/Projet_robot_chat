#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void matvec(int **matrix, int *vector, int *result, int rowsPerProcess, int cols, int rank)
{
    // start timer
    clock_t start = clock();
    for (int i = rowsPerProcess * rank; i < rowsPerProcess * (rank + 1); i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("rank %d: time spent = %f\n", rank, time_spent);
}

void matvec_mpi(int **matrix, int *vector, int *result, int rows, int cols, int rank, int size)
{

    int rowsPerProcess = rows / size; // 10000/4 = 2500
    printf

    // cols = 10000
    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
        {
            MPI_Send(*matrix[rowsPerProcess * i], rowsPerProcess * cols, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(vector, cols, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        matvec(matrix, vector, result, rowsPerProcess, cols, rank);
    }
    else
    {
        MPI_Recv(matrix[rowsPerProcess * rank], rowsPerProcess * cols, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(vector, cols, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        matvec(matrix, vector, result, rowsPerProcess, cols, rank);
        // for(int i = 0; i < 10; i++)
        // {
        //     printf("matrix[%d][%d] = %d\n", rowsPerProcess * rank, i, matrix[rowsPerProcess * rank][i]);
        //     printf("vector[%d] = %d\n", i, vector[i]);
        // }
    }
}

int main(int argc, char *argv[])
{
    int rank, size;
    // must ALWAYS be called to run an MPI program
    MPI_Init(&argc, &argv);
    // get process rank id
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // get total number of processes in communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int rows = 100; // Example size
    int cols = 100;
    // Allocate and initialize matrix and vectors
    int **matrix = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }

    int *vector = malloc(cols * sizeof(int));
    int *result = malloc(rows * sizeof(int));

    if (rank == 0)
    {
        // Initialize matrix with some values
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                matrix[i][j] = 2;
            }
        }

        // Initialize vector with some values
        for (int i = 0; i < cols; i++)
        {
            vector[i] = 3;
        }
        for (int i = 0; i < rows; i++)
        {
            result[i] = 0;
        }
    }
    else
    {
        // Initialize matrix with some values
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                matrix[i][j] = 0;
            }
        }

        // Initialize vector with some values
        for (int i = 0; i < cols; i++)
        {
            vector[i] = 0;
        }
        for (int i = 0; i < rows; i++)
        {
            result[i] = 0;
        }
    }

    matvec_mpi(matrix, vector, result, rows, cols, rank, size);

    // wait for all processes to finish
    int sumResult = 0.0;
    for (int i = 0; i < rows; i++)
    {
        sumResult += result[i];
    }
    printf("rank %d: sumResult = %d\n", rank, sumResult);
    // Free memory
    for (int i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(result);
    MPI_Finalize();

    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "mpi.h"

#define N 1000000000

int main(int argc, char **argv)
{
    int procid, nproc, j;
    unsigned long long int startval, endval,i;
    char *endptr;
    double sum, partial_sum, start, stop;

    // MPI initialization
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &procid);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Status status;

    start = MPI_Wtime();

    // Dividing the range in equal parts
    startval = (N*(procid/nproc)) +1; // 0 to n, n+1 to 2n, 2n+1 to 3n etc...
    endval = N*((procid+1)/nproc);

    // Summing only the part assigned to the particular process
    for (i=startval;i<=endval;i++)
    {
        sum = sum + i;
    }

    if (procid!=0) // If not host process, send the partial sum to host process (process ID 0)
    {
        MPI_Send(&sum, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }
    else
    {
        for (j=1;j<nproc;j++) // For the host process, recieve the partial sum from subordinate processes, and join to main sum
        {
            MPI_Recv(&partial_sum, 1, MPI_DOUBLE, j, 1, MPI_COMM_WORLD, &status);
            sum = sum + partial_sum;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    stop = MPI_Wtime();

    MPI_Finalize();

    if (procid == 0)
    {
        printf("Runtime = %F s\n", stop - start);
    }
}

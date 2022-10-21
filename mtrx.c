#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "mpi.h"

#define N 10000 // Number of rows and columnns in the matrix
long long int A[N][N], B[N][N], C[N][N]; // C = A * B

int main(int argc, char **argv)
{
  int nproc, procid, nslave, nrows, row_marker;
  int i, j, k;
  double start, stop;

// MPI initialization
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &procid);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Status status;
  MPI_Barrier(MPI_COMM_WORLD);

  nslave = nproc - 1;

  start = MPI_Wtime();

  if (procid == 0) // Root process
  {
  // Matrix A and Matrix B both will be filled with random numbers
      srand ( time(NULL) );
      for (i = 0; i<N; i++)
      {
        for (j = 0; j<N; j++)
        {
          A[i][j]= rand();
          B[i][j]= rand();
        }
      }

      nrows = N/nslave;
      row_marker = 0;
      for (i = 1; i <= nslave; i++)
      {

        MPI_Send(&row_marker, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        MPI_Send(&nrows, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
        MPI_Send(&A[row_marker][0], nrows*N, MPI_INT,i,3, MPI_COMM_WORLD);
        MPI_Send(&B, N*N, MPI_INT, i, 4, MPI_COMM_WORLD);
        row_marker += nrows;
      }

      for (i = 1; i <= nslave; i++)
      {
        MPI_Recv(&row_marker, 1, MPI_INT, i, 5, MPI_COMM_WORLD, &status);
        MPI_Recv(&nrows, 1, MPI_INT, i, 6, MPI_COMM_WORLD, &status);
        MPI_Recv(&C[row_marker][0], nrows*N, MPI_INT, i, 7, MPI_COMM_WORLD, &status);
      }
  }
  else // Slave Processes
  {
    MPI_Recv(&row_marker, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&nrows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
    MPI_Recv(&A, nrows*N, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
    MPI_Recv(&B, N*N, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);

    //Matrix multiplication
    for (k = 0; k < N; k++)
    {
      for (i = 0; i < nrows; i++)
      {
        C[i][k] = 0;
        for (j = 0; j < N; j++)
        {
          C[i][k] += (A[i][j] * B[j][k]);
        }
      }
    }
    MPI_Send(&row_marker, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
    MPI_Send(&nrows, 1, MPI_INT, 0, 6, MPI_COMM_WORLD);
    MPI_Send(&C, nrows*N, MPI_INT, 0, 7, MPI_COMM_WORLD);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  stop = MPI_Wtime();

  MPI_Finalize();

  if (procid == 0)
  {
    printf("Runtime = %F s\n", stop - start);
  }
}

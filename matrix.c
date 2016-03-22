#include <mpi.h>
#include <stdio.h>

#define MATRIX_SIZE	4

int main(int argc, char ** argv) {
	int rank, numprocs, i, j, k;
	int A[MATRIX_SIZE][MATRIX_SIZE];
	int B[MATRIX_SIZE][MATRIX_SIZE];
	int C[MATRIX_SIZE][MATRIX_SIZE];
	
	MPI_Init (&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	for(i = 0; i < MATRIX_SIZE; i++) {
		for(j = 0; j < MATRIX_SIZE; j++) {
			A[i][j] = i + j;
			B[i][j] = i + j;
			C[i][j] = 0;
		}
	}
	
	for(i = 0; i < MATRIX_SIZE; i++) {
		for(j = 0; j < MATRIX_SIZE; j++) {
			for(k = 0; k < MATRIX_SIZE; k++) {
				C[i][j] += A[i][k] * B[k][j]; 
			}
		}
	}
	
	for(i = 0; i < MATRIX_SIZE; i++) {
		for(j = 0; j < MATRIX_SIZE; j++) {
			printf("%d\t", C[i][j]);
			if(j == (MATRIX_SIZE - 1))
				printf("\n");
		}
	}
	
	MPI_Finalize();
}

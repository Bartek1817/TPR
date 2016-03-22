#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MATRIX_SIZE	4

void print_matrix(int ** M, int n) {
	int i, j;
	for(i = 0; i < n; i++) {
		for(j = 0; j < n; j++)
			printf("%d\t", M[i][j]);
		printf("\n");
	}
}

void print_vector(int * V, int n) {
	int i;
	for(i = 0; i < n; i++)
		printf("%d\t", V[i]);
	printf("\n");
}

int main(int argc, char ** argv) {
	int rank, numprocs, i, j, k;
	int ** A;
	int ** B;
	int ** C;
	int *dataA;
	int *dataB;
	int *dataC;
	int vector_in[MATRIX_SIZE];
	int vector_out[MATRIX_SIZE];
	
	MPI_Init (&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	MPI_Datatype col, coltype;
	MPI_Type_vector(MATRIX_SIZE, 1, MATRIX_SIZE, MPI_INT, &col);
	MPI_Type_commit(&col);
	MPI_Type_create_resized(col, 0, sizeof(int), &coltype);
	MPI_Type_commit(&coltype);
	
	dataA = (int *)malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int));
	dataB = (int *)malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int));
	dataC = (int *)malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(int));
	A = (int **)malloc(MATRIX_SIZE * sizeof(int *));
	B = (int **)malloc(MATRIX_SIZE * sizeof(int *));
	C = (int **)malloc(MATRIX_SIZE * sizeof(int *));
	for (i = 0; i < MATRIX_SIZE; i++) {
		A[i] = &(dataA[i * MATRIX_SIZE]);
		B[i] = &(dataB[i * MATRIX_SIZE]);
		C[i] = &(dataC[i * MATRIX_SIZE]);
	}
	
	if(rank == 0) {	
		for(i = 0; i < MATRIX_SIZE; i++) {
			for(j = 0; j < MATRIX_SIZE; j++) {
				A[i][j] = i + j;
				B[i][j] = i + j;
			}
		}
	}
	
	MPI_Bcast(dataA, MATRIX_SIZE * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(dataB, 1, coltype, vector_in, MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	
	for(i = 0; i < MATRIX_SIZE; i++)
			vector_out[i] = 0;
	for(i = 0; i < MATRIX_SIZE; i++) {
		for(j = 0; j < MATRIX_SIZE; j++) {
			vector_out[i] += A[i][j] * vector_in[j]; 
		}
	}
	
	MPI_Gather(vector_out, MATRIX_SIZE, MPI_INT, dataC, 1, coltype, 0, MPI_COMM_WORLD);

	if(rank == 0) {
		print_matrix(C, MATRIX_SIZE);
	}
	
	MPI_Finalize();
}

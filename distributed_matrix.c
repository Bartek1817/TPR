#include <mpi.h>
#include <stdio.h>

#define MATRIX_SIZE	4

int main(int argc, char ** argv) {
	int rank, numprocs, i, j, k;
	int A[MATRIX_SIZE][MATRIX_SIZE];
	int B[MATRIX_SIZE][MATRIX_SIZE];
	int C[MATRIX_SIZE][MATRIX_SIZE];
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
	
	if(rank == 0) {
		for(i = 0; i < MATRIX_SIZE; i++) {
			for(j = 0; j < MATRIX_SIZE; j++) {
				A[i][j] = i + j;
				B[i][j] = i + j;
			}
		}
	}
	
	MPI_Bcast(A, MATRIX_SIZE * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(B, 1, coltype, vector_in, MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	
	for(i = 0; i < MATRIX_SIZE; i++)
			vector_out[i] = 0;
	for(i = 0; i < MATRIX_SIZE; i++) {
			for(j = 0; j < MATRIX_SIZE; j++) {
				vector_out[i] += A[i][j] * vector_in[j]; 
			}
	}
	
	MPI_Gather(vector_out, MATRIX_SIZE, MPI_INT, C, 1, coltype, 0, MPI_COMM_WORLD);

	if(rank == 0) {
		for(i = 0; i < MATRIX_SIZE; i++) {
			for(j = 0; j < MATRIX_SIZE; j++) {
				printf("%d\t", C[i][j]);
			}
			printf("\n");
		}
	}
	
	MPI_Finalize();
}

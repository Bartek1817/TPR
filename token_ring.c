#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
  	MPI_Init(NULL, NULL);
  	int world_rank, world_size, counter = 5, buffer = 1;
	double t1, t2, t3;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  	if (world_size < 2) {
    	fprintf(stderr, "World size must be two for %s\n", argv[0]);
    	MPI_Abort(MPI_COMM_WORLD, 1);
  	}
	if(world_rank == 0)
		MPI_Send(&buffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	while(counter--) {
		MPI_Recv(&buffer, 1, MPI_INT, (world_rank + world_size - 1) % world_size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		t2 = MPI_Wtime();
		//printf("Żeton posiada węzeł nr %d\n", world_rank);
		printf("Opóźnienie w węźle %d wynosi %f\n", world_rank, t2-t1);
		MPI_Send(&buffer, 1, MPI_INT, (world_rank + world_size + 1) % world_size, 0, MPI_COMM_WORLD);
		t1 = MPI_Wtime();
	}	
 	MPI_Finalize();
}



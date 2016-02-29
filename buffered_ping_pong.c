#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
  	MPI_Init(NULL, NULL);
  	int world_rank, world_size, s1, bufsize, buffer = 1, counter = 10;
	double t1, t2, t3;
	char * buf;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Pack_size(1, MPI_INT, MPI_COMM_WORLD, &s1);
	bufsize = MPI_BSEND_OVERHEAD + s1;
 	buf = (char *)malloc(bufsize);
	MPI_Buffer_attach(buf, bufsize);
  	if (world_size != 2) {
    	fprintf(stderr, "World size must be two for %s\n", argv[0]);
    	MPI_Abort(MPI_COMM_WORLD, 1);
  	}
	while(counter--) {
		if(world_rank == 0) {
			t1 = MPI_Wtime();
			MPI_Bsend(&buffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			t2 = MPI_Wtime();
			printf("Ping number %d\n", 10 - counter);
			MPI_Recv(&buffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			t3 = MPI_Wtime();
			printf("Czas działania Send w węźle %d: %f\n", world_rank, t2-t1);
			printf("Czas oczekiwania na odpowiedź w węźle %d: %f\n", world_rank, t3-t2);
			sleep(1);
		}
		else if(world_rank == 1){
			MPI_Recv(&buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//t3 = MPI_Wtime();
			MPI_Bsend(&buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
			//t2 = MPI_Wtime();
			printf("Pong number %d\n", 10 - counter);
		}
	}
 	MPI_Finalize();
}



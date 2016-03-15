#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SAMPLES		1000000000
#define MASTER		0
#define RADIUS		32767

int main(int argc, char ** argv) {
	int rank, numprocs, i, x, y, inside = 0, overall = 0;
	double pi, t1, t2;
	int sendbuf[2];
	int * recvbuf;
	sendbuf[0] = 0;
	sendbuf[1] = 0;
	
	MPI_Init (&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	recvbuf = (int *)calloc(2 * numprocs, sizeof(int));
	/*if(numprocs != 2) {
		fprintf(stderr, "World size must be 2 for %s\n", argv[0]);
    		MPI_Abort(MPI_COMM_WORLD, 1);
	}*/

	srand(time(NULL));

	t1 = MPI_Wtime();	

	for(i = 0; i < SAMPLES / numprocs; i++) {
		x = rand() % (RADIUS + 1);
		y = rand() % (RADIUS + 1);
		if((x * x + y * y) < (RADIUS * RADIUS)) 
			(sendbuf[0])++;
		(sendbuf[1])++;
	}

	MPI_Gather(sendbuf, 2, MPI_INT, recvbuf, 2, MPI_INT, MASTER, MPI_COMM_WORLD);
	
	if(rank == MASTER) {
		for(i = 0; i < 2 * numprocs; i += 2) {
			inside += recvbuf[i];
			overall += recvbuf[i + 1];
		}
		pi = 4 * (double)inside / overall;

		t2 = MPI_Wtime();

		printf("Distributed program\n");
		printf("PI = %.40f\n", pi);
		printf("Calculation time = %f[s]\n", t2 - t1);
	}
	
	MPI_Finalize();
}

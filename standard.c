#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MSG_MAX_LEN	2 * 1024
#define SAMPLES		10000000
#define MSG_ID		0
#define MASTER		0
#define SLAVE		1
#define RADIUS		32767

int main(int argc, char ** argv) {
	int rank, numprocs, i, x, y, inside = 0, overall = 0;
	double pi, t1, t2;
	
	MPI_Init (&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/*if(numprocs != 2) {
		fprintf(stderr, "World size must be 2 for %s\n", argv[0]);
    		MPI_Abort(MPI_COMM_WORLD, 1);
	}*/

	srand(time(NULL));
	
	t1 = MPI_Wtime();

	for(i = 0; i < SAMPLES; i++) {
		x = rand() % (RADIUS + 1);
		y = rand() % (RADIUS + 1);
		if((x * x + y * y) < (RADIUS * RADIUS)) 
			inside++;
		overall++;
	}

	t2 = MPI_Wtime();
	
	pi = 4 * (double)inside / overall;
	
	printf("PI = %.40f\n", pi);
	printf("Calculation time = %f[s]\n", t2 - t1);
	
	MPI_Finalize();
}

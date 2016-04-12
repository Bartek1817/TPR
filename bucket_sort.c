#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[]) {
	int n_threads, id, min;
	long n, i, j, k, chunk, ppt, sp;
	float delta, tmp;
	float * tab;
	float ** buckets;
	int * counter; 
	double t1, t2, t_single, t_multi;



	n = atol(argv[1]);
	n_threads = atoi(argv[2]);
	chunk = atoi(argv[3]);
 	ppt = (long)(n / n_threads);
	delta = 1.0 / n_threads;
	tab = (float *)malloc(n * sizeof(float));
	counter = (int *)calloc(n_threads, sizeof(int));
	buckets = (float **)malloc(n_threads * sizeof(float *));
	for(i = 0; i < n_threads; i++)
		buckets[i] = (float *)malloc(2 * n / n_threads * sizeof(float *));	
	for(i = 0; i < n; i++) 
		tab[i] = (float)rand() / ((float)RAND_MAX + 1);


//**********************************************************************
//								Single thread						   *
//**********************************************************************

	t1 = omp_get_wtime();
	//#pragma omp parallel for schedule(static, chunk) default(none) \
		private(i, j) shared(tab, buckets, counter, n, delta, chunk)
	for(i = 0; i < n; i++) {
		j = 0;
		while(tab[i] > ++j * delta);
		//#pragma opm critical
		buckets[--j][counter[j]++] = tab[i];
	}	
	//t2 = omp_get_wtime();	
	//printf("Etap 1: %f\n", t2 - t1);

	//t1 = omp_get_wtime();
	//#pragma omp parallel num_threads(n_threads) default(none) \
	 private(i, j, id, min, tmp) shared(tab, buckets, counter)
	//{
		/*#pragma omp single 
		{
			printf("Liczba wątków: %d\n", omp_get_num_threads());
		}*/
	for(k = 0; k < n_threads; k++) {
		//id = omp_get_thread_num();
		for(i = 0; i < counter[k] - 1; i++) {
			min = i;
			for(j = i + 1; j < counter[k]; j++) 
				if(buckets[k][j] < buckets[k][min]) min = j;
			tmp = buckets[k][i];
			buckets[k][i] = buckets[k][min];
			buckets[k][min] = tmp;
		}
	}
	//}

	t2 = omp_get_wtime();
	t_single = t2 - t1;
	/*/printf("Time: %f\n", t2 - t1);
	/for(i = 0; i < n_threads; i++) {
		for(j = 0; j < counter[i]; j++)
			printf("%f\t", buckets[i][j]);
		printf("\n");
	}*/

//**********************************************************************
//								Multi thread						   *
//**********************************************************************

	for(i = 0; i < n_threads; i++)
		counter[i] = 0;

	t1 = omp_get_wtime();
	//#pragma omp parallel for schedule(static, chunk) default(none) \
		private(i, j) shared(tab, buckets, counter, n, delta, chunk)
	for(i = 0; i < n; i++) {
		j = 0;
		while(tab[i] > ++j * delta);
		//#pragma opm critical
		buckets[--j][counter[j]++] = tab[i];
	}	
	//t2 = omp_get_wtime();	
	//printf("Etap 1: %f\n", t2 - t1);

	//t1 = omp_get_wtime();
	#pragma omp parallel num_threads(n_threads) default(none) \
	 private(i, j, id, min, tmp) shared(tab, buckets, counter)
	{
		/*#pragma omp single 
		{
			printf("Liczba wątków: %d\n", omp_get_num_threads());
		}*/
		id = omp_get_thread_num();
		for(i = 0; i < counter[id] - 1; i++) {
			min = i;
			for(j = i + 1; j < counter[id]; j++) 
				if(buckets[id][j] < buckets[id][min]) min = j;
			tmp = buckets[id][i];
			buckets[id][i] = buckets[id][min];
			buckets[id][min] = tmp;
		}
	}

	t2 = omp_get_wtime();
	t_multi = t2 - t1;
	//printf("Time: %f\n", t2 - t1);
	/*for(i = 0; i < n_threads; i++) {
		for(j = 0; j < counter[i]; j++)
			printf("%f\t", buckets[i][j]);
		printf("\n");
	}*/

	printf("Number of threads: %d\nSingle thread time: %f\n\
Multi thread time: %f\nSpeed up: %f", n_threads, t_single, 
			t_multi, t_single / t_multi);


	free(tab);	
	for(i = 0; i < n_threads; i++)
		free(buckets[i]);
	free(buckets);	 	
}

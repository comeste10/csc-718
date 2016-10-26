// Steven Comer
// CSC 718
// Project Part II
// 22 Oct 2016

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#define N 100000000

int *makeArray(int length) {
	srand(time(NULL));
	int *array = malloc(sizeof(int)*length);
	for(int i=0; i<length; i++) {
		array[i] = (int)rand();
	}
	return array;
}

int *copyArray(int length, int *array) {
	int * new_array = malloc(sizeof(int)*length);
	memcpy(new_array, array, sizeof(int)*length);
	return new_array;
}

int sorted(int length, int *array) {
	for(int i=0; i<length-1; i++) {
		if(array[i] > array[i+1]) {
			return 0;
		}
	}
	return 1;
}

void printArray(int length, int *array) {
	for(int i=0; i<length; i++) {
		printf("%d\n", array[i]);
	}
	printf("\n");
}

void do_merge(int length, int *a, int *b) {
	int half_length,i,j,k;
	half_length = length/2;
	i = 0;
	j = half_length;
	k = 0;
	while(i<half_length && j<length) {
		if(a[i] <= a[j]) {
			b[k] = a[i];
			i++;
		}
		else {
			b[k] = a[j];
			j++;
		}
		k++;
	}
	while(i<half_length) {
		b[k] = a[i];
		i++;
		k++;
	}
	while(j<length) {
		b[k] = a[j];
		j++;
		k++;
	}
	memcpy(a, b, sizeof(int)*length);
}

void do_sMergeSort(int length, int *a, int *b) {
    if(length < 2) {
        return;
    }
	int cut = length/2;
	do_sMergeSort(cut, a, b);
	do_sMergeSort(length-cut, a+cut, b+cut);
	do_merge(length, a, b);
}

void sMergeSort(int length, int *array) {
    int *temp_array = copyArray(length, array);
    do_sMergeSort(length, array, temp_array);
    free(temp_array);
}

void do_pMergeSort(int length, int *a, int *b, int num_threads) {
	if(num_threads < 2) {
		do_sMergeSort(length, a, b);
	}
	else {
		int cut = length/2;
		int thread_cut = num_threads/2;
		#pragma omp parallel sections
		{
			#pragma omp section
			do_pMergeSort(cut, a, b, thread_cut);
			#pragma omp section
			do_pMergeSort(length-cut, a+cut, b+cut, num_threads-thread_cut);
		}
		do_merge(length, a, b);
	}
}

void pMergeSort(int length, int *array, int num_threads) {
	int *temp_array = copyArray(length, array);
	do_pMergeSort(length, array, temp_array, num_threads);
	free(temp_array);
}

int main(int argc, char **argv) {

	// setup
	int *s_rand_array = makeArray(N);
	int *p2_rand_array = copyArray(N, s_rand_array);
	int *p4_rand_array = copyArray(N, s_rand_array);
	omp_set_dynamic(0);
	int num_threads;
	clock_t begin, end;
	double exec_time;

	// sequential
	begin = clock();
	sMergeSort(N, s_rand_array);
	end = clock();
	exec_time = (double)(end - begin) / CLOCKS_PER_SEC * 1000;
	printf("Sequential execution time:\t\t%10.2f ms\n", exec_time);

	// 2 threads
	omp_set_num_threads(2);
	#pragma omp parallel
	{
		#pragma omp master
		{
			num_threads = omp_get_num_threads();
		}
	}
	begin = clock();
	pMergeSort(N, p2_rand_array, num_threads);
	end = clock();
	exec_time = (double)(end - begin) / CLOCKS_PER_SEC * 1000;
	printf("%d threads execution time:\t\t%10.2f ms\n", num_threads, exec_time);

	// 4 threads
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        #pragma omp master
        {
            num_threads = omp_get_num_threads();
        }
    }
    begin = clock();
    pMergeSort(N, p4_rand_array, num_threads);
    end = clock();
    exec_time = (double)(end - begin) / CLOCKS_PER_SEC * 1000;
    printf("%d threads execution time:\t\t%10.2f ms\n", num_threads, exec_time);

	free(s_rand_array);
	free(p2_rand_array);
	free(p4_rand_array);
	return 0;
}

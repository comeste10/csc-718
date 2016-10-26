// Steven Comer
// CSC 718
// Project Part II
// 22 Oct 2016

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#define N 6

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

/*
void do_merge(int *a, int first, int middle, int last, int *b) {
	int i,j,k;
	i = first;
	j = middle;
	for(k=first; k<last; k++) {
		if(i<middle && (j>=last || a[i] <= a[j])) {
			b[k] = a[i];
			i++;
		}
		else {
			b[k] = a[j];
			j++;
		}
	}
}
*/

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

void do_sMergeSort(int *b, int first, int last, int *a) {
    if(last - first < 2) {
        return;
    }
    int middle = (first + last)/2;
    do_sMergeSort(a, first, middle, b);
    do_sMergeSort(a, middle, last, b);
    //do_merge(b, first, middle, last, a);
	do_merge(N, a, b);
}

void sMergeSort(int length, int *array) {
    int *temp_array = copyArray(length, array);
    do_sMergeSort(temp_array, 0, length, array);
    free(temp_array);
}

void do_pMergeSort(int *b, int first, int last, int *a, int num_threads) {
	if(num_threads == 1) {
		do_sMergeSort(b, first, last, a);
	}
	else if(num_threads > 1){
		int middle = (first + last)/2;
		#pragma omp parallel sections
		{
			#pragma omp section
			do_pMergeSort(b, first, middle, a, num_threads/2);
			#pragma omp section
			do_pMergeSort(b, middle, last, a, num_threads/2);	
		}
		//do_merge(b, first, middle, last, a);
	}
}

void pMergeSort(int length, int *array, int num_threads) {
	int *temp_array = copyArray(length, array);
	do_pMergeSort(temp_array, 0, length, array, num_threads);
	free(temp_array);
}

int main(int argc, char **argv) {

	// setup
	int *s_rand_array = makeArray(N);
	int *p_rand_array = copyArray(N, s_rand_array);

	// sequential
	clock_t s_begin = clock();
	sMergeSort(N, s_rand_array);
	clock_t s_end = clock();
	double s_time = (double)(s_end - s_begin) / CLOCKS_PER_SEC * 1000;
	printf("Sorted? %d\n", sorted(N, s_rand_array));
	printf("Sequential execution time:\t\t%10.2f ms\n", s_time);

	// parallel
	/*
	int num_threads;
	#pragma omp parallel
	{
		#pragma omp master
		{
			num_threads = omp_get_num_threads();
		}
	}
	clock_t p_begin = clock();
	pMergeSort(N, p_rand_array, num_threads);
	clock_t p_end = clock();
	double p_time = (double)(p_end - p_begin) / CLOCKS_PER_SEC * 1000;
	printf("Sorted? %d\n", sorted(N, p_rand_array));
	printArray(N, p_rand_array);
	printf("Parallel execution time:\t\t%10.2f ms\n", p_time);
	*/

	free(s_rand_array);
	free(p_rand_array);
	return 0;
}

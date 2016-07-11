/*
Author:		Dillon VanBuskirk & Connor Nesbitt
Date:		7-11-2016
Program:	Shear Sort [Parallel]
Purpose:	Sort data in snake order. This program reads data in a 1d array
			and converts it into a 2d array with number of rows equal to the
			number of threads. After sorting, it converts the 2d array back into
			a 1d array.
*/

#include "shear_sort.hpp"


int shear_sort::compare(const void * elem1, const void * elem2) {
	int f = *((int*)elem1);
	int s = *((int*)elem2);
	if (f > s) return  1;
	if (f < s) return -1;
	return 0;
}

int shear_sort::compare_reversed(const void * elem1, const void * elem2) {
	int f = *((int*)elem1);
	int s = *((int*)elem2);
	if (f < s) return  1;
	if (f > s) return -1;
	return 0;
}

void shear_sort::quicksort_column(int **a, int top, int bottom, int column) {
	int i = top, j = bottom;

	int pivot = bottom;
	swap(a[(i + j) / 2][column], a[bottom][column]);

	while (i < j) {
		while (i < j && a[i][column] < a[pivot][column])
			i++;
		while (i < j && a[j][column] >= a[pivot][column])
			j--;
		swap(a[i][column], a[j][column]);
	}

	swap(a[i][column], a[pivot][column]);
	pivot = i;

	if (top < pivot - 1)
		quicksort_column(a, top, pivot - 1, column);
	if (pivot + 1 < bottom)
		quicksort_column(a, pivot + 1, bottom, column);
}

void shear_sort::performAlgorithm(int **a, const int NUM_DATA, const int NUM_ROWS, const int NUM_COLUMNS, const int THREAD_COUNT) {
	int SHEARSORT_RUN_COUNT = (int)(ceil(log2(NUM_ROWS)) + 1);

	for (int i = 0; i < SHEARSORT_RUN_COUNT*2; i++) {
		if (i % 2 == 0) {
#pragma omp parallel for num_threads(THREAD_COUNT)
			for (int j = 0; j < NUM_ROWS; j++) {
				if (j % 2 == 0) {
					qsort(a[j], NUM_COLUMNS, sizeof(*a[j]),compare);
				}
				else {
					qsort(a[j], NUM_COLUMNS, sizeof(*a[j]),compare_reversed);
				}
			}
		}
		else {
#pragma omp parallel for num_threads(THREAD_COUNT)
			for (int j = 0; j < NUM_COLUMNS; j++) {
				quicksort_column(a, 0, NUM_ROWS - 1, j);
			}
		}
	}
}

std::string shear_sort::name() const 
{
    return "Shear Sort Algorithm";
}

shear_sort::shear_sort(int _nthreads) : sortable(_nthreads)
{
}

void shear_sort::sort_array(int array[],int n)
{
	THREAD_COUNT = nthreads;
	NUM_ROWS = THREAD_COUNT;
	NUM_COLUMNS = n / NUM_ROWS; 
	NUM_DATA = NUM_ROWS * NUM_COLUMNS;
	a = new int *[NUM_ROWS];
    int count = 0;
	
	
	for (unsigned long int r = 0; r < NUM_ROWS; r++) {
		a[r] = new int[NUM_COLUMNS];
	}

	for (int r = 0; r < NUM_ROWS; r++) {
		for (int c = 0; c < NUM_COLUMNS; c++) {
			a[r][c] = array[count];
			count++;
		}
	}
	
	performAlgorithm(a, n, NUM_ROWS, NUM_COLUMNS, THREAD_COUNT);
	
	/* Output */
	for (int r = 0; r < NUM_ROWS; r++) {
		if (r % 2 == 0) {
			for (int c = 0; c < NUM_COLUMNS; c++) {
				array[r*NUM_COLUMNS+c] = a[r][c];
			}
		}
		else {
			for (int c = NUM_COLUMNS - 1; c >= 0; c--) {
				array[(r*NUM_COLUMNS) + ((NUM_COLUMNS - 1) - c)] = a[r][c];
			}
		}
	}
	/* End Output */
	
	delete[] a;
}

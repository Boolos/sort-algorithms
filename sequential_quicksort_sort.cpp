#include "sequential_quicksort_sort.hpp"

sequential_quicksort_sort::sequential_quicksort_sort(int _nthreads) : sortable(_nthreads) { }

void sequential_quicksort_sort::sort_array(int array[], int n){
	std::sort(array, array + n);
}
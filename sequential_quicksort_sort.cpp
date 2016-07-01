#include "sequential_quicksort_sort.hpp"

sequential_quicksort_sort::sequential_quicksort_sort(int _nthreads) : sortable(_nthreads) { }

std::string sequential_quicksort_sort::name() const {
	return "Sequential sort";
}

void sequential_quicksort_sort::sort_array(int array[], int n){
	std::sort(array, array + n);
}
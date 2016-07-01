#include "bitonic_sort.hpp"

bitonic_sort::bitonic_sort(int _nthreads) : sortable(_nthreads) { }

std::string bitonic_sort::name() const {
	return "Batcher's bitonic sort";
}

void bitonic_sort::sort_array(int array[], int n){
	//TODO: this method takes in the array of values and the size of the array and it needs to
	//      modify the array such that after this method, the values in array are sorted.
}
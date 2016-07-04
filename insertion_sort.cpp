#include "insertion_sort.hpp"

insertion_sort::insertion_sort(int _nthreads) : sortable(_nthreads) { }

std::string insertion_sort::name() const {
    return "Insertion sort";
}

void insertion_sort::sort_array(int array[], int n) {
}
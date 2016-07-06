#include "insertion_sort.hpp"

insertion_sort::insertion_sort(int _nthreads) : sortable(_nthreads) { }

std::string insertion_sort::name() const {
    return "Sequential insertion sort";
}

void insertion_sort::sort_array(int array[], int n) {
    int j;
    int target;
    
    for (int i = 1; i < n; i++) {
        target = array[i];
        j = i - 1;
        while (j >= 0 && target < array[j]) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = target;
    }
}
#include "insertion_sort.hpp"
#include <iostream>

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
        array[j] = target;
    }
}

/*
void insertion_sort::sort_array(int array[], int n) {
    sort_array(array, n, 2);
}
*/

void insertion_sort::sort_array(int array[], int n, int increment) {
    sort_array(array, n, increment, 0);
}

void insertion_sort::sort_array(int array[], int n, int increment, int start) {
    int j;
    int target;
    
    for (int i = increment + start; i < n; i += increment) {
        target = array[i];
        j = i - increment;
        while (j >= start && target < array[j]) {
            array[j + increment] = array[j];
            j -= increment;
        }
        array[j] = target;
    }
}

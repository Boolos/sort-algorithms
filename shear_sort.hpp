#ifndef shear_sort_hpp
#define shear_sort_hpp

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <chrono>
#ifdef _OPENMP
#include <omp.h>
#endif
//#include <random>
#include <vector>
#include <utility>
#include "sortable.hpp"
using namespace std;

class shear_sort: public sortable {
public:
    shear_sort(int _nthreads);
    std::string name() const;
    void sort_array(int array[],int n);
private:
    int MAX_DATA;
    int MIN_DATA;
    int NUM_ROWS; 
    int NUM_COLUMNS;
    int THREAD_COUNT;
    int NUM_DATA;
    int **a;

    void performAlgorithm(int **a, const int NUM_DATA, const int NUM_ROWS, const int NUM_COLUMNS, const int THREAD_COUNT);
    void quicksort_column(int **a, int top, int bottom, int column);
    static int compare_reversed(const void * elem1, const void * elem2);
    static int compare(const void * elem1, const void * elem2);

};

#endif

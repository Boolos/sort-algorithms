#include "shell_sort.hpp"

using namespace std;

shell_sort::shell_sort(int nthreads, sortable * sequential_sort) 
    : _nthreads(nthreads), _sequential_sort(sequential_sort) { }

string shell_sort::name() const {
    return "Parallel shell sort";
}

void shell_sort::sort_array(int array[], int n) {
    int gap_sequence [] = { 701, 301, 132, 57, 23, 10, 4, 1 };

    for (int i = 0; i < 8; i++) {
        int gap = gap_sequence[i];

        #pragma omp parallel num_threads(_nthreads)
        {
            #pragma omp for
            for (int j = 0; j < gap; j++) {
                int size = (j == 0 && (n - 1) % gap == 0) ? n / gap + 1 : n / gap;
                int partition [size];
                int count = 0;
                for (int k = 0 + j; k < n; k += gap) {
                    partition[count++] = array[k];
                }
                _sequential_sort->sort_array(partition, size);
                count = 0;
                for (int k = 0 + j; k < n; k += gap) {
                    array[k] = partition[count++];
                }
            }
        }
    }
}

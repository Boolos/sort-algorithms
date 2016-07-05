#include "shell_sort.hpp"

using namespace std;

shell_sort::shell_sort(int nthreads) : sortable(nthreads) { 
    _nthreads = nthreads;
}

string shell_sort::name() const {
    return "Parallel shell sort";
}

void shell_sort::sort_array(int array[], int n) {
    int max = n % 100;
    int med = max / 2;
    int gap_sequence[3] = { max, med, 1 };

    for (int i = 0; i < 3; i++) {
        int gap = gap_sequence[i];

        #pragma omp parallel num_threads(_nthreads) 
        {
            #pragma omp parallel for
            for (int j = gap; j < n; j++) {
                int target = array[j];
                int k = j;
                for (k; k >= gap && array[k - gap] > target; k -= gap) {
                    array[k] = array[k - gap];
                }
                array[k] = target;
            }
        }
    }
}

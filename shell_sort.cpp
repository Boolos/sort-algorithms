#include "shell_sort.hpp"

using namespace std;

shell_sort::shell_sort(int nthreads, sortable * sequential_sort) 
    : _nthreads(nthreads), _sequential_sort(sequential_sort) { }

string shell_sort::name() const {
    return "Parallel shell sort";
}

void shell_sort::sort_array(int array[], int n) {
    std::vector<int> gap_sequence = { 100000, 10000, 701, 301, 132, 57, 23, 10, 4, 1 };
    sort(array, n, gap_sequence);
}

void shell_sort::sort(int array[], int array_size, std::vector<int> gap_sequence) {
    for (int i = 0; i < gap_sequence.size(); i++) {
        int gap = gap_sequence[i];
        sort_gap_in_parallel(array, array_size, gap);
    }
}

void shell_sort::sort_gap_in_parallel(int array[], int array_size, int gap) {
    #pragma omp parallel num_threads(_nthreads)
    {
        #pragma omp for
        for (int j = 0; j < gap; j++) {
            std::vector<int> partition = make_partition(&array[j], array_size - j, gap);
            _sequential_sort->sort_array(&partition[0], partition.size());
            write_to_array(partition, &array[j], array_size - j, gap);
        }
    }
}

void shell_sort::write_to_array(std::vector<int> partition, int array[], int array_size, int gap) {
    int count = 0;
    for (int k = 0; k < array_size; k += gap) {
        array[k] = partition[count++];
    }
}

std::vector<int> shell_sort::make_partition(int array[], int array_size, int gap) {
    std::vector<int> partition;

    int count = 0; 
    for (int i = 0; i < array_size; i += gap) {
        partition.push_back(array[i]);
    }

    return partition;
}
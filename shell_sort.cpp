#include "shell_sort.hpp"

using namespace std;

shell_sort::shell_sort(int _nthreads) : sortable(_nthreads) { }

string shell_sort::name() const {
    return "Shell sort";
}

void shell_sort::sort_array(int array[], int n) {
}
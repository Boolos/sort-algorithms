#ifndef sequential_quicksort_sort_hpp
#define sequential_quicksort_sort_hpp

#include <algorithm>
#include "sortable.hpp"

class sequential_quicksort_sort : public sortable {
public:
	sequential_quicksort_sort(int _nthreads);
	
	void sort_array(int array[], int n);
};

#endif /* sequential_quicksort_sort_hpp */

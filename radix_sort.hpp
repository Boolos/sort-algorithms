#ifndef radix_sort_hpp
#define radix_sort_hpp


#include <iostream>
#include <cstdint>
#include <omp.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <iterator>
#include "sortable.hpp"

class radix_sort : public sortable {
public:
	radix_sort(int _nthreads);
	std::string name() const;
	void sort_array(int array[], int n);

private:
	int _nthreads;

};

#endif /* radix_sort_hpp */

#ifndef radix_sort_hpp
#define radix_sort_hpp

#include <algorithm>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>
#include <stdlib.h>
#include "sortable.hpp"

class radix_sort : public sortable {
public:
	radix_sort(int _nthreads);
	std::string name() const;
	void sort_array(int array[], int n);
	//int compare(int a, int b, int digit) const;
	const int radix = 16;
	
private:
	std::vector<int> powers;
};

#endif /* radix_sort_hpp */

#ifndef bitonic_sort_hpp
#define bitonic_sort_hpp

#include <string>
#include "sortable.hpp"

class bitonic_sort : public sortable {
public:
	bitonic_sort(int _nthreads);
	
	std::string name() const;
	
	void sort_array(int array[], int n);
};

#endif /* bitonic_sort_hpp */

#ifndef insertion_sort_hpp
#define insertion_sort_hpp

#define CSCE_SORT_DEBUG false

#include "sortable.hpp"

class insertion_sort : public sortable {
public:
	insertion_sort(int _nthreads);
	std::string name() const;
	void sort_array(int array[], int n);
	void sort_array(int array[], int n, int increment);
	void sort_array(int array[], int n, int increment, int start);
	
private:
};


#endif /* insertion_sort_hpp */

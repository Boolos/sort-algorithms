#ifndef sortable_hpp
#define sortable_hpp

#include <string>

class sortable {
public:
	virtual void sort_array(int array[], int n) = 0;
	virtual std::string name() const = 0;
	sortable(int _nthreads = 4);
	int nthreads;
	
};

#endif /* sortable_hpp */

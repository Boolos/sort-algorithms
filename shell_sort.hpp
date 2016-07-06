#ifndef shell_sort_hpp
#define shell_sort_hpp

#define CSCE_SORT_DEBUG false

#include "sortable.hpp"
#include "insertion_sort.hpp"

class shell_sort : public sortable {
public:
    shell_sort(int nthreads, sortable * sequential_sort);
	std::string name() const;
	void sort_array(int array[], int n);
	
private:
    int _gap;
    int _nthreads;
    sortable * _sequential_sort;
};

#endif /* shell_sort_hpp */

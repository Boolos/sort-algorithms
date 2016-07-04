#ifndef shell_sort_hpp
#define shell_sort_hpp

#define CSCE_SORT_DEBUG false

#include "sortable.hpp"

class shell_sort : public sortable {
public:
    shell_sort(int _nthreads);
	std::string name() const;
	void sort_array(int array[], int n);
	
private:
};


#endif /* shell_sort_hpp */

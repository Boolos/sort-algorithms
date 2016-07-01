#ifndef even_odd_sort_hpp
#define even_odd_sort_hpp

#define CSCE_SORT_DEBUG false

#include <algorithm>
#include <cmath>
#include <thread>
#include <mutex>
#include "sortable.hpp"

class even_odd_sort : public sortable {
public:
	even_odd_sort(int _nthreads);
	std::string name() const;
	void sort_array(int array[], int n);
	
private:
	std::mutex cout_mutex;
	
	void sort_thread(int array[], int start, int stop);
	
	void merge_thread(int array[], int start, int mid, int stop);
};


#endif /* even_odd_sort_hpp */

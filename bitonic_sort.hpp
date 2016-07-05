#ifndef bitonic_sort_hpp
#define bitonic_sort_hpp

#include <string>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <omp.h>
#include "sortable.hpp"

class bitonic_sort : public sortable {
public:
	bitonic_sort(int _nthreads);
	
	std::string name() const;
	
	void sort_array(int array[], int n);

private:
	struct Data
	{
		int *array;
		int n;
	};
	
	Data *data;
	int THREADS;
	
	void runThread(size_t id);
	void reverse(int *array, int n);
	static int compare(const void *a, const void *b);
};

#endif /* bitonic_sort_hpp */

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
		int n, actualN;
		Data(int *array = NULL, int n = 0, int actualN = 0) : array(array), n(n), actualN(actualN) {}
		int operator[](size_t i) { if(i >= actualN) return INT_MAX; else return array[i]; }
	};
	
	Data *data;
	int THREADS;
	
	void runThread(size_t id);
	void reverse(int *array, int n);
	static int compare(const void *a, const void *b);
};

#endif /* bitonic_sort_hpp */

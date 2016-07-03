#ifndef bucket_sort_hpp
#define bucket_sort_hpp

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <array>
#include <thread>
#include <mutex>
#include <cmath>
#include <stdlib.h>
#include "sortable.hpp"

class bucket_sort : public sortable {
public:
	bucket_sort(int _nthreads);
	std::string name() const;
	void sort_array(int array[], int n);
	
private:
	std::vector<int> count;
	std::mutex* bucket_mutex;
	
	std::mutex limits_mutex;
	std::pair<int, int> limits; //the minimum and maximum value in the array
	
	const int nbuckets = 32;
	std::vector< std::pair<int, int> > bucket_limits; //the minimum and maximum value for each bucket
	std::vector<int> bucket_sizes;
	std::vector< std::vector<int> > buckets;
	int bucket_index(int value) const;
	
	
	void limits_thread(int array[], int start, int stop);
	void determine_bucket_ranges();
	void count_thread(int array[], int start, int stop);
	void insert_thread(int array[], int start, int stop);
	void sort_thread(int array[], int bucket_index, int start);
};

#endif /* bucket_sort_hpp */

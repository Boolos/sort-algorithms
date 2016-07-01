#include "radix_sort.hpp"

radix_sort::radix_sort(int _nthreads) : sortable(_nthreads) {
	this->powers.reserve(64);
	long long int r = 1L;
	long long int max = std::numeric_limits<int>::max();
	for(int x=0; r < max; x++){
		this->powers[x] = static_cast<int>(r);
		r *= this->radix;
	}
}

std::string radix_sort::name() const {
	return "Radix sort";
}

//returns -1 if a<b, 0 if a==b, and 1 if a>b for the given digit.
//this also needs to consider negative and positive values.
/*int radix_sort::compare(int a, int b, int digit) const {
	int _a = std::abs(static_cast<double>((a / this->powers[digit]) % this->radix));
	int _b = std::abs(static_cast<double>((b / this->powers[digit]) % this->radix));
	
	
	// 12
	// -30
	// 32
	// 20
	
	
	
	if(a >= 0 && b >= 0){
		return _a - _b;
	} else if(a < 0 && b < 0){
		return _b - _a;  //-31   12
	} else {
		
	}
}*/

void radix_sort::sort_array(int array[], int n){
	//std::sort(array, array + n);
	
	for(int digit = 0; digit < this->powers.size(); digit++){
		std::vector<int> count(this->radix);
		std::vector<std::pair<int, int>> ranges;
		std::vector<std::thread> threads;
		int start = 0, stop = 0;
		
		
		
		
		
		
		/*
		 
		 std::pair<int, int>* indices = new std::pair<int, int>[nthreads];
		 std::thread* threads = new std::thread[nthreads]; //4 threads
		 int nelements = static_cast<int>(std::ceill(static_cast<long double>(n) / static_cast<long double>(nthreads)));  // ceil(7 / 4) = 2
		 int start = 0, stop = nelements;
		 int segment_count = 0;
		 
		 //sort elements in each segment
		 for(int x=0; x<nthreads && start < n; x++, start += nelements, stop += nelements){
		 indices[x] = std::make_pair(start, std::min(stop, n));
		 threads[x] = std::thread(&even_odd_sort::sort_thread, this, array, indices[x].first, indices[x].second);
		 segment_count++;
		 }
		 for(int x=0; x<segment_count; x++) {
		 threads[x].join();
		 }
		 
		 */
		
		
		
		
		
		
		
		
	}
	
	
	/*
	 
	 start from least significant digit
	 for each digit, from least significant to most significant {
	 
		in the main thread, declare an int count[radix] such that count[0] is the number of elements
			with a value of zero in the current digit position
	 
		divide the array into different segments of equal size based on the number of threads
		in each thread {
			declare a local int count[radix] array and count the number of elements in the current thread
				that have a specific value for each digit position
			
			after the thread is done, add the values from the local count variable into the main thread's count
				variable using a mutex to prevent write contentions
		}
	 
		//now count[radix] contains something like [5, 7, 2, 3, 5, 2, 1, 3, 5, 3] meaning there are 5 elements
		//with a zero for the current digit and 7 elements with a one for the current digit and so on.
	 
		//so each digit can be placed in a bucket starting in position:
		//        [0,  0+5,  5+7,  5+7+2,   ...  ]
		
		declare an array of integers that is equal to the size of the radix. Each digit will have its own start position.
		declare an array of mutexes that is equal to the size of the radix. Each digit will have its own mutex.
	 
		declare an array of integers equal to the overall size of the original array. This will hold the sorted values.
	 
		divide the array into different segments again:
		for each thread {
			for each element in the thread's array segment, insert it into the new array at the position specified by the start position array. The start position will need to be increased based on a mutex. This whole operation should be atomic, so wrap the reading of the index, writing of the element, and increment of the index in a mutex.
		}
	 
	 
		copy the data back into the original array, either through memcpy or similar or possibly parallelize this also.
	 */
}
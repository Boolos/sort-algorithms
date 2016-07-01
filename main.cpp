#include <iostream>
#include <unistd.h>

#include "utility.hpp"
#include "sortable.hpp"
#include "sequential_quicksort_sort.hpp"
#include "even_odd_sort.hpp"
#include "radix_sort.hpp"
#include "bitonic_sort.hpp"

int main(int argc, char * argv[]) {
	int n = 48000000;
	int thread_count = 4;
	std::string input_file_path;
	std::string output_file_path;
	long long int duration = 0;
	bool debug = false;
	
	int c;
	while((c = getopt(argc, argv, ":df:n:o:t:")) != -1){
		switch(c){
			case 'd':
				debug = true;
				break;
				
			case 'f':
				if(optarg != NULL){
					input_file_path = std::string(optarg);
				}
				break;
				
			case 'n':
				if(optarg != NULL){
					n = std::stoi(optarg);
				}
				break;
				
			case 'o':
				if(optarg != NULL){
					output_file_path = std::string(optarg);
				}
				break;
			
			case 't':
				if(optarg != NULL){
					thread_count = std::stoi(optarg);
				}
				break;
				
			case '?':
				break;
		}
	}
	
	
	int* values = new int[n];
	
	//
	//first - load the values into the array, either by populating it
	//        with random values or by reading the values from a file.
	//
	std::cout << "Populating array with " << n << " values ... " << std::flush;
	if(input_file_path.empty()){
		//no input file was specified, so populate the array with random numbers
		duration = utility::duration_function(utility::fill_random, values, n);
	} else {
		//load from the specified file
		duration = utility::duration_function(utility::fill_from_file, values, n, input_file_path);
	}
	std::cout << "done in " << utility::duration_string(duration) << std::endl;
	std::cout << std::endl;
	
	
	//
	//second - if the output file path was specified, write the array to the output file
	//         so it can be used in subsequent runs of this process.
	//
	if(!output_file_path.empty()){
		std::cout << "Writing array to file (" << output_file_path << ")" << std::flush;
		duration = utility::duration_function(utility::write_to_file, values, n, output_file_path);
		std::cout << "done in " << utility::duration_string(duration) << std::endl;
		std::cout << std::endl;
	}
	
	
	//
	//third - pick a sortable implementation.
	//
	
	std::vector<sortable*> sorts;
	sorts.push_back(new sequential_quicksort_sort(thread_count));
	sorts.push_back(new even_odd_sort(thread_count));
	sorts.push_back(new radix_sort(thread_count));
	sorts.push_back(new bitonic_sort(thread_count));
	
	for(std::size_t x=0; x<sorts.size(); x++){
		int* values_copy = new int[n];
		std::copy(values, values + n, values_copy);
		
		std::cout << "-------------------------------------------" << std::endl;
		std::cout << "Testing sort implementation (" << sorts[x]->name() << ")" << std::endl;
		std::cout << std::endl;
		
		std::cout << "Sorting array ... " << std::flush;
		//TODO: fix this to where it will be able to invoke the sort_array method from a sortable instance
		//duration = duration_function(&sort_array, values, n);
		
		std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
		sorts[x]->sort_array(values, n);
		std::chrono::high_resolution_clock::time_point stop_time = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
		
		std::cout << "done in " << utility::duration_string(duration) << std::endl;
		std::cout << std::endl;
		
		std::cout << "Validating sort results ... " << std::flush;
		bool correct = utility::is_sorted(values, n);
		std::cout << (correct ? "correct" : "INCORRECT") << std::endl;
		std::cout << "--------------------------------------------" << std::endl << std::endl;
		
		std::copy(values_copy, values_copy + n, values);
		delete[] values_copy;
	}
	
	delete[] values;
}

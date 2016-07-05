#include <iostream>
#include <unistd.h>
#include <chrono>

#include "utility.hpp"
#include "sortable.hpp"
#include "sequential_quicksort_sort.hpp"
#include "even_odd_sort.hpp"
#include "bucket_sort.hpp"
#include "bitonic_sort.hpp"
#include "insertion_sort.hpp"
#include "shell_sort.hpp"

int main(int argc, char * argv[]) {
	int n = 32000000; //the default number of elements in the array to sort. This can be changed by specifying the -n runtime argument.
	int thread_count = 4; //the default number of threads. This can be changed by the -t runtime argument.
	std::string input_file_path; //where to load data from, if anywhere. If this is not specified, the data will be generated at runtime.
	std::string output_file_path; //where to write out the generated data, if anywhere.
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
	
	
	int* values = new int[n]; //the array to populate and sort
	
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
	
	
	//if running in debug mode, print the values of the array
	if(debug){
		std::cout << "Running in debug mode ... printing array" << std::endl;
		std::cout << "-------" << std::endl;
		utility::print_array(values, n);
		std::cout << "-------" << std::endl;
		std::cout << "done printing array" << std::endl << std::endl;
	}
	
	
	//
	//third - run all sortable implementations.
	//
	
	std::vector<sortable*> sorts;
	sorts.push_back(new sequential_quicksort_sort(thread_count));
	sorts.push_back(new even_odd_sort(thread_count));
	sorts.push_back(new bucket_sort(thread_count));
	sorts.push_back(new bitonic_sort(thread_count));
	sorts.push_back(new shell_sort(thread_count));

	// Only include sequential insertion sort for small data sets
	// sorts.push_back(new insertion_sort(thread_count));
	
	for(std::size_t x=0; x<sorts.size(); x++){
		int* values_copy = new int[n];
		std::copy(values, values + n, values_copy);
		
		std::cout << "-------------------------------------------" << std::endl;
		std::cout << "Testing sort implementation (" << sorts[x]->name() << ")" << std::endl;
		std::cout << std::endl;
		
		std::cout << "Sorting array ... " << std::flush;
		
		std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
		sorts[x]->sort_array(values, n);
		std::chrono::high_resolution_clock::time_point stop_time = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
		
		std::cout << "done in " << utility::duration_string(duration) << std::endl;
		std::cout << std::endl;
		
		std::cout << "Validating sort results ... " << std::flush;
		std::vector<std::string> error_messages;
		int error_count = utility::validate(values, n, error_messages);
		std::cout << ((error_count == 0) ? "correct" : "INCORRECT") << std::endl;
		if(error_count > 0){
			std::cout << "There were " << error_count << " errors.";
			if(error_count > error_messages.size()){
				std::cout << " Displaying only the first " << error_messages.size() << " errors.";
			}
			std::cout << std::endl;
			for(std::string& message : error_messages){
				std::cout << "Error: " << message << std::endl;
			}
		}
		std::cout << "-------------------------------------------" << std::endl << std::endl;
		
		std::copy(values_copy, values_copy + n, values);
		delete[] values_copy;
	}
	
	delete[] values;
}

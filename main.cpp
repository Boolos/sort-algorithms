#include <iostream>
#include <unistd.h>
#include <chrono>

#include "utility.hpp"
#include "sortable.hpp"
#include "sequential_quicksort_sort.hpp"
#include "even_odd_sort.hpp"
#include "even_odd_sort_open_mp.hpp"
#include "bucket_sort.hpp"
#include "bitonic_sort.hpp"
#include "insertion_sort.hpp"
#include "shell_sort.hpp"
#include "shear_sort.hpp"
#include "radix_sort.hpp"

namespace csce {

	/**
	 * This defines the sort implementations to run. To add a sortable implementation,
	 * add an entry to the vector returned by this method.
	 */
	std::vector<sortable*> sortables(int thread_count){
		std::vector<sortable*> sorts;
		sorts.push_back(new sequential_quicksort_sort(thread_count));
		sorts.push_back(new even_odd_sort(thread_count));
		sorts.push_back(new even_odd_sort_open_mp(thread_count));
		sorts.push_back(new bucket_sort(thread_count));
		sorts.push_back(new bitonic_sort(thread_count));
		sorts.push_back(new shell_sort(thread_count, new insertion_sort(thread_count)));
		sorts.push_back(new shear_sort(thread_count));
		sorts.push_back(new radix_sort(thread_count));
	
		return sorts;
	}
	
	bool sort_tuple_comparator (const std::tuple<std::string, long long int, int>& a, const std::tuple<std::string, long long int, int>& b){
		return std::get<1>(a) < std::get<1>(b);
	}
}

int main(int argc, char * argv[]) {
	int n = 32000000; //the default number of elements in the array to sort. This can be changed by specifying the -n runtime argument.
	int thread_count = 4; //the default number of threads. This can be changed by the -t runtime argument.
	std::string input_file_path; //where to load data from, if anywhere. If this is not specified, the data will be generated at runtime.
	std::string output_file_path; //where to write out the generated data, if anywhere.
	int iterations = 1; //the number of times to sort the data
	long long int duration = 0;
	bool debug = false;

	int c;
	while((c = getopt(argc, argv, ":df:n:o:r:t:")) != -1){
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
		
			case 'r':
				if(optarg != NULL){
					iterations = std::stoi(optarg);
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


	int* values_copy = new int[n];
	if(iterations > 1){
		std::cout << "***********************************" << std::endl;
		std::cout << "** Running " << iterations << " iterations" << std::endl;
		std::cout << "***********************************" << std::endl;
		std::cout << std::endl << std::endl;
	}

	std::vector<std::tuple<std::string, long long int, int>> sort_tuples;

	for(int iteration = 0; iteration < iterations; iteration++){
		if(iterations > 1){
			std::cout << "***********************************" << std::endl;
			std::cout << "** Starting iteration " << iteration << std::endl;
			std::cout << "***********************************" << std::endl;
		}
	
		std::vector<sortable*> sorts = csce::sortables(thread_count);
		if(iteration == 0){
			sort_tuples.reserve(sorts.size());
			for(std::size_t x=0; x<sorts.size(); x++){
				sort_tuples.push_back(std::make_tuple(sorts[x]->name(), 0L, 0));
			}
		}
	
		for(std::size_t x=0; x<sorts.size(); x++){
			std::copy(values, values + n, values_copy);
	
			std::cout << "-------------------------------------------" << std::endl;
			std::cout << "Testing sort implementation (" << sorts[x]->name() << ")" << std::endl;
			std::cout << std::endl;
	
			std::cout << "Sorting array ... " << std::flush;
	
			std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
			sorts[x]->sort_array(values_copy, n);
			std::chrono::high_resolution_clock::time_point stop_time = std::chrono::high_resolution_clock::now();
			duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
			std::get<1>(sort_tuples[x]) += duration;
	
			std::cout << "done in " << utility::duration_string(duration) << std::endl;
			std::cout << std::endl;
	
			std::cout << "Validating sort results ... " << std::flush;
			std::vector<std::string> error_messages;
			int error_count = utility::validate(values_copy, n, error_messages);
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
			} else {
				std::get<2>(sort_tuples[x])++;
			}
			std::cout << "-------------------------------------------" << std::endl;
		}
	
		//clean up the sortable pointers
		for(auto& sort : sorts){
			delete sort;
		}
	
		if(iterations > 1){
			std::cout << std::endl;
		}
	}

	if(iterations > 1){
		std::sort(sort_tuples.begin(), sort_tuples.end(), csce::sort_tuple_comparator);
		
		std::cout << std::endl;
		std::cout << "===================================" << std::endl;
		std::cout << "== Run statistics for " << iterations << " runs" << std::endl;
		std::cout << "===================================" << std::endl;
	
		for(std::size_t x=0; x<sort_tuples.size(); x++){
			std::string sort_name;
			long long int sort_duration = 0L;
			int sort_correct = 0;
			std::tie(sort_name, sort_duration, sort_correct) = sort_tuples[x];
			
			std::cout << "==" << std::endl;
			std::cout << "== " << sort_name << std::endl;
			std::cout << "==   Total time: " << utility::duration_string(sort_duration) << std::endl;
			std::cout << "== Average time: " << utility::duration_string(sort_duration / iterations) << std::endl;
			
			long double correct_percent = 100.0L * static_cast<long double>(sort_correct) / static_cast<long double>(iterations);
			std::cout << "==      Correct: " << correct_percent << "%  (" << sort_correct << " / " << iterations << ")" << std::endl;
			std::cout << "==" << std::endl;
		}
		
		std::cout << "===================================" << std::endl;
	}

	delete[] values_copy;
	delete[] values;
}

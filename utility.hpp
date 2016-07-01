#ifndef utility_hpp
#define utility_hpp

#include <iostream>
#include <chrono>
#include <string>
#include <sstream>
#include <fstream>
#include <random>

namespace utility {
	template<typename f, typename... args>
	long long int duration_function(f func, args&&... arg){
		std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
		func(std::forward<args>(arg)...);
		std::chrono::high_resolution_clock::time_point stop_time = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
	}
	
	std::string duration_string(long long int duration);
	
	void fill_random(int array[], int n);
	
	void fill_from_file(int array[], int n, const std::string& file_path);
	
	void write_to_file(int array[], int n, const std::string& file_path);
	
	void print_array(int array[], int n);
	
	bool is_sorted(int array[], int n);
};

#endif /* utility_hpp */

#include "utility.hpp"

std::string utility::duration_string(long long int duration){
	std::stringstream out;
	out << (duration / 1E9) << " seconds";
	return out.str();
}
	
void utility::fill_random(int array[], int n){
	std::random_device rd;
	std::mt19937 mt(rd());
	int min = std::numeric_limits<int>::min();
	int max = std::numeric_limits<int>::max();
	std::uniform_int_distribution<int> dist(min, max);
	for(int x=0; x<n; x++){
		array[x] = dist(mt);
	}
}
	
void utility::fill_from_file(int array[], int n, const std::string& file_path) {
	std::ifstream file(file_path, std::ios::in);
	if(file.is_open()){
		int value;
		int x = 0;
		while(file >> value){
			array[x] = value;
			x++;
			if(x >= n){
				break;
			}
		}
		file.close();
	}
}
	
void utility::write_to_file(int array[], int n, const std::string& file_path){
	std::ofstream file(file_path, std::ios_base::out | std::ios_base::trunc);
	if(file.is_open()){
		for(int x=0; x<n; x++){
			file << array[x] << std::endl;
		}
		file.close();
	}
}
	
void utility::print_array(int array[], int n){
	for(int x=0; x<n; x++){
		std::cout << "array[" << x << "] = " << array[x] << std::endl;
	}
}
	
int utility::validate(int array[], int n, std::vector<std::string>& output_errors){
	int error_count = 0;
	for(int x=1; x<n; x++){
		if(array[x-1] > array[x]){
			if(error_count < 10){
				std::stringstream error;
				error << "validate: array[" << (x-1) << "] = " << array[x-1] << " is not less than or equal to array[" << x << "] = " << array[x] << std::endl;
				output_errors.push_back(error.str());
			}
			error_count++;
		}
	}
	return error_count;
}
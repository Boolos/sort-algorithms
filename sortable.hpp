#ifndef sortable_hpp
#define sortable_hpp

#define CSCE_SORT_DEBUG false

namespace utility {
	template<typename f, typename... args>
	long long int duration_function(f func, args&&... arg){
		std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
		func(std::forward<args>(arg)...);
		std::chrono::high_resolution_clock::time_point stop_time = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
	}
	
	std::string duration_string(long long int duration){
		std::stringstream out;
		out << (duration / 1E9) << " seconds";
		return out.str();
	}
	
	void fill_random(int array[], int n){
		std::random_device rd;
		std::mt19937 mt(rd());
		int min = std::numeric_limits<int>::min();
		int max = std::numeric_limits<int>::max();
		std::uniform_int_distribution<int> dist(min, max);
		for(int x=0; x<n; x++){
			array[x] = dist(mt);
		}
	}
	
	void fill_from_file(int array[], int n, const std::string& file_path) {
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
	
	void write_to_file(int array[], int n, const std::string& file_path){
		std::ofstream file(file_path, std::ios_base::out | std::ios_base::trunc);
		if(file.is_open()){
			for(int x=0; x<n; x++){
				file << array[x] << std::endl;
			}
			file.close();
		}
	}
	
	void print_array(int array[], int n){
		for(int x=0; x<n; x++){
			std::cout << "array[" << x << "] = " << array[x] << std::endl;
		}
	}
	
	bool is_sorted(int array[], int n){
		for(int x=1; x<n; x++){
			if(array[x-1] > array[x]){
				return false;
			}
		}
		return true;
	}
};

class sortable {
public:
	virtual void sort_array(int array[], int n) = 0;
	sortable(int _nthreads = 4);
	int nthreads;
	
};


sortable::sortable(int _nthreads) : nthreads(_nthreads) { }


class even_odd_sort : public sortable {
public:
	even_odd_sort(int _nthreads) : sortable(_nthreads) { }
	
	void sort_array(int array[], int n){
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
		
		if(segment_count < 2){
			//no need to do an even-odd merge if there are not at least two segments to merge
			return;
		}
		
		//now do even-odd merge
		for(int start_index = 0, y=0; y<nthreads; start_index = (start_index + 1) % 2, y++){
			int thread_count = 0;
			for(int x=start_index, max = std::min(nthreads, segment_count) - 1; x < max; x+=2){
				thread_count++;
				threads[x>>1] = std::thread(&even_odd_sort::merge_thread, this, array, indices[x].first, indices[x].second, indices[x+1].second);
			}
			for(int x=0; x<thread_count; x++){
				threads[x].join();
			}
		}
		
		delete[] threads;
		delete[] indices;
	}
	
private:
	std::mutex cout_mutex;
	
	void sort_thread(int array[], int start, int stop){
#if CSCE_SORT_DEBUG
		cout_mutex.lock();
		std::cout << "sort(" << start << ", " << stop << ")" << std::endl;
		cout_mutex.unlock();
#endif
		
		std::sort(array + start, array + stop);
	}
	
	void merge_thread(int array[], int start, int mid, int stop){
		int* tmp = new int[stop - start];
		
		int pa = start;
		int pb = mid;
		int x = 0;
		while(pa < mid && pb < stop){
			int a = array[pa];
			int b = array[pb];
			if(a <= b){
				pa++;
				tmp[x++] = a;
			} else {
				pb++;
				tmp[x++] = b;
			}
		}
		
		while(pa < mid){
			tmp[x++] = array[pa++];
		}
		
		while(pb < stop){
			tmp[x++] = array[pb++];
		}
		
		std::copy(tmp, tmp + (stop - start), array + start);
		
		delete[] tmp;
	}
};



class sequential_quicksort_sort : public sortable {
public:
	sequential_quicksort_sort(int _nthreads) : sortable(_nthreads) { }
	
	void sort_array(int array[], int n){
		std::sort(array, array + n);
	}
};

#endif /* sortable_hpp */

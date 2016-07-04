#include "even_odd_sort.hpp"

even_odd_sort::even_odd_sort(int _nthreads) : sortable(_nthreads) { }

std::string even_odd_sort::name() const {
	return "Even odd sort";
}

void even_odd_sort::sort_array(int array[], int n){
	std::pair<int, int>* indices = new std::pair<int, int>[nthreads];
	std::thread* threads = new std::thread[nthreads]; //4 threads
	int nelements = static_cast<int>(ceil(static_cast<double>(n) / static_cast<double>(nthreads)));  // ceil(7 / 4) = 2
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
	
void even_odd_sort::sort_thread(int array[], int start, int stop){
#if CSCE_SORT_DEBUG
	cout_mutex.lock();
	std::cout << "sort(" << start << ", " << stop << ")" << std::endl;
	cout_mutex.unlock();
#endif
	
	std::sort(array + start, array + stop);
}

void even_odd_sort::merge_thread(int array[], int start, int mid, int stop){
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

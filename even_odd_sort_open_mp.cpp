#include "even_odd_sort_open_mp.hpp"

even_odd_sort_open_mp::even_odd_sort_open_mp(int _nthreads) : sortable(_nthreads) { }

std::string even_odd_sort_open_mp::name() const {
	return "Even odd sort -- OpenMP --";
}

void even_odd_sort_open_mp::sort_array(int array[], int n){
	std::pair<int, int>* indices = new std::pair<int, int>[nthreads];
	std::thread* threads = new std::thread[nthreads]; //4 threads
	int nelements = static_cast<int>(ceil(static_cast<double>(n) / static_cast<double>(nthreads)));  // ceil(7 / 4) = 2
	int start = 0, stop = nelements;
	int segment_count = 0;
    
	for(int x=0; x<nthreads && start < n; x++, start += nelements, stop += nelements) {
		indices[x] = std::make_pair(start, std::min(stop, n));
		segment_count++;
	}

    #pragma omp parallel num_threads(nthreads)
    {
        #pragma omp for 
        for (int x = 0; x < segment_count; x++) {
            even_odd_sort_open_mp::sort_thread(array, indices[x].first, indices[x].second);
        }
    }

	if(segment_count < 2) {
		//no need to do an even-odd merge if there are not at least two segments to merge
		return;
	}

	for(int start_index = 0, y=0; y<nthreads; start_index = (start_index + 1) % 2, y++) {
		int thread_count = 0;
        int max = std::min(nthreads, segment_count) - 1;

        #pragma omp parallel num_threads(nthreads) 
        {
            #pragma omp for
            for(int x=start_index; x < max; x+=2) {
                even_odd_sort_open_mp::merge_thread(array, indices[x].first, indices[x].second, indices[x+1].second);
            }
        }
	}
	
	delete[] threads;
	delete[] indices;
}
	
void even_odd_sort_open_mp::sort_thread(int array[], int start, int stop){
#if CSCE_SORT_DEBUG
	cout_mutex.lock();
	std::cout << "sort(" << start << ", " << stop << ")" << std::endl;
	cout_mutex.unlock();
#endif
	
	std::sort(array + start, array + stop);
}

void even_odd_sort_open_mp::merge_thread(int array[], int start, int mid, int stop){
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

#include "radix_sort.hpp"
#define BASE_BITS 8
#define BASE (1 << BASE_BITS)
#define MASK (BASE-1)
#define DIGITS(v, shift) (((v) >> shift) & MASK)

std::string radix_sort::name() const
{
	return "Radix Sort with Parallel Counting Sort";
}

radix_sort::radix_sort(int _nthreads) : sortable (_nthreads) {}

void radix_sort::sort_array(int array[], int m)
{	
	unsigned* data = (unsigned*) array;
	std::size_t n = (unsigned) m;
    unsigned * buffer = (unsigned*) std::calloc(n, sizeof(unsigned));
    int total_digits = sizeof(unsigned)*8;
 
    //Each thread use local_bucket to move data
    size_t i;
	
    for(int shift = 0; shift < total_digits; shift+=BASE_BITS) {
        size_t bucket[BASE] = {0};
 
        size_t local_bucket[BASE] = {0}; 
		//size needed in each bucket/thread
        //1st pass, scan whole and check the count
		
        #pragma omp parallel firstprivate(local_bucket)
        {
            #pragma omp for schedule(static) nowait
            for(i = 0; i < n; i++){
                local_bucket[DIGITS(data[i], shift)]++;
            }
            
			#pragma omp critical
            for(i = 0; i < BASE; i++) {
                bucket[i] += local_bucket[i];
            }
            #pragma omp barrier
			
            #pragma omp single
            for (i = 1; i < BASE; i++) {
                bucket[i] += bucket[i - 1];
            }
			
            int nthreads = omp_get_num_threads();
            int thread_id = omp_get_thread_num();
			
            for(int current_t = nthreads - 1; current_t >= 0; current_t--) {
				
                if(current_t == thread_id) {
					
                    for(i = 0; i < BASE; i++) {
                        bucket[i] -= local_bucket[i];
                        local_bucket[i] = bucket[i];
                    }
					
                } else { 
                    #pragma omp barrier
                }
 
            }
			
            #pragma omp for schedule(static)
            for(i = 0; i < n; i++) { 
                buffer[local_bucket[DIGITS(data[i], shift)]++] = data[i];
            }
        }
		
        //now copy back the sorted data
        unsigned* tmp = data;
        data = buffer;
        buffer = tmp;
		int* array = (int*) data;
		
    }
	
    free(buffer);
}


#include "bitonic_sort.hpp"
#include <climits>
#include <cstdlib>
#include <iostream>
#include <omp.h>
using namespace std;

struct Data
{
	int *array;
	int n, actualN;
	Data(int *array = NULL, int n = 0, int actualN = 0) : array(array), n(n), actualN(actualN) {}
	int operator[](size_t i) { if(i >= actualN) return INT_MAX; else return array[i]; }
};
Data *data;
int THREADS;

int compare(const void *a, const void *b)
{
	int x = *(int *)a, y = *(int *)b;
	return x > y ? 1 : (x < y ? -1 : 0);
}

void reverse(int *array, int n)
{
	if(n <= 1)
		return;

	for(int i = 0; i < n/2; i++)
		swap(array[i], array[n - 1 - i]);
}

void runThread(size_t id)
{
	Data &p = data[id];
	qsort(p.array, p.n, sizeof(int), compare);
	
	for(int iBit = 1; iBit <= THREADS / 2; iBit *= 2)
	{
		bool outputUp = (id / (iBit * 2)) % 2 == 0;
		size_t pid;
		bool up;
		
		for(int iMerge = iBit; iMerge >= 1; iMerge /= 2)
		{
			#pragma omp barrier
			
			pid	= (id / iMerge) % 2 == 0 ? id + iMerge : id - iMerge;
			up	= (id < pid) == outputUp;
			
			Data &q = data[pid];
			
			int i = 0, j = 0, inc1 = 1, inc2 = 1;
			if(p.array[0] > p.array[p.n-1] == up)
			{
				i = p.n - 1;
				inc1 = -1;
			}
			if(q.array[0] > q.array[q.n-1] == up)
			{
				j = q.n - 1;
				inc2 = -1;
			}
			
			int *tmp = new int[p.n];
			
			for(int k = 0; k < p.n; k++)
			{
				if(j < 0 || j >= q.n || p.array[i] < q.array[j] == up)
				{
					tmp[k] = p.array[i];
					i += inc1;
				}
				else
				{
					tmp[k] = q.array[j];
					j += inc2;
				}
			}
			
			#pragma omp barrier
			
			for(int k = 0; k < p.n; k++)
				p.array[k] = tmp[k];
			
			delete[] tmp;
		}
		
		if(!up)
			reverse(p.array, p.n);
	}
}

bitonic_sort::bitonic_sort(int _nthreads) : sortable(_nthreads)
{
	if(nthreads == 0 || nthreads & (nthreads - 1))
	{
		cerr << "cannot use bitonic sort with a number of processors not equal to a power of two" << endl;
		exit(1);
	}
}

std::string bitonic_sort::name() const
{
	return "Batcher's bitonic sort";
}

void bitonic_sort::sort_array(int _array[], int _n)
{
	THREADS = nthreads;
	const int N = _n;
	
	int paddedN	= THREADS * (N / THREADS + 1);
	int n		= paddedN / THREADS;
	
	data = new Data[THREADS];
	
	#pragma omp parallel num_threads(THREADS)
	{
		size_t i = omp_get_thread_num();
		
		data[i].array	= _array + i * n;
		data[i].n		= n;
		data[i].actualN	= N - (i * n);
		
		runThread(i);
	}
	
	delete[] data;
}
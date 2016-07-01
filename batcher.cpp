/* File:	batcher.cpp
 * Author:	Luke Godfrey
 * Purpose:	A parallel implementation of Batcher's bitonic sort using OpenMP
 */

#include <iostream>
#include <climits>
#include <cstdlib>
#include <omp.h>
using namespace std;

#define N		1000000
#define MAX		1000000
#define THREADS	32

struct Data
{
	int *array;
	int n;
};
Data data[THREADS];

int compare(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

void reverse(int *array, int n)
{
	if(n <= 1)
		return;

	for(int i = 0; i < n/2; i++)
		swap(array[i], array[n - 1 - i]);
}

int *generateData(int n, int padding)
{
	int *array = new int[n + padding];
	for(int i = 0; i < n; i++)
		array[i] = rand() % MAX;
	for(int i = 0; i < padding; i++)
		array[i+n] = MAX;
	return array;
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

int main()
{
	int paddedN	= THREADS * (N / THREADS + 1);
	int n		= paddedN / THREADS;
	int *array	= generateData(N, paddedN - N), *answer;
	
#ifdef DEBUG
	answer = new int[N];
	for(int i = 0; i < N; i++)
		answer[i] = array[i];
	qsort(answer, N, sizeof(int), compare);
#endif
	
	#pragma omp parallel num_threads(THREADS)
	{
		size_t i = omp_get_thread_num();
		
		data[i].array = array + i * n;
		data[i].n = paddedN / THREADS;
		
		runThread(i);
	}
	
#ifdef DEBUG
	bool success = true;
	for(int i = 0; i < N && success; i++)
		if(answer[i] != array[i])
			success = false;
	cout << (success ? "sort succeeded!" : "sort failed!") << endl;
#endif
	
	return 0;
}

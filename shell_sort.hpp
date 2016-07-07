#ifndef shell_sort_hpp
#define shell_sort_hpp

#define CSCE_SORT_DEBUG false

#include <vector>
#include "sortable.hpp"
#include "insertion_sort.hpp"

class shell_sort : public sortable {
public:
    shell_sort(int nthreads, sortable * sequential_sort);
	std::string name() const;
	void sort_array(int array[], int n);
	
private:
    int _gap;
    int _nthreads;
    sortable * _sequential_sort;
    void sort(int array[], int array_size, std::vector<int> gap_sequence);
    void sort_gap_in_parallel(int array[], int array_size, int gap);
    std::vector<int> make_partition(int array[], int array_size, int gap);
    void write_to_array(std::vector<int> partition, int array[], int array_size, int gap);
};

#endif /* shell_sort_hpp */

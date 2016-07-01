#ifndef sortable_hpp
#define sortable_hpp

class sortable {
public:
	virtual void sort_array(int array[], int n) = 0;
	sortable(int _nthreads = 4);
	int nthreads;
	
};

#endif /* sortable_hpp */

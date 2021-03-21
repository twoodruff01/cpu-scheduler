#ifndef MAX_HEAP
#define MAX_HEAP

void down_heap(int array[], int startIndex, int length);
void heap_sort(int array[], int length);
int delete_max(int array[], int length);
void up_heap(int array[], int length);
void construct_heap(int array[], int length);

void swap(int *v1, int *v2);

void fill_heap(int array[], int length);
void print_heap_horizontally(int array[], int length);

#endif

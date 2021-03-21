#ifndef MIN_HEAP
#define MIN_HEAP

typedef struct {
    void *array;
    int array_size;
    int length;
} min_heap;

min_heap *initialise_heap(int initial_size);

void down_heap(int array[], int start_Index, int length);
void heap_sort(int array[], int length);
int delete_min(int array[], int length);
void up_heap(int array[], int length);
void heapify(int array[], int length);

void swap(int *v1, int *v2);

void fill_heap(int array[], int length);
void print_heap_horizontally(int array[], int length);

#endif

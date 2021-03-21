#ifndef MIN_HEAP
#define MIN_HEAP

// Put this somewhere else later !!!
typedef struct {
    int arrival_time;
    int pid;  // priority 2
    int run_time;
    int parallelisable;
    int remaining_run_time;  // priority 1
} process;

typedef struct {
    process **process_array;
    int array_size;
    int length;
} min_heap;


min_heap *initialise_heap(int initial_size);
void push(min_heap **heap, process *new_process);
void swap_process(process *p1, process *p2);

void down_heap(int array[], int start_Index, int length);
void heap_sort(int array[], int length);
int delete_min(int array[], int length);
void up_heap(int array[], int last_index);
void heapify(int array[], int length);

void swap(int *v1, int *v2);

void fill_heap(int array[], int length);
void print_heap_horizontally(int array[], int length);
void keep_adding_to_heap(int array[], int length);

#endif

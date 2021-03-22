#ifndef MIN_HEAP
#define MIN_HEAP

#include <stdbool.h>
#include "../utils.h"

typedef struct {
    process **process_array;
    int array_size;
    int last_index;
} min_heap;


min_heap *initialise_heap(int initial_size);
void push(min_heap **heap, process *new_process);
process *pop(min_heap **heap);
void _down_heap(min_heap **heap, int start_index);
process *peek(min_heap *heap);
bool is_empty(min_heap *heap);
void free_min_heap(min_heap *heap);
void heap_sort(min_heap **heap);
void heapify(min_heap **heap);

void print_heap(min_heap *heap);

#endif

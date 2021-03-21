#ifndef MIN_HEAP
#define MIN_HEAP

#include "stdbool.h"
#include "../utils.h"

typedef struct {
    process **process_array;
    int array_size;
    int last_index;
} min_heap;


min_heap *initialise_heap(int initial_size);
void push(min_heap **heap, process *new_process);
process *pop(min_heap **heap);
void print_process_heap_horizontally(min_heap *heap);
bool is_empty(min_heap *heap);
void free_min_heap(min_heap *heap);
process *peek(min_heap *heap);

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

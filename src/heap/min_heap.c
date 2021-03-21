/*
A basic implementation of a min heap
Basic guide to heaps:
- Don't care if the left child is greater than the right, only about parent and child.
- Root should either be max or min value
- Heap starts from index 1
- i = parent*2      (left child)
- i = parent*2 + 1  (right child)
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include "min_heap.h"
#include "../utils.h"


/*
Create new heap in memory.
initial size is just the initial number of processes you think you might have.
This implementation will allocate more memory for new processes if needed.
*/
min_heap *initialise_heap(int initial_size) {
    min_heap *new_heap = malloc(sizeof(min_heap));
    assert(new_heap);
    
    // This needs to be length+1 or you'll go past the end of the array without realising
    new_heap->process_array = malloc(sizeof(process) * (initial_size + 1));  // ???
    assert(new_heap->process_array);
    new_heap->array_size = initial_size;
    new_heap->last_index = 0;
    return new_heap;
}


/*
Add item to heap.
Will increase size of heap if needed.
*/
void push(min_heap **heap, process *new_process) {

    // Avoid pointer bugs...
    process *new_process_copy = malloc(sizeof(process));
    assert(new_process_copy);
    *new_process_copy = *new_process;

    // Increase index of heap before we try putting anything in it.
    int index = (*heap)->last_index + 1;

    // Check for end of array here and add more memory if needed.
    if (index > (*heap)->array_size) {
        int new_array_size = index * 2;
        (*heap)->process_array = realloc((*heap)->process_array, sizeof(process) * new_array_size);
        assert((*heap)->process_array);
        (*heap)->array_size = new_array_size;
    }

    // Insert at end of heap.
    (*heap)->last_index = index;
    ((*heap)->process_array)[index] = new_process;

    // Upheap: go up through the heap till you find a new spot for the inserted process.
    while (index / 2 > 0) {
        
        process *parent_process = ((*heap)->process_array)[index / 2];

        if (less_than(parent_process, new_process_copy) == true) {
            break;
        } else {
            //swap parent with child
            swap_process((((*heap)->process_array)[index]), (((*heap)->process_array)[index / 2]));
            index /= 2;
        }
    }
    free(new_process_copy);
}


/*
New
*/
// process *pop(min_heap **heap) {
//     ;
// }


/*
Fixes the heap, starting at the root.
startIndex should usually be 1
*/
// void down_heap1(min_heap **heap) {
//     int heap_length = (*heap)->last_index;

//     if (is_empty(*heap) == true) {
//         return;
//     }

//     int parent_index = 1;
//     int child_index;
//     while (parent_index <= heap_length / 2) {

//         child_index = parent_index*2;

//         /*
//         Start at root
//             find smaller child
//                 ->remaining_time
//                 ->pid
//             if smaller child is smaller:
//                 swap
//             else:
//                 break
//         */






//         // Only compare the smaller child (or child with smaller pid)
//         if (child_index < heap_length && ((*heap)->process_array)[child_index] && array[child_index] > array[child_index + 1]) {  // Add second priority here. Could use a function pointer to a comparator ???
//             child_index++;
//         }
//         // Stop when parent smaller then child
//         if (array[parent_index] <= array[child_index]) {
//             break;
//         }
//         swap(&array[parent_index], &array[child_index]);
//         parent_index = child_index;
//     }
// }

/*
Return first process (or null if there aren't any).
*/
process *peek(min_heap *heap) {
    if (is_empty(heap) != false) {
        return (heap->process_array)[1];
    } else {
        return NULL;
    }
}


/*
Returns min node of heap
Then fixes the heap
*/
int delete_min(int array[], int last_value_index) {
    int min = array[1];

    swap(&array[1], &array[last_value_index--]);

    down_heap(array, 1, last_value_index);
    return min;
}


/*
Assumes that we're incrementing and decrementing heap->last_index properly.
*/
bool is_empty(min_heap *heap) {
    if (heap->last_index <= 0) {
        return true;
    } else {
        return false;
    }
}


/*
Assumes that we're incrementing and decrementing heap->last_index properly.
*/
void free_min_heap(min_heap *heap) {
    for (int i = 1; i <= heap->last_index; i++) {
        free((heap->process_array)[i]);
    }
    free(heap->process_array);
    free(heap);
}


// ---------------------------------Helper Functions-------------------------------------------------


/*
Fixes the heap, starting at the root.
startIndex should usually be 1
*/
void down_heap(int array[], int start_index, int length) {
    int parent_index = start_index;
    int child_index;
    while (parent_index <= length / 2) {

        child_index = parent_index*2;
        // Only compare the smaller child
        if (child_index < length && array[child_index] > array[child_index + 1]) {  // Add second priority here. Could use a function pointer to a comparator ???
            child_index++;
        }
        // Stop when parent smaller then child
        if (array[parent_index] <= array[child_index]) {
            break;
        }
        swap(&array[parent_index], &array[child_index]);
        parent_index = child_index;
    }
}


/*
Takes an array after it's had items added to it, and turns it into a heap
*/
void heapify(int array[], int length) {
    // downHeap() for each sub-heap in the heap, starting from 1 level above leaves
    int sub_heap_index = length / 2;
    while (sub_heap_index >= 1) {
        down_heap(array, sub_heap_index, length);
        sub_heap_index--;
    }
}


/*
Sorts in descending order
*/
void heap_sort(int array[], int length) {
    while (length > 1) {
        swap(&array[1], &array[length--]);
        down_heap(array, 1, length);
    }
}


void swap(int *v1, int *v2) {
    int temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}

/*
Just for debugging
*/
void print_process_heap_horizontally(min_heap *heap) {
    int heap_length = heap->last_index;
    printf("[");
    int i = 1;
    for (i = 1; i < heap_length; i++) {
        printf("(%d,%d)", (heap->process_array)[i]->remaining_run_time, (heap->process_array)[i]->pid);
    }
    printf("(%d,%d)]\n", (heap->process_array)[i]->remaining_run_time, (heap->process_array)[i]->pid);
}

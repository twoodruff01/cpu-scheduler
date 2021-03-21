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
#include "min_heap.h"

// TODO:
// - Figure out how void pointer and all that sort of shit work
// - Generalise it
// - Should be able to insert a void pointer, and an int for its priority, and an int for secondary priority
// - Connect to project 1


min_heap *initialise_heap(int initial_size) {

    min_heap *new_heap = malloc(sizeof(min_heap));
    assert(new_heap);

    // This needs to be length+1 or you'll go past the end of the array without realising
    new_heap->process_array = malloc(sizeof(process) * (initial_size + 1));  // ???
    assert(new_heap->process_array);
    new_heap->array_size = initial_size;
    new_heap->length = 0;
    
    return new_heap;
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
Fix heap after inserting new item at the bottom of the heap
*/
void up_heap(int array[], int last_index) {
    // Insert at end of heap, then go up through the heap till you find its place
    int last_value = array[last_index];
    while (last_index / 2 > 0 && array[last_index / 2] >= last_value) {
        swap(&array[last_index], &array[last_index / 2]);
        last_index /= 2;
    }
    // array[length] = lastValue;
}


/*
New
*/
// void free_min_heap(min_heap *heap) {

// }


/*
Add item to heap
*/
void push(min_heap **heap, process *new_process) {

    int priority_1 = new_process->remaining_run_time;
    int priority_2 = new_process->pid;

    // Insert at end of heap. Could check for end of array here and add more memory !!!
    int index = (*heap)->length + 1;
    (*heap)->length = index;
    ((*heap)->process_array)[index] = new_process;

    // Fix heap: go up through the heap till you find a new spot for the last value. (upheap).
    while (index / 2 > 0) {
        
        int parent_priority_1 = ((*heap)->process_array)[index / 2]->remaining_run_time;
        int parent_priority_2 = ((*heap)->process_array)[index / 2]->pid;

        if (parent_priority_1 < priority_1 || (parent_priority_1 == priority_1 && parent_priority_2 < priority_2)) {  // What if PID's are equal too ???) {
            break;
        } else {
            //swap parent with child
            swap_process((((*heap)->process_array)[index]), (((*heap)->process_array)[index / 2]));
            index /= 2;
        }
    }
}


void swap_process(process *p1, process *p2) {
    process temp_process = *p1;
    *p1 = *p2;
    *p2 = temp_process;
}


/*
New
*/
// void* pop(min_heap *heap) {

// }


/*
New
*/
// int is_empty(min_heap *heap) {

// }


/*
Sorts in descending order
*/
void heap_sort(int array[], int length) {
    while (length > 1) {
        swap(&array[1], &array[length--]);
        down_heap(array, 1, length);
    }
}


// ---------------------------------Helper Functions-------------------------------------------------


void swap(int *v1, int *v2) {
    int temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}


/*
Fill heap with random numbers
*/
void fill_heap(int array[], int length) {
    srand(time(0));
    int i;
     // Use <= because we're not using index 0
    for (i = 1; i <= length; i++) {
        array[i] = rand() % 200;
    }
}


/*
Use upheap repeatedly
*/
void keep_adding_to_heap(int array[], int length) {
    srand(time(0));
    int i;
     // Use <= because we're not using index 0
    for (i = 1; i <= length; i++) {
        int new_value = rand() % 200;
        printf("Adding value: %d\n", new_value);
        array[i] = new_value;
        up_heap(array, i);
        print_heap_horizontally(array, length);  // print whole array every time whilst testing
    }
}


/*
Just for debugging
*/
void print_heap_horizontally(int array[], int length) {
    printf("[");
    int i = 1;
    for (i = 1; i < length; i++) {
        printf("%d,", array[i]);
    }
    printf("%d]\n", array[i]);
}


/*
Just for debugging
*/
void print_process_heap_horizontally(min_heap *heap) {
    int heap_length = heap->length;
    printf("[");
    int i = 1;
    for (i = 1; i < heap_length; i++) {
        printf("(%d,%d)", (heap->process_array)[i]->remaining_run_time, (heap->process_array)[i]->pid);
    }
    printf("(%d,%d)]\n", (heap->process_array)[i]->remaining_run_time, (heap->process_array)[i]->pid);
}

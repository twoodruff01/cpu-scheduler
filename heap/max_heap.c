#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "max_heap.h"

/*
TODO:
- Revise this and make sure it actually works
- Generalise it
    Should be able to insert a void pointer, and an int for its priority
- Turn it into a min heap
- Connect it to project 1
*/

/*
Heap:
    - Don't care if the left child is greater than the right, only about parent and child.
    - Root should either be max or min value
    - Heap starts from index 1
    i = parent*2      (left child)
    i = parent*2 + 1  (right child)
*/

/*
Fixes the heap (starting at the root)
startIndex should usually be 1
*/
void down_heap(int array[], int start_index, int length) {
    int parent_index = start_index;
    int child_index;
    while (parent_index <= length / 2) {

        child_index = parent_index*2;
        // Only compare the larger child
        if (child_index < length && array[child_index] < array[child_index + 1]) {
            child_index++;
        }
        // Stop when parent larger then child
        if (array[parent_index] >= array[child_index]) {
            break;
        }
        swap(&array[parent_index], &array[child_index]);
        parent_index = child_index;
    }
}


/*
*/
void heap_sort(int array[], int length) {
    while (length > 1) {
        swap(&array[1], &array[length--]);
        down_heap(array, 1, length);
    }
}


/*
Returns max node of heap
Then fixes the heap
*/
int delete_max(int array[], int last_value_index) {
    int max = array[1];

    swap(&array[1], &array[last_value_index--]);

    down_heap(array, 1, last_value_index);
    return max;
}


/*
Fix heap after inserting new item at the bottom/end of the heap
*/
void up_heap(int array[], int length) {
    // Insert at end of heap, then go up through the heap till you find its place
    int last_value = array[length];
    while (array[length / 2] <= last_value && length > 0) {
        swap(&array[length], &array[length / 2]);
        length /= 2;
    }
    // array[length] = lastValue;
}


/*
Takes an array after it's had items added to it, and turns it into a heap
*/
void construct_heap(int array[], int length) {
    // downHeap() for each sub-heap in the heap, starting from 1 level above leaves
    int subHeapIndex = length / 2;
    while (subHeapIndex >= 1) {
        down_heap(array, subHeapIndex, length);
        subHeapIndex--;
    }
}


/*
New
*/
// void push() {

// }


/*
New
*/
// void* pop() {

// }


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
    for (i = 1; i <= length; i++) { // <= because not using 0 makes things tricky
        array[i] = rand() % 200;
    }
}


void print_heap_horizontally(int array[], int length) {
    printf("[");
    int i = 1;
    for (i = 1; i < length; i++) {
        printf("%d,", array[i]);
    }
    printf("%d]\n", array[i]);
}

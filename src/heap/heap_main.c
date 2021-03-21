#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "min_heap.h"

#define ARRAY_SIZE 20

int main() {

    // this needs to be length+1 or you'll go past the end of the array without realising
    int *numbers = malloc(sizeof(int) * (ARRAY_SIZE + 1));
    assert(numbers);

    // numbers[1] = 5;
    // print_heap_horizontally(numbers, ARRAY_SIZE);
    // up_heap(numbers, 1);
    // print_heap_horizontally(numbers, ARRAY_SIZE);

    keep_adding_to_heap(numbers, ARRAY_SIZE);
    heap_sort(numbers, ARRAY_SIZE);
    print_heap_horizontally(numbers, ARRAY_SIZE);

    // fill_heap(numbers, ARRAY_SIZE);
    // print_heap_horizontally(numbers, ARRAY_SIZE);

    // heapify(numbers, ARRAY_SIZE);
    // print_heap_horizontally(numbers, ARRAY_SIZE);

    // printf("%d\n", delete_min(numbers, ARRAY_SIZE));
    // print_heap_horizontally(numbers, ARRAY_SIZE);
    
    // heap_sort(numbers, ARRAY_SIZE);
    // print_heap_horizontally(numbers, ARRAY_SIZE);

    free(numbers);

    // ---------------------------------------------------------------

    // process *thingo = malloc(sizeof(process));
    // assert(thingo);
    // thingo->remaining_run_time = 5;

    // min_heap *first_heap = initialise_heap(ARRAY_SIZE);
    // push(&first_heap, thingo);

    // printf("%d\n", (first_heap->process_array)[1]->remaining_run_time);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "min_heap.h"

#define ARRAY_SIZE 18

int main() {

    // this needs to be length+1 or you'll go past the end of the array without realising
    int *numbers = malloc(sizeof(int) * (ARRAY_SIZE + 1));
    assert(numbers);



    // upheap testing ------------------------------------------
    // numbers[1] = 5;
    // print_heap_horizontally(numbers, ARRAY_SIZE);
    // up_heap(numbers, 1);
    // print_heap_horizontally(numbers, ARRAY_SIZE);

    // keep_adding_to_heap(numbers, ARRAY_SIZE);
    // heap_sort(numbers, ARRAY_SIZE);
    // print_heap_horizontally(numbers, ARRAY_SIZE);
    // upheap testing ------------------------------------------



    // downheap heapify heap_sort testing ----------------------
    // fill_heap(numbers, ARRAY_SIZE);
    // print_heap_horizontally(numbers, ARRAY_SIZE);

    // heapify(numbers, ARRAY_SIZE);
    // print_heap_horizontally(numbers, ARRAY_SIZE);

    // printf("%d\n", delete_min(numbers, ARRAY_SIZE));
    // print_heap_horizontally(numbers, ARRAY_SIZE);
    
    // heap_sort(numbers, ARRAY_SIZE);
    // print_heap_horizontally(numbers, ARRAY_SIZE);
    // downheap heapify heap_sort testing ----------------------
    free(numbers);



    // ---------------------------------------------------------------

    // process *thingo = malloc(sizeof(process));
    // assert(thingo);
    // thingo->remaining_run_time = 5;
    
    min_heap *first_heap = initialise_heap(ARRAY_SIZE);

    srand(time(0));
    int i;
     // Use <= because we're not using index 0
    for (i = 1; i <= ARRAY_SIZE; i++) {
        // Create new process
        process *thingo = malloc(sizeof(process));
        assert(thingo);
        thingo->remaining_run_time = rand() % 200;
        thingo->pid = rand() % 200;

        printf("Adding process: run_time=%d pid=%d\n", thingo->remaining_run_time, thingo->pid);
        push(&first_heap, thingo);
        
        print_process_heap_horizontally(first_heap);
    }


    // min_heap *first_heap = initialise_heap(ARRAY_SIZE);
    // push(&first_heap, thingo);

    // printf("%d\n", (first_heap->process_array)[1]->remaining_run_time);

    return 0;
}

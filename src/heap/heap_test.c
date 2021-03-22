#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>
#include "min_heap.h"

#define ARRAY_SIZE 17

int main() {
    srand(time(0));
    
    min_heap *first_heap = initialise_heap(ARRAY_SIZE);

     // Use <= because we're not using index 0
    for (int i = 1; i <= ARRAY_SIZE; i++) {
        // Create new process
        process *thingo = malloc(sizeof(process));
        assert(thingo);
        thingo->remaining_run_time = rand() % 10;
        thingo->pid = rand() % 200;

        printf("Adding process: run_time=%d pid=%d\n", thingo->remaining_run_time, thingo->pid);
        push(&first_heap, thingo);
        
        print_heap(first_heap);
    }

    // printf("Testing pop\n");
    // for (int i = 1; i <= ARRAY_SIZE; i++) {
    //     if (is_empty(first_heap) != true) {
    //         print_process(pop(&first_heap));  // Will cause memory leaks if used this way.
    //         printf("\n");
    //     }
    //     if (is_empty(first_heap) != true) {
    //         print_heap(first_heap);
    //     }
    // }

    heap_sort(&first_heap);
    printf("first_heap sorted: ");
    print_heap(first_heap);

    heapify(&first_heap);
    printf("first_heap heapified: ");
    print_heap(first_heap);
    printf("End of first_heap stuff\n");

    min_heap *second_heap = initialise_heap(ARRAY_SIZE);
    while (is_empty(first_heap) != true) {
        push(&second_heap, pop(&first_heap));
    }

    printf("second_heap: ");
    print_heap(second_heap);

    heap_sort(&second_heap);
    printf("second_heap sorted: ");
    print_heap(second_heap);

    free_min_heap(first_heap);
    free_min_heap(second_heap);

    return 0;
}

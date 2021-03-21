#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "min_heap.h"

#define ARRAY_SIZE 18

int main() {
    
    min_heap *first_heap = initialise_heap(ARRAY_SIZE);

    srand(time(0));
    int i;
     // Use <= because we're not using index 0
    for (i = 1; i <= ARRAY_SIZE; i++) {
        // Create new process
        process *thingo = malloc(sizeof(process));
        assert(thingo);
        thingo->remaining_run_time = rand() % 10;
        thingo->pid = rand() % 200;

        printf("Adding process: run_time=%d pid=%d\n", thingo->remaining_run_time, thingo->pid);
        push(&first_heap, thingo);
        
        print_process_heap_horizontally(first_heap);
    }

    free_min_heap(first_heap);


    return 0;
}

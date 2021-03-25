#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>

#include "cpu.h"

#define ARRAY_SIZE 17

int main() {
    srand(time(0));
    
    cpu *first_heap = initialise_cpu(ARRAY_SIZE, 0);

     // Use <= because we're not using index 0
    for (int i = 1; i <= ARRAY_SIZE; i++) {
        // Create new process
        process *thingo = malloc(sizeof(process));
        assert(thingo);
        thingo->remaining_run_time = rand() % 10;
        sprintf(thingo->pid,"%d", rand() % 200);

        printf("Adding process: run_time=%d pid=%s\n", thingo->remaining_run_time, thingo->pid);
        cpu_push(&first_heap, thingo);
        
        print_cpu(first_heap);
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

    cpu_sort(&first_heap);
    printf("first_heap sorted: ");
    print_cpu(first_heap);

    cpu_heapify(&first_heap);
    printf("first_heap heapified: ");
    print_cpu(first_heap);
    printf("End of first_heap stuff\n");

    cpu *second_heap = initialise_cpu(ARRAY_SIZE, 1);
    while (cpu_is_empty(first_heap) != true) {
        cpu_push(&second_heap, cpu_pop(&first_heap));
    }

    printf("second_heap: ");
    print_cpu(second_heap);

    cpu_sort(&second_heap);
    printf("second_heap sorted: ");
    print_cpu(second_heap);

    while (cpu_is_empty(second_heap) != true) {
        free(cpu_pop(&second_heap));
    }
    free_cpu(first_heap);
    free_cpu(second_heap);

    return 0;
}

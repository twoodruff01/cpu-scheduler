/*
multicore is just a min_heap of cpu's, sorted on:
- total remaining_run_time
- cpu id
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../utils.h"
#include "multicore.h"
#include "cpu.h"


/*
Create new cores in memory.
cpu_quantity is the number of cpu's to use.
cpu_quantity should be > 0
*/
multicore *initialise_cores(int cpu_quantity) {
    multicore *new_multicore = malloc(sizeof(multicore));
    assert(new_multicore);
    new_multicore->array_size = cpu_quantity;
    new_multicore->last_index = 0;
    
    // This needs to be length+1 or you'll go past the end of the array without realising
    new_multicore->cpu_array = malloc(sizeof(cpu) * (cpu_quantity + 1));
    assert(new_multicore->cpu_array);

    // Create all the cpu's and add them to the heap.
    for (int i = 0; i < cpu_quantity; i++) {
        cpu *new_cpu = initialise_cpu(INITIAL_PROCESSES_BUDGET, i);
        multicore_push(&new_multicore, new_cpu);
    }

    return new_multicore;
}


/*
Add cpu to heap.
*/
void multicore_push(multicore **cores, cpu *new_cpu) {

    // Increase index of heap before we try putting anything in it.
    int index = (*cores)->last_index + 1;

    // Insert at end of heap.
    (*cores)->last_index = index;
    ((*cores)->cpu_array)[index] = new_cpu;

    // Upheap: go up through the heap till you find a new spot for the inserted cpu.
    while (index / 2 > 0) {
        cpu *parent_cpu = ((*cores)->cpu_array)[index / 2];
        if (cpu_less_than(parent_cpu, new_cpu) == true) {
            break;
        } else {
            //swap parent with child.
            swap_cpu_pointers(&(((*cores)->cpu_array)[index]), &(((*cores)->cpu_array)[index / 2]));
            index /= 2;
        }
    }
}


/*
Adds process to the highest priority cpu on the multicore, then fixes the heap
*/
void multicore_add_process(multicore **cores, process *new_process) {
    cpu *first_cpu = ((*cores)->cpu_array)[1];
    cpu_push(&first_cpu, new_process);
    // Fix the heap.
    _multicore_downheap(cores, 1, cpu_less_than);
}


/*
Returns true if any of the cpu's on the multicore still contain a process.
*/
bool multicore_has_process(multicore *cores) {
    for (int i = cores->last_index; i >= 1; i--) {
        if (cpu_is_empty((cores->cpu_array)[i]) != true) {
            return true;
        }
    }
    return false;
}


/*
Assumes that we're incrementing and decrementing heap->last_index properly.
*/
void free_cores(multicore *cores) {
    for (int i = 1; i <= cores->last_index; i++) {
        free_cpu((cores->cpu_array)[i]);
    }
    free(cores->cpu_array);
    free(cores);
}


/*
Sorts in descending order. (using heap_sort)
Turns the heap into a glorified sorted list.
*/
void multicore_sort(multicore **cores, bool on_pid) {
    int initial_length = (*cores)->last_index;
    while ((*cores)->last_index > 1) {
        swap_cpu_pointers(&(((*cores)->cpu_array)[1]), &(((*cores)->cpu_array)[(*cores)->last_index--]));
        if (on_pid == true) {
            _multicore_downheap(cores, 1, cpu_pid_less_than);
        } else {
            _multicore_downheap(cores, 1, cpu_less_than);
        }
    }
    (*cores)->last_index = initial_length;
}


/*
Turns a heap that has been sorted by heap_sort() back into a heap.
The heap might not be exactly the same as the previous one, since children only have to
be smaller than their parents and this could change between building the same heap.
*/
void multicore_heapify(multicore **cores, bool on_pid) {
    // downHeap() for each sub-heap in the heap, starting from 1 level above leaves
    int sub_heap_index = (*cores)->last_index / 2;
    while (sub_heap_index >= 1) {
        if (on_pid == true) {
            _multicore_downheap(cores, sub_heap_index, cpu_pid_less_than);
        } else {
            _multicore_downheap(cores, sub_heap_index, cpu_less_than);
        }
        sub_heap_index--;
    }
}


/*
Fixes the heap, starting at start_index.
Should generally be called with start_index=1 if you haven't already
called it on the heap recently.
*/
void _multicore_downheap(multicore **cores, int start_index, bool (*less_than)(cpu *, cpu *)) {
    int heap_length = (*cores)->last_index;

    // Avoid bugs.
    if ((*cores)->last_index <= 0) {
        return;
    }

    int parent_index = start_index;
    int child_index;
    while (parent_index <= heap_length / 2) {

        child_index = parent_index*2;

        // Only compare the smaller child.
        cpu *child_1 = ((*cores)->cpu_array)[child_index];
        cpu *child_2 = ((*cores)->cpu_array)[child_index + 1];
        if ((child_index < heap_length) && (child_2 != 0) && (*less_than)(child_2, child_1) == true) {
            child_index++;
        }

        // Stop when parent smaller then child
        if ((*less_than)(((*cores)->cpu_array)[parent_index], ((*cores)->cpu_array)[child_index]) == true) {
            break;
        }
        
        swap_cpu_pointers(&(((*cores)->cpu_array)[parent_index]), &(((*cores)->cpu_array)[child_index]));
        parent_index = child_index;
    }
}


/*
Just for debugging
*/
void print_multicore(multicore *cores) {
    for (int i = 1; i <= cores->last_index; i++) {
        print_cpu(cores->cpu_array[i]);
    }
    if (cores->last_index <= 0) {
        printf("How is this possible?\n");
    }
}

/*
mulitcore is just a min_heap of cpu's, sorted on:
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
initial_size is the number of cpu's you want to budget for.
*/
multicore *initialise_cores(int initial_size) {
    multicore *new_multicore = malloc(sizeof(multicore));
    assert(new_multicore);
    
    // This needs to be length+1 or you'll go past the end of the array without realising
    new_multicore->cpu_array = malloc(sizeof(cpu) * (initial_size + 1));
    assert(new_multicore->cpu_array);
    new_multicore->array_size = initial_size;
    new_multicore->last_index = 0;
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
Returns a copy of the first cpu on the heap, then fixes the heap.
This copy will need to be freed later.
Doesn't check if heap is empty: call is_empty() separately to check.
Same thing as delete_min()
*/
cpu *multicore_pop(multicore **cores) {
    cpu *min_cpu = malloc(sizeof(cpu));
    assert(min_cpu);
    *min_cpu = *(((*cores)->cpu_array)[1]);

    // Put the last cpu at the front.
    swap_cpu_pointers(&(((*cores)->cpu_array)[1]), &(((*cores)->cpu_array)[(*cores)->last_index]));  // check this !!!

    // Free the old first process.
    free(((*cores)->cpu_array)[(*cores)->last_index]);

    // Fix heap.
    (*cores)->last_index--;
    _multicore_downheap(cores, 1);

    return min_cpu;
}


/*
Return first process (or null if there aren't any).
BE CAREFUL MODIFYING THE RETURNED PROCESS.
*/
cpu *multicore_peek(multicore *cores) {
    if (multicore_is_empty(cores) != true) {
        return (cores->cpu_array)[1];
    } else {
        return NULL;
    }
}


/*
Assumes that we're incrementing and decrementing heap->last_index properly.
*/
bool multicore_is_empty(multicore *cores) {
    if (cores->last_index <= 0) {
        return true;
    } else {
        return false;
    }
}


/*
Assumes that we're incrementing and decrementing heap->last_index properly.
*/
void free_cores(multicore *cores) {
    for (int i = 1; i <= cores->last_index; i++) {  // Need to test if this works with only one process on the heap !!!
        free_cpu((cores->cpu_array)[i]);
    }
    free(cores->cpu_array);
    free(cores);
}


/*
Sorts in descending order. (using heap_sort)
Turns the heap into a glorified sorted list of processes.
*/
void multicore_sort(multicore **cores) {
    int initial_length = (*cores)->last_index;
    while ((*cores)->last_index > 1) {
        swap_cpu_pointers(&(((*cores)->cpu_array)[1]), &(((*cores)->cpu_array)[(*cores)->last_index--]));  // check this !!!
        _multicore_downheap(cores, 1);
    }
    (*cores)->last_index = initial_length;
}


/*
Turns a heap that has been sorted by heap_sort() back into a heap.
The heap might not be exactly the same as the previous one, since children only have to
be smaller than their parents and this could change between building the same heap.
*/
void multicore_heapify(multicore **cores) {
    // downHeap() for each sub-heap in the heap, starting from 1 level above leaves
    int sub_heap_index = (*cores)->last_index / 2;
    while (sub_heap_index >= 1) {
        _multicore_downheap(cores, sub_heap_index);
        sub_heap_index--;
    }
}


/*
Fixes the heap, starting at start_index.
Should generally be called with start_index=1 if you haven't already
called it on the heap recently.
*/
void _multicore_downheap(multicore **cores, int start_index) {
    int heap_length = (*cores)->last_index;

    // Avoid bugs.
    if (multicore_is_empty(*cores) == true) {
        return;
    }

    int parent_index = start_index;
    int child_index;
    while (parent_index <= heap_length / 2) {

        child_index = parent_index*2;

        // Only compare the smaller child.
        cpu *child_1 = ((*cores)->cpu_array)[child_index];
        cpu *child_2 = ((*cores)->cpu_array)[child_index + 1];
        if ((child_index < heap_length) && (child_2 != 0) && cpu_less_than(child_2, child_1) == true) {  // Bug if child_2 doesn't exist maybe ???
            child_index++;
        }

        // Stop when parent smaller then child
        if (cpu_less_than(((*cores)->cpu_array)[parent_index], ((*cores)->cpu_array)[child_index]) == true) {
            break;
        }
        
        swap_cpu_pointers(&(((*cores)->cpu_array)[parent_index]), &(((*cores)->cpu_array)[child_index]));  // check this !!!
        parent_index = child_index;
    }
}

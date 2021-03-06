/*
cpu is just a min_heap of processes sorted on:
- remaining_run_time
- pid
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "../utils.h"
#include "cpu.h"


/*
Create new cpu in memory.
initial_size is the number of processes you want to budget for.
This implementation will allocate more memory for new processes if needed.
*/
cpu *initialise_cpu(int initial_size, int id) {
    cpu *new_cpu = malloc(sizeof(cpu));
    assert(new_cpu);
    
    // This needs to be length+1 or you'll go past the end of the array without realising
    new_cpu->process_array = malloc(sizeof(process) * (initial_size + 1));
    assert(new_cpu->process_array);
    new_cpu->array_size = initial_size;
    new_cpu->last_index = 0;

    new_cpu->cpu_id = id;
    new_cpu->total_remaining_run_time = 0;
    return new_cpu;
}


/*
Add item to heap.
Will increase size of heap if needed.
If the process that is currently first ends up not being first, this will update its -
is_running variable to false.
*/
void cpu_push(cpu **current_cpu, process *new_process) {

    // Update cpu's workload.
    (*current_cpu)->total_remaining_run_time += new_process->remaining_run_time;

    // Keep track of whether the process at the front ends up being the same.
    // Assume it won't be, then check later on and if it is, change value back to true.
    char *previous_process_pid = NULL;
    bool previous_process_run_status = false;
    if (cpu_is_empty(*current_cpu) != true) {
        previous_process_pid = ((*current_cpu)->process_array[1])->pid;
        previous_process_run_status = ((*current_cpu)->process_array)[1]->is_running;
        ((*current_cpu)->process_array)[1]->is_running = false;
    }

    // Increase index of heap before we try putting anything in it.
    int index = (*current_cpu)->last_index + 1;

    // Check if the spot for the new process is off the end of the array, and add more memory if needed.
    if (index > (*current_cpu)->array_size) {
        (*current_cpu)->array_size = index * 2;
        (*current_cpu)->process_array = realloc((*current_cpu)->process_array, sizeof(process) * ((*current_cpu)->array_size));
        assert((*current_cpu)->process_array);
    }

    // Insert at end of heap.
    (*current_cpu)->last_index = index;
    ((*current_cpu)->process_array)[index] = new_process;

    // Upheap: go up through the heap till you find a new spot for the inserted process.
    while (index / 2 > 0) {
        process *parent_process = ((*current_cpu)->process_array)[index / 2];
        if (process_less_than(parent_process, new_process) == true) {
            break;
        } else {
            //swap parent with child.
            swap_process_pointers(&(((*current_cpu)->process_array)[index]), &(((*current_cpu)->process_array)[index / 2]));
            index /= 2;
        }
    }

    // If the same process is still at the front, change that process's is_running value back to what it was before.
    if (previous_process_pid != NULL && strcmp(((*current_cpu)->process_array)[1]->pid, previous_process_pid) == 0) {
        ((*current_cpu)->process_array)[1]->is_running = previous_process_run_status;
    }
}


/*
Returns a copy of the first process on the heap, then fixes the heap.
This copy will need to be freed later.
Doesn't check if heap is empty: call is_empty() separately to check.
Same thing as delete_min()
*/
process *cpu_pop(cpu **current_cpu) {
    process *min_process = malloc(sizeof(process));
    assert(min_process);
    *min_process = *(((*current_cpu)->process_array)[1]);

    // Put the last process at the front.
    swap_process_pointers(&(((*current_cpu)->process_array)[1]), &(((*current_cpu)->process_array)[(*current_cpu)->last_index]));

    // Free the old first process.
    free(((*current_cpu)->process_array)[(*current_cpu)->last_index]);

    // Fix heap.
    (*current_cpu)->last_index--;
    _cpu_downheap(current_cpu, 1);

    return min_process;
}


/*
Return first process (or null if there aren't any).
BE CAREFUL MODIFYING THE RETURNED PROCESS.
*/
process *cpu_peek(cpu *current_cpu) {
    if (cpu_is_empty(current_cpu) != true) {
        return (current_cpu->process_array)[1];
    } else {
        return NULL;
    }
}


/*
Assumes that we're incrementing and decrementing heap->last_index properly.
*/
bool cpu_is_empty(cpu *current_cpu) {
    if (current_cpu->last_index <= 0) {
        return true;
    } else {
        return false;
    }
}


/*
Assumes that we're incrementing and decrementing heap->last_index properly.
Doesn't free processes on the cpu. If there are processes on it, you shouldn't be freeing it.
*/
void free_cpu(cpu *current_cpu) {

    if (cpu_is_empty(current_cpu) != true) {
        // printf("\n\n\nYou're freeing a CPU with processes still on it.\n\n\n");
    }
    free(current_cpu->process_array);
    free(current_cpu);
}


/*
Sorts in descending order. (using heap_sort)
Turns the heap into a glorified sorted list of processes.
*/
void cpu_sort(cpu **current_cpu) {
    int initial_length = (*current_cpu)->last_index;
    while ((*current_cpu)->last_index > 1) {
        swap_process_pointers(&(((*current_cpu)->process_array)[1]), &(((*current_cpu)->process_array)[(*current_cpu)->last_index--]));
        _cpu_downheap(current_cpu, 1);
    }
    (*current_cpu)->last_index = initial_length;
}


/*
Turns a heap that has been sorted by heap_sort() back into a heap.
The heap might not be exactly the same as the previous one, since children only have to
be smaller than their parents and this could change between building the same heap.
*/
void cpu_heapify(cpu **current_cpu) {
    // downHeap() for each sub-heap in the heap, starting from 1 level above leaves
    int sub_heap_index = (*current_cpu)->last_index / 2;
    while (sub_heap_index >= 1) {
        _cpu_downheap(current_cpu, sub_heap_index);
        sub_heap_index--;
    }
}


/*
Returns true if first cpu is less than the second.
Bases this on:
1. Total remaining_run_time
2. CPU id
*/
bool cpu_less_than(cpu *c1, cpu *c2) {
    return (c1->total_remaining_run_time < c2->total_remaining_run_time || (c1->total_remaining_run_time == c2->total_remaining_run_time && c1->cpu_id < c2->cpu_id));
}


/*
Returns true if first cpu's pid is less than the second.
*/
bool cpu_pid_less_than(cpu *c1, cpu *c2) {
    return (c1->cpu_id < c2->cpu_id);
}


void swap_cpu_pointers(cpu **c1, cpu **c2) {
    cpu *temp_cpu = *c1;
    *c1 = *c2;
    *c2 = temp_cpu;
}


/*
Just for debugging
*/
void print_cpu(cpu *current_cpu) {
    int heap_length = current_cpu->last_index;

    if (cpu_is_empty(current_cpu) != true) {
        printf("CPU:%d[", current_cpu->cpu_id);
        int i = 1;
        for (i = 1; i < heap_length; i++) {
            printf("(%d,%s)", (current_cpu->process_array)[i]->remaining_run_time, (current_cpu->process_array)[i]->pid);
        }
        printf("(%d,%s)]\n", (current_cpu->process_array)[i]->remaining_run_time, (current_cpu->process_array)[i]->pid);
    } else {
        printf("CPU:%d[EMPTY]\n", current_cpu->cpu_id);
    }
}


/*
Fixes the heap, starting at start_index.
Should generally be called with start_index=1 if you haven't already
called it on the heap recently.
*/
void _cpu_downheap(cpu **current_cpu, int start_index) {
    int heap_length = (*current_cpu)->last_index;

    // Avoid bugs.
    if (cpu_is_empty(*current_cpu) == true) {
        return;
    }

    int parent_index = start_index;
    int child_index;
    while (parent_index <= heap_length / 2) {

        child_index = parent_index*2;

        // Only compare the smaller child.
        process *child_1 = ((*current_cpu)->process_array)[child_index];
        process *child_2 = ((*current_cpu)->process_array)[child_index + 1];
        if ((child_index < heap_length) && (child_2 != 0) && process_less_than(child_2, child_1) == true) {
            child_index++;
        }

        // Stop when parent smaller then child
        if (process_less_than(((*current_cpu)->process_array)[parent_index], ((*current_cpu)->process_array)[child_index]) == true) {
            break;
        }
        
        swap_process_pointers(&(((*current_cpu)->process_array)[parent_index]), &(((*current_cpu)->process_array)[child_index]));
        parent_index = child_index;
    }
}

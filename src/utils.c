#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "utils.h"


/*
If you have a pointer already pointing at one of these processes, after
this call, it will point to the other. Not ideal... Unavoidable ???
*/
void swap_process(process *p1, process *p2) {
    process temp_process = *p1;
    *p1 = *p2;
    *p2 = temp_process;
}


/*
Returns true if the first 'process' is less than the second.
Bases this first on remaining_time, and second on PID.
Assumes that PID's will never be equal. ???
*/
bool less_than(process *p1, process *p2) {
    int p1_remaining_run_time = p1->remaining_run_time;
    int p1_pid = p1->pid;

    int p2_remaining_run_time = p2->remaining_run_time;
    int p2_pid = p2->pid;

    if (p1_remaining_run_time < p2_remaining_run_time || (p1_remaining_run_time == p2_remaining_run_time && p1_pid < p2_pid)) {
        return true;
    } else {
        return false;
    }
}


void print_process(process *p) {
    printf("(%d,%d)\n", p->remaining_run_time, p->pid);
}


/*
Creates a process using the given values.
Note that parallelisable is converted to boolean here.
*/
process *process_from_row(int arrival_time, int pid, int run_time, char parallelisable_char) {
    process *new_process = malloc(sizeof(process));
    assert(new_process);
    new_process->arrival_time = arrival_time;
    new_process->pid = pid;
    new_process->run_time = run_time;

    // Boolean is much easier to work with:
    if (parallelisable_char == 'n') {
        new_process->is_parallelisable = false;
    } else if (parallelisable_char == 'p') {
        new_process->is_parallelisable = true;
    } else {
        printf("Strange parallelisable input given in file: %c\n", parallelisable_char);
    }

    // Process hasn't been given any CPU time yet:
    new_process->remaining_run_time = run_time;

    return new_process;
}

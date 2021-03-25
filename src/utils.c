/*
functions and data types that get used everywhere
*/
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"


/*
Moves the actual process location in memory.
*/
void swap_process(process *p1, process *p2) {
    process temp_process = *p1;
    *p1 = *p2;
    *p2 = temp_process;
}


void swap_process_pointers(process **p1, process **p2) {
    process *temp_process = *p1;
    *p1 = *p2;
    *p2 = temp_process;
}


/*
Returns true if the first 'process' is less than the second.
Bases this first on remaining_time, and second on PID.
Assumes that PID's will never be equal.
*/
bool process_less_than(process *p1, process *p2) {

    // TODO: use string comparison for pid
    int p1_remaining_run_time = p1->remaining_run_time;
    int p1_pid = atoi(p1->pid);

    int p2_remaining_run_time = p2->remaining_run_time;
    int p2_pid = atoi(p2->pid);

    if (p1_remaining_run_time < p2_remaining_run_time || (p1_remaining_run_time == p2_remaining_run_time && p1_pid < p2_pid)) {
        return true;
    } else {
        return false;
    }
    return true;
}


void print_process_running(process *p, int current_time, int cpu_id) {
    printf("%d,RUNNING,pid=%s,remaining_time=%d,cpu=%d\n", current_time, p->pid, p->remaining_run_time, cpu_id);
}


void print_process_finished(process *p, int current_time, int processes_remaining) {
    printf("%d,FINISHED,pid=%s,proc_remaining=%d\n", current_time, p->pid, processes_remaining);
}


/*
Creates a process using the given values.
Note that parallelisable is converted to boolean here.
*/
process *process_from_row(int arrival_time, char *pid, int run_time, char parallelisable_char) {
    process *new_process = malloc(sizeof(process));
    assert(new_process);
    new_process->arrival_time = arrival_time;
    strcpy(new_process->pid, pid);
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


/*
The number of sub-processes should be the greater of K processors and x execution time
*/
int max(int a, int b) {
    if (a >= b) {
        return a;
    } else {
        return b;
    }
}


/*
Just for debugging.
*/
void print_process(process *p) {
    printf("(%d,%s)\n", p->remaining_run_time, p->pid);
}

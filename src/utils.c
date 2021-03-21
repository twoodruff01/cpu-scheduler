#include "stdbool.h"
#include "utils.h"

void swap_process(process *p1, process *p2) {
    process temp_process = *p1;
    *p1 = *p2;
    *p2 = temp_process;
}


/*
Returns true if the first 'process' is less than the second.
Bases this first on remaining_time, and second on PID.
WHY ISN'T THIS FUCKING WORKING ??????????????????????????????
*/
bool less_than(process *p1, process *p2) {
    int p1_remaining_run_time = p1->remaining_run_time;
    int p1_pid = p1->pid;

    int p2_remaining_run_time = p2->remaining_run_time;
    int p2_pid = p2->pid;

    if (p1_remaining_run_time < p2_remaining_run_time || (p1_remaining_run_time == p2_remaining_run_time && p1_pid < p2_pid)) {  // What if PID's are equal too ???
        return true;
    } else {
        return false;
    }
}


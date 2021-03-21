#ifndef UTILS
#define UTILS

typedef struct {
    int arrival_time;
    int pid;  // priority 2
    int run_time;
    int parallelisable;
    int remaining_run_time;  // priority 1
} process;

void swap_process(process *p1, process *p2);
bool less_than(process *p1, process *p2);

#endif

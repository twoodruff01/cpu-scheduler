#ifndef UTILS
#define UTILS

#define MAX_PROCESSORS 1024
#define INITIAL_PROCESSES_BUDGET 100
#define PID_MAX_LENGTH 30

typedef struct {
    int arrival_time;
    char pid[PID_MAX_LENGTH];  // priority 2
    int run_time;
    bool is_parallelisable;
    int remaining_run_time;  // priority 1

    bool is_sub_process;

} process;


void swap_process(process *p1, process *p2);
void swap_process_pointers(process **p1, process **p2);
bool process_less_than(process *p1, process *p2);
bool pid_less_than(char *pid1, char *pid2);
process *process_from_row(int arrival_time, char *pid, int run_time, char parallelisable_char);
void print_process_running(process *p, int current_time, int cpu_id);
void print_process_finished(process *p, int current_time, int processes_remaining);
void print_process(process *p);
int max(int a, int b);

int *_string_to_int_array(char *input);

#endif

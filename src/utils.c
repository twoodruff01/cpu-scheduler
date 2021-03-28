/*
functions and data types that get used everywhere
*/
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
Bases this 
- first on remaining_time
- second on PID
Assumes that PID's will never be equal.
*/
bool process_less_than(process *p1, process *p2) {
    return (p1->remaining_run_time < p2->remaining_run_time || (p1->remaining_run_time == p2->remaining_run_time && pid_less_than(p1->pid, p2->pid)));
}


/*
Account for the fact that we have two different types of pid's
PID can either be:
- 4.523 (float as a string)
- 3 (integer as a string)
Assume that pid's will never be the same.
*/
bool pid_less_than(char *pid1, char *pid2) {

    // Parse strings and split into two arrays of two.
    int *first_pid = _string_to_int_array(pid1);
	int *second_pid = _string_to_int_array(pid2);

	bool result;
	if (first_pid[0] < second_pid[0]) {
		// Just compare first values
		result = true;
	} else if (first_pid[0] == second_pid[0]) {
		// Since there's no duplicate pid's, the only time we need to look
		// at the second number is if we're comparing sub-processes - which
		// should have decimal values (and not be equal).
		result = (first_pid[1] < second_pid[1]);
	} else {
		result = false;
	}

	free(first_pid);
	free(second_pid);
	return result;
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
        // TODO: remove.
        printf("Strange parallelisable input given in file: %c\n", parallelisable_char);
    }

    // Process hasn't been given any CPU time yet:
    new_process->remaining_run_time = run_time;
    new_process->is_running = false;
    new_process->is_sub_process = false;
    new_process->parent_finished_printed = false;

    return new_process;
}


/*
Creates a sub process using a parent process.
*/
process *create_sub_process(process *parent, int execution_time, int sub_number) {

    process *new_sub_process = malloc(sizeof(process));
    assert(new_sub_process);

    // Turn sub_number into a string.
    char sub_number_string[PID_MAX_LENGTH] = "";
    sprintf(sub_number_string, "%d", sub_number);

    char new_id[PID_MAX_LENGTH] = "";
    strcat(new_id, parent->pid);
    strcat(new_id, ".");
    strcat(new_id, sub_number_string);
    strcpy(new_sub_process->pid, new_id);

    new_sub_process->arrival_time = parent->arrival_time;
    new_sub_process->run_time = execution_time;
    new_sub_process->is_parallelisable = false;
    new_sub_process->remaining_run_time = execution_time;

    new_sub_process->is_running = false;
    new_sub_process->is_sub_process = true;
    new_sub_process->parent_finished_printed = false;

    return new_sub_process;
}


void print_process_running(process *p, int current_time, int cpu_id) {
    printf("%d,RUNNING,pid=%s,remaining_time=%d,cpu=%d\n", current_time, p->pid, p->remaining_run_time, cpu_id);
}


void print_process_finished(process *p, int current_time, int processes_remaining) {
    printf("%d,FINISHED,pid=%s,proc_remaining=%d\n", current_time, p->pid, processes_remaining);
}


/*
Just for debugging.
*/
void print_process(process *p) {
    printf("(%d,%s)\n", p->remaining_run_time, p->pid);
}


/*
The number of sub-processes should be the lesser of K processors and x execution time
*/
int min(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}


/*
Does integer division and rounds up the result if needed.
*/
int divide_round_int(int a, int b) {
    int remainder = a % b;
    if (remainder == 0) {
        return a / b;
    } else {
        return a / b + 1;
    }
}


/*
Rounds the given double to 2 decimal places
*/
double round_double_to_2(double input) {
    return roundl(input * 100.0) / 100.0;
}


/*
Takes a string representation of numbers as input.
Like this:
- "5"
- "23.546"
Returns an array of two numbers.
If the second number is -1, then the string was just an int.
Needs to be freed.
*/
int *_string_to_int_array(char *input) {
	char *input_copy = malloc(sizeof(char) * 30);
	assert(input_copy);
	strcpy(input_copy, input);

	int *values = malloc(sizeof(int) * 2);
	assert(values);

	char *next_part = strtok(input_copy, ".");
	values[0] = atoi(next_part);

	next_part = strtok(NULL, ".");
	if (next_part != NULL) {
		values[1] = atoi(next_part);
	} else {
		values[1] = -1;
	}

	free(input_copy);
	return values;
}

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "heap/min_heap.h"
#include "utils.h"

#define MAX_PROCESSORS 1024
#define INITIAL_PROCESSES_BUDGET 100


/*
Parses and does value checking.
For processor_flag, atoi will try it's best to deal with non-integer input.
*/
void parse_cli(int argc, char **argv, char **file_flag, int processor_flag) {
    int option;
    while ((option = getopt(argc, argv, "f:p:c")) != -1) {
        switch (option) {
            case 'f':
                *file_flag = optarg;
                break;
            case 'p':
                processor_flag = atoi(optarg);
                break;
            case 'c':
                // TODO: implement section 4 scheduler.
                printf("You haven't implemented that yet.\n");
                break;
            case '?':
                printf("Parsing didn't work\n");
                exit(1);
        }
    }
    // printf("file_flag=%s, processor_flag=%d\n", *file_flag, processor_flag);
    
    // Value checking.
    if (*file_flag == NULL || processor_flag == 0 || processor_flag > MAX_PROCESSORS) {
        printf("invalid file_flag or processor_flag given\n");
        exit(1);
    }
}


/*
Reads every line in the given file into an array of processes.
Does absolutely no format checking.
The returned array should be in the same order as the file was.
*/
process **read_input_file(char *filename) {

    FILE *input_process_file = fopen(filename, "r");
    assert(input_process_file);

    process **all_processes = malloc(sizeof(process*) * INITIAL_PROCESSES_BUDGET);
    assert(all_processes);

    int index = 0;
    int max_length = INITIAL_PROCESSES_BUDGET;

    int arrival_time;
    int pid;
    int run_time;
    char parallelisable_char;
    while (fscanf(input_process_file, "%d %d %d %c", &arrival_time, &pid, &run_time, &parallelisable_char) == 4) {
        // printf("%d %d %d %c\n", arrival_time, pid, run_time, parallelisable_char);
        process *new_process = process_from_row(arrival_time, pid, run_time, parallelisable_char);
        
        // Allocate more memory if needed.
        if (index > max_length) {
            max_length *= 2;
            all_processes = realloc(all_processes, sizeof(process*) * max_length);
            assert(all_processes);
        }

        all_processes[index] = new_process;
        index++;
    }
    fclose(input_process_file);

    // Set sentinel to NULL for when you iterate through the array later.
    all_processes[index] = NULL;
    return all_processes;
}


void run_processor(min_heap **processor, int time) {
    
    peek(*processor)->remaining_run_time--;
    if (peek(*processor)->remaining_run_time == 0) {
        process *finished_process = pop(processor);
        print_process_finished(finished_process, time, (*processor)->last_index);
        free(finished_process);
    }
}


int main(int argc, char **argv) {

    char *input_file_name = NULL;
    int number_of_processors = 0;

    // Parse and check command-line arguments.
    parse_cli(argc, argv, &input_file_name, number_of_processors);

    // Read input file into an array of processes. (Watch out for weird pointer problems caused by this array)
    process **all_processes = read_input_file(input_file_name);

    // Actual Algorithm
    min_heap *processor = initialise_heap(INITIAL_PROCESSES_BUDGET);
    process *next_process;
    int time = 0;
    int process_index = 0;
    while (true) {
        next_process = all_processes[process_index];

        if (next_process != NULL && next_process->arrival_time == time) {
            // New process has arrived, just add it to processor
            push(&processor, next_process);
            process_index++;

        } else if (next_process == NULL) {
            // No more processes left to add. Run the rest that are already on the processor.
            while (is_empty(processor) != true) {
                time++;
                run_processor(&processor, time);
            }
            break;

        }

        // Actually use CPU time
        time++;
        if (is_empty(processor) != true) {
            run_processor(&processor, time);
        }
        
    }
    free_min_heap(processor);
    return 0;
}


/*
0 4 30 n
3 2 40 n
5 1 20 n
20 3 30 n

0,RUNNING,pid=4,remaining_time=30,cpu=0
5,RUNNING,pid=1,remaining_time=20,cpu=0
25,FINISHED,pid=1,proc_remaining=3
25,RUNNING,pid=4,remaining_time=25,cpu=0
50,FINISHED,pid=4,proc_remaining=2
50,RUNNING,pid=3,remaining_time=30,cpu=0
80,FINISHED,pid=3,proc_remaining=1
80,RUNNING,pid=2,remaining_time=40,cpu=0
120,FINISHED,pid=2,proc_remaining=0
Turnaround time 62
Time overhead 2.93 1.9
Makespan 120
*/

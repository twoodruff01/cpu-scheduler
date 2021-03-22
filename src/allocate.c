#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "heap/min_heap.h"
#include "utils.h"

#define MAX_PROCESSORS 1024


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


int main(int argc, char **argv) {

    // Parse and check command-line arguments.
    char *input_file_name = NULL;
    int number_of_processes = 0;
    parse_cli(argc, argv, &input_file_name, number_of_processes);

    FILE *input_process_file = fopen(input_file_name, "r");
    assert(input_process_file);

    int time = 0;
    min_heap *processor = initialise_heap(100);

    // Run scheduling algorithm on input file, whilst reading it.
    int arrival_time;
    int pid;
    int run_time;
    char parallelisable_char;
    while (fscanf(input_process_file, "%d %d %d %c", &arrival_time, &pid, &run_time, &parallelisable_char) == 4) {
        // printf("%d %d %d %c\n", arrival_time, pid, run_time, parallelisable_char);
        process *new_process = process_from_row(arrival_time, pid, run_time, parallelisable_char);

        // Just do Task 1 to get your head around it.
        int next_arrival_time = new_process->arrival_time;
        while (time < next_arrival_time && is_empty(processor) != true) {
            process *running_process = pop(&processor);
            printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n", time, running_process->pid, running_process->remaining_run_time, 0);
            running_process->remaining_run_time--;
            if (running_process->remaining_run_time <= 0) {
                printf("%d,FINISHED,pid=%d,proc_remaining=%d\n", time, running_process->pid, processor->last_index);
                printf("Process finished: ");
                print_process(running_process);
                free(running_process);
            } else {
                push(&processor, running_process);
            }
            time++;
        }
        time = next_arrival_time;  // Fast forward in case heap was empty a while ago.
        push(&processor, new_process);
    }

    while (is_empty(processor) != true) {
        process *running_process = pop(&processor);
        running_process->remaining_run_time--;
        if (running_process->remaining_run_time <= 0) {
            printf("Process finished: ");
            print_process(running_process);
            free(running_process);
        } else {
            push(&processor, running_process);
        }
        time++;
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

    fclose(input_process_file);
    return 0;
}

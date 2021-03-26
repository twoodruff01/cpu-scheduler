#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "heap/multicore.h"
#include "heap/cpu.h"
#include "utils.h"


/*
Parses and does value checking.
For processor_flag, atoi will try it's best to deal with non-integer input.
*/
void parse_cli(int argc, char **argv, char **file_flag, int *processor_flag) {
    int option;
    while ((option = getopt(argc, argv, "f:p:c")) != -1) {
        switch (option) {
            case 'f':
                *file_flag = optarg;
                break;
            case 'p':
                *processor_flag = atoi(optarg);
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
    if (*file_flag == NULL || *processor_flag == 0 || *processor_flag > MAX_PROCESSORS) {
        printf("invalid file_flag or processor_flag given\n");
        exit(1);
    }
}


/*
Reads every line in the given file into an array of processes.
Does absolutely no format checking.
The returned array should be in the same order as the file was.
*/
process **read_input_file(char *filename, int *number_of_processes) {

    FILE *input_process_file = fopen(filename, "r");
    assert(input_process_file);

    process **all_processes = malloc(sizeof(process*) * INITIAL_PROCESSES_BUDGET);
    assert(all_processes);

    int index = 0;
    int max_length = INITIAL_PROCESSES_BUDGET;

    int arrival_time;
    // if pid in file is longer than PID_MAX_LENGTH, the while loop below won't even run once.
    char pid[PID_MAX_LENGTH];
    int run_time;
    char parallelisable_char;
    while (fscanf(input_process_file, "%d %s %d %c", &arrival_time, pid, &run_time, &parallelisable_char) == 4) {
        // printf("%d %s %d %c\n", arrival_time, pid, run_time, parallelisable_char);
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

    *number_of_processes = index;

    // Set sentinel to NULL for when you iterate through the array later.
    all_processes[index] = NULL;
    return all_processes;
}

/*
Go through each cpu and give each first process one second of cpu time.
- Check for and print finished processes first.
- Check for and print running processes second.
*/
void run_one_second(multicore **cores, int *remaining_processes, int *time, int *total_turnaround, double *max_overhead, double *average_overhead) {

    // Sort cpu's on just cpu id.
    // Have to do this to print stuff out as the spec requested.
    multicore_heapify(cores, true);
    multicore_sort(cores, true);    

    cpu *current_cpu = NULL;
    process *first_process = NULL;

    // Print finished processes first
    for (int j = (*cores)->last_index; j >= 1; j--) {
        current_cpu = ((*cores)->cpu_array)[j];

        if (cpu_is_empty(current_cpu) == true) {
            continue;
        } else {
            first_process = cpu_peek(current_cpu);
        }
        
        // Check if process has finished.
        if (first_process->remaining_run_time == 0) {
            // TODO: edit this to handle sub-processes.

            // Calculate statistics
            int turnaround_time = *time - first_process->arrival_time; 
            *total_turnaround += turnaround_time;
            double time_overhead = ((double) turnaround_time) / ((double) first_process->run_time);
            *average_overhead += time_overhead;
            if (time_overhead > *max_overhead) {
                *max_overhead = time_overhead;
            }

            (*remaining_processes)--;
            print_process_finished(first_process, *time, *remaining_processes);
            free(cpu_pop(&current_cpu));
        }
    }

    // Print running processes second
    for (int j = (*cores)->last_index; j >= 1; j--) {
        current_cpu = ((*cores)->cpu_array)[j];
        first_process = cpu_peek(current_cpu);

        if (cpu_is_empty(current_cpu) == true) {
            continue;
        } else {
            first_process = cpu_peek(current_cpu);
        }

        // Check if process is already running.
        // cpu_push will update this variable elsewhere if the process moves.
        if (first_process->is_running == false) {
            first_process->is_running = true;
            print_process_running(first_process, *time, current_cpu->cpu_id);
        }

        // ACTUALLY DECREMENT TIME HERE
        first_process->remaining_run_time--;
        current_cpu->total_remaining_run_time--;
    }
    
    multicore_heapify(cores, false);
    (*time)++;
}


int main(int argc, char **argv) {

    char *input_file_name = NULL;
    int number_of_processors = 0;
    parse_cli(argc, argv, &input_file_name, &number_of_processors);

    // Statistics
    int total_turnaround = 0;
    int number_of_processes = 0;
    double max_overhead = 0;
    double total_overhead = 0;

    /*
    -----------------------Actual Algorithm-----------------------
    */

    int time = 0;
    int remaining_processes = 0;
    process *current_process = NULL;
    process *next_process = NULL;
    // process **parallelized;
    process **all_processes = read_input_file(input_file_name, &number_of_processes);
    multicore *cores = initialise_cores(number_of_processors);

    int i = 0;
    while (true) {

        current_process = all_processes[i];
        next_process = all_processes[i + 1];

        if (current_process->is_parallelisable == true) {
            /*
            TODO:
            - Split process into sub-processes and add them to cpu's.
            - Also keep track of them somehow.
            - int sub_processes = max(number_of_processors, current_process->run_time);
            */
            // remaining_processes++;

        } else {
            // Just add process to one cpu.
            multicore_add_process(&cores, current_process);
            // printf("------ADDING:");
            // print_process(current_process);
            remaining_processes++;
        }
        // print_multicore(cores);

        // Run all of the processes up to the next appropriate time.
        if (next_process == NULL) {
            // No more processes will be added, so just run all the remaining ones to completion.
            while (multicore_has_process(cores) == true) {
                run_one_second(&cores, &remaining_processes, &time, &total_turnaround, &max_overhead, &total_overhead);
            }
            break;

        } else while (time < next_process->arrival_time) {
            // Ensure processes with same arrival time all get added.
            // Run all the processes up till the next arrival time.
            run_one_second(&cores, &remaining_processes, &time, &total_turnaround, &max_overhead, &total_overhead);
        }


        // if (next_process == NULL) {
        //     printf("Before sorting:\n");
        //     print_multicore(cores);

        //     multicore_sort(&cores, false);
        //     printf("After sorting:\n");
        //     print_multicore(cores);

        //     printf("Heapify on pid \n");
        //     multicore_heapify(&cores, true);
        //     print_multicore(cores);

        //     printf("Sort on pid \n");
        //     multicore_sort(&cores, true);
        //     print_multicore(cores);
        //     break;
        // }
        i++;
    }
    
    printf("Turnaround time %d\n", divide_round_int(total_turnaround, number_of_processes));
    printf("Time overhead %g %g\n", round_double_to_2(max_overhead), round_double_to_2(total_overhead / number_of_processes));
    printf("Makespan %d\n", time - 1);
    free_cores(cores);
    free(all_processes);
    return 0;
}

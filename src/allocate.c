#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "linked_list/linked_list.h"
#include "heap/multicore.h"
#include "heap/cpu.h"
#include "utils.h"


/*
Parses command line input and does value checking.
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
The returned array should be in the same order as the file was.
Does not check input file formatting; malformed input will crash this program.
*/
process **read_input_file(char *filename, int *number_of_processes) {

    FILE *input_process_file = fopen(filename, "r");
    assert(input_process_file);

    process **all_processes = malloc(sizeof(process*) * INITIAL_PROCESSES_BUDGET);
    assert(all_processes);

    // if pid in file is longer than PID_MAX_LENGTH, the while loop below won't even run once.
    int arrival_time;
    char pid[PID_MAX_LENGTH];
    int run_time;
    char parallelisable_char;

    int index = 0;
    int max_length = INITIAL_PROCESSES_BUDGET;
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
Update statistics to be printed out at the end.
*/
void update_statistics(int time, int *total_turnaround, double *average_overhead, double *max_overhead, int arrival_time, int run_time) {
        
    int turnaround_time = time - arrival_time;

    *total_turnaround += turnaround_time;

    double time_overhead = ((double) turnaround_time) / ((double) run_time);

    if (time_overhead > *max_overhead) {
        *max_overhead = time_overhead;
    }

    *average_overhead += time_overhead;
}


/*
Go through each cpu and give each first process one second of cpu time.
- Check for and print finished processes first.
- Check for and print running processes second.
- Works by kind of using message passing between the for loops.
This function would be a lot nicer if I didn't have to print out everything 
in such a ridiculously specific order.
*/
void run_one_second(multicore **cores, linked_list **parallelized_processes, int *remaining_processes, int *time, int *total_turnaround, double *max_overhead, double *average_overhead) {

    // Sort cpu's on just cpu id.
    multicore_heapify(cores, true);
    multicore_sort(cores, true);    

    cpu *current_cpu = NULL;
    process *first_process = NULL;

    // Check status of processes after last second
    for (int j = (*cores)->last_index; j >= 1; j--) {
        current_cpu = ((*cores)->cpu_array)[j];
        first_process = cpu_peek(current_cpu);
        if (cpu_is_empty(current_cpu) == true) {
            continue;
        }
        
        if (first_process->remaining_run_time == 0 && first_process->is_sub_process) {
            // Sub-process has finished.
            
            // TODO: edit this to handle sub-processes.
            process *parent_process = find_parent(*parallelized_processes, first_process);
            if (parent_process->sub_processes_remaining == 0 && parent_process->parent_finished_printed == false) {
                // The last sub-process just finished: so the parent process is done too!
                update_statistics(*time, total_turnaround, average_overhead, max_overhead, parent_process->arrival_time, parent_process->run_time);
                print_process_finished(parent_process, *time, *remaining_processes);
                parent_process->parent_finished_printed = true;
                // TODO: Remove parent from linked_list (not essential)
                // free(parent_process);  // TODO: only free this if you plan to remove parent from list
            }
            // Take sub-process off cpu:
            free(cpu_pop(&current_cpu));

        } else if (first_process->remaining_run_time == 0) {
            // Actual process has finished
            update_statistics(*time, total_turnaround, average_overhead, max_overhead, first_process->arrival_time, first_process->run_time);
            print_process_finished(first_process, *time, *remaining_processes);
            // Take processes off cpu:
            free(cpu_pop(&current_cpu));
        }
    }

    // ACTUALLY DECREMENT TIME HERE.
    // These value will all be checked in the next call of this function.
    for (int j = (*cores)->last_index; j >= 1; j--) {
        current_cpu = ((*cores)->cpu_array)[j];
        first_process = cpu_peek(current_cpu);
        if (cpu_is_empty(current_cpu) == true) {
            continue;
        }

        // Check if process is already running.
        // cpu_push will update this variable elsewhere if the process moves.
        if (first_process->is_running == false) {
            first_process->is_running = true;
            print_process_running(first_process, *time, current_cpu->cpu_id);
        }

        // Decrement time variables.
        // print_process(first_process);
        first_process->remaining_run_time--;
        current_cpu->total_remaining_run_time--;

        // Decrement remaining_processes
        if (first_process->remaining_run_time == 0 && first_process->is_sub_process) {
            // TODO: edit this to handle sub-processes.
            // check if this is the last sub-process
            process *parent_process = find_parent(*parallelized_processes, first_process);
            parent_process->sub_processes_remaining--;
            if (parent_process->sub_processes_remaining == 0) {
                (*remaining_processes)--;
            }

        } else if (first_process->remaining_run_time == 0) {
            (*remaining_processes)--;   
        }
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
    This program uses three types of structs:
    - process
    - cpu
    - multicore

    cpu and multicore both wrap around min heaps, and are sorted differently.
    - cpu
        contains processes
        sorts those processes on:
            1. remaining run-time
            2. process id
    -multicore
        contains cpu's
        sorts those cpu's on:
            1. total remaining run-time
            2. cpu id

    At the start of every second, all of the processes that 'arrive' in that second are 
    added to a temporary cpu (which no processes will run on). Then, all of the processes on 
    that cpu are popped off in order of least run-time, and added to the actual multicore.

    Every second, the multicore goes through every cpu (in order of cpu id) and runs
    each cpu for one second, until there are no more processes left to add, at which point
    it just runs all the existing processes on all it's cpu(s) to completion.
    */

    int time = 0;
    int remaining_processes = 0;  // This actually means the number of processes that have arrived but not been started
    process *current_process = NULL;
    process *next_process = NULL;
    linked_list *parallelized_processes = create_list();
    process **all_processes = read_input_file(input_file_name, &number_of_processes);
    multicore *cores = initialise_cores(number_of_processors);
    cpu *temp_process_buffer = initialise_cpu(INITIAL_PROCESSES_BUDGET, -1);

    int i = 0;
    while (true) {

        // By default, add each new process to the process buffer.
        current_process = all_processes[i];
        next_process = all_processes[i + 1];
        if (current_process != NULL) {
            cpu_push(&temp_process_buffer, current_process);
            i++;
        }

        // If more than one process arrives at the same time, just add them all to the buffer.
        while (next_process != NULL && next_process->arrival_time == current_process->arrival_time) {
            current_process = all_processes[i];
            next_process = all_processes[i + 1];
            cpu_push(&temp_process_buffer, current_process);
            i++;
        }
        
        /*
        TODO: 
        - Add parallelised processes to the temp_process_buffer instead of adding them straight into the multicore.
        - So if you get two parallelisable processes at the same time, stuff still works.
        - Also change the loop so it adds all the para processes to cpu before adding any to multicore.
        */

        // Now add all of the processes that have arrived in the last second to cpu(s).
        while (cpu_is_empty(temp_process_buffer) != true) {

            // TODO: watch out for this creating memory problems.
            process *process_to_add = cpu_pop(&temp_process_buffer);
            // TODO: remove
            assert(process_to_add);

            if (process_to_add->is_parallelisable == true) {            

                // Split into sub-processes and add each of them to the cpu
                int sub_process_quantity = min(number_of_processors, process_to_add->run_time);
                int execution_time = divide_round_int(process_to_add->run_time, sub_process_quantity) + 1;  // TODO: integer division ok?
                for (int j = 0; j < sub_process_quantity; j++) {
                    process *new_sub_process = create_sub_process(process_to_add, execution_time, j);
                    // printf("New sub-process: ");
                    // print_process(new_sub_process);
                    multicore_add_process(&cores, new_sub_process);
                }

                // TODO: keep track of sub-processes somehow.
                process_to_add->sub_processes_remaining = sub_process_quantity;
                insert_process(&parallelized_processes, process_to_add);
                remaining_processes++;

            } else {
                // Just add process to one cpu.
                multicore_add_process(&cores, process_to_add);
                // printf("------ADDING:");
                // print_process(current_process);
                // print_multicore(cores);
                remaining_processes++;   // Careful with this if you decide to also add sub-processes here
            }
        }

        // Run all of the processes up to the next appropriate time.
        if (next_process == NULL) {
            // No more processes will be added, so just run all the remaining ones to completion.
            // print_multicore(cores);
            // return 0;
            while (multicore_has_process(cores) == true) {
                run_one_second(&cores, &parallelized_processes, &remaining_processes, &time, &total_turnaround, &max_overhead, &total_overhead);
            }
            break;

        } else while (time < next_process->arrival_time) {
            // Run all the processes up till the next arrival time.
            run_one_second(&cores, &parallelized_processes, &remaining_processes, &time, &total_turnaround, &max_overhead, &total_overhead);
        }
    }

    // print_list(parallelized_processes);
    
    printf("Turnaround time %d\n", divide_round_int(total_turnaround, number_of_processes));
    printf("Time overhead %g %g\n", round_double_to_2(max_overhead), round_double_to_2(total_overhead / number_of_processes));
    printf("Makespan %d\n", time - 1);
    free_cpu(temp_process_buffer);
    free_cores(cores);
    free(all_processes);
    return 0;
}

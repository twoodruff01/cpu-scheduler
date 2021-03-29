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
(Value checking disabled for submission).
*/
void parse_cli(int argc, char **argv, char **file_flag, int *processor_flag, bool *run_custom_algorithm) {
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
                *run_custom_algorithm = true;
                break;
            case '?':
                // printf("Parsing didn't work\n");
                // exit(1);
                break;
        }
    }

    // Value checking.
    if (*file_flag == NULL || *processor_flag == 0 || *processor_flag > MAX_PROCESSORS) {
        // printf("Invalid file_flag or processor_flag given\n");
        // exit(1);
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

    // Set sentinel to NULL for when you iterate through the array later.
    all_processes[index] = NULL;
    *number_of_processes = index;
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
*/
void run_one_second(multicore **cores, linked_list **parallelized_processes, int *remaining_processes, int *time, int *total_turnaround, double *max_overhead, double *average_overhead) {

    // Sort cpu's on just cpu id.
    multicore_heapify(cores, true);
    multicore_sort(cores, true);    

    cpu *current_cpu = NULL;
    process *first_process = NULL;

    // Check if any processes have finished in the last second.
    for (int j = (*cores)->last_index; j >= 1; j--) {
        current_cpu = ((*cores)->cpu_array)[j];
        first_process = cpu_peek(current_cpu);
        if (cpu_is_empty(current_cpu) == true) {
            continue;
        }
        
        if (first_process->remaining_run_time == 0 && first_process->is_sub_process) {
            // Sub-process has finished.
            process *parent_process = find_parent(*parallelized_processes, first_process);
            if (parent_process != NULL && parent_process->sub_processes_remaining == 0 && parent_process->parent_finished_printed == false) {
                // The last sub-process just finished: so the parent process is done too!
                update_statistics(*time, total_turnaround, average_overhead, max_overhead, parent_process->arrival_time, parent_process->run_time);
                print_process_finished(parent_process, *time, *remaining_processes);
                parent_process->parent_finished_printed = true;
                linked_list_remove_node(parallelized_processes, parent_process);
            }
            // Take sub-process off cpu:
            free(cpu_pop(&current_cpu));

        } else if (first_process->remaining_run_time == 0) {
            // Actual process has finished: printf and remove from cpu.
            update_statistics(*time, total_turnaround, average_overhead, max_overhead, first_process->arrival_time, first_process->run_time);
            print_process_finished(first_process, *time, *remaining_processes);
            free(cpu_pop(&current_cpu));
        }
    }

    // ACTUALLY DECREMENT TIME FOR ALL PROCESSES HERE.
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

        first_process->remaining_run_time--;
        current_cpu->total_remaining_run_time--;

        // Decrement remaining_processes if needed.
        if (first_process->remaining_run_time == 0 && first_process->is_sub_process) {
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


/*
Adds non-parallelizeable processes straight to the cpu
Splits up parallelizeable processes and adds all the sub-processes to the cpu,
the parent process get's added to a linked list to keep track of it.
- Only for use with the temp_process_buffer.
*/
void add_and_split_processes(cpu **current_cpu, linked_list **parallelized_processes, process *new_process, int *remaining_processes, int num_processors, bool run_custom_algorithm) {

    if (new_process->is_parallelisable == true) {            
        // Split into sub-processes and add each of them to the cpu
        int sub_process_quantity = min(num_processors, new_process->run_time);
        if (run_custom_algorithm == true) {
            sub_process_quantity--;
        }
        int execution_time = divide_round_int(new_process->run_time, sub_process_quantity) + 1;
        new_process->sub_processes_remaining = sub_process_quantity;
        insert_process(parallelized_processes, new_process);
        for (int j = 0; j < sub_process_quantity; j++) {
            process *new_sub_process = create_sub_process(new_process, execution_time, j);
            cpu_push(current_cpu, new_sub_process);
        }
    } else {
        // Just add one process to cpu.
        cpu_push(current_cpu, new_process);
    }
    (*remaining_processes)++;
}


int main(int argc, char **argv) {

    char *input_file_name = NULL;
    int number_of_processors = 0;
    bool run_custom_algorithm = false;
    parse_cli(argc, argv, &input_file_name, &number_of_processors, &run_custom_algorithm);

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
    linked_list *parallelized_processes = create_list();
    process **all_processes = read_input_file(input_file_name, &number_of_processes);
    multicore *cores = initialise_cores(number_of_processors);
    cpu *temp_process_buffer = initialise_cpu(INITIAL_PROCESSES_BUDGET, -1);

    int i = 0;
    while (true) {

        // By default, add each new process to the process buffer, whilst also splitting into sub-processes.
        current_process = all_processes[i];
        next_process = all_processes[i + 1];
        if (current_process != NULL) {

            if (run_custom_algorithm == true && current_process->run_time <= 2) {
                cpu_push(&temp_process_buffer, current_process);
                remaining_processes++;
            } else {
                add_and_split_processes(&temp_process_buffer, &parallelized_processes, current_process, &remaining_processes, number_of_processors, run_custom_algorithm);
            }
            i++;
        }

        // If more than one process arrives at the same time, just add them all to the buffer.
        while (next_process != NULL && next_process->arrival_time == current_process->arrival_time) {
            current_process = all_processes[i];
            next_process = all_processes[i + 1];
            if (run_custom_algorithm == true && current_process->run_time <= 2) {
                cpu_push(&temp_process_buffer, current_process);
                remaining_processes++;
            } else {
                add_and_split_processes(&temp_process_buffer, &parallelized_processes, current_process, &remaining_processes, number_of_processors, run_custom_algorithm);
            }
            i++;
        }

        // Now add all of the processes that have arrived in the last second to cpu(s).
        // Sub processes could actually end up on the same cpu: I don't care at this point.
        while (cpu_is_empty(temp_process_buffer) != true) {
            process *process_to_add = cpu_pop(&temp_process_buffer);
            multicore_add_process(&cores, process_to_add);
        }

        // Run all of the processes up to the next appropriate time.
        if (next_process == NULL) {
            // No more processes will be added, so just run all the remaining ones to completion.
            while (multicore_has_process(cores) == true) {
                run_one_second(&cores, &parallelized_processes, &remaining_processes, &time, &total_turnaround, &max_overhead, &total_overhead);
            }
            break;

        } else while (time < next_process->arrival_time) {
            // Run all the processes up till the next arrival time.
            run_one_second(&cores, &parallelized_processes, &remaining_processes, &time, &total_turnaround, &max_overhead, &total_overhead);
        }
    }

    printf("Turnaround time %d\n", divide_round_int(total_turnaround, number_of_processes));
    printf("Time overhead %g %g\n", round_double_to_2(max_overhead), round_double_to_2(total_overhead / number_of_processes));
    printf("Makespan %d\n", time - 1);
    free(parallelized_processes);
    free_cpu(temp_process_buffer);
    free_cores(cores);
    free(all_processes);
    return 0;
}

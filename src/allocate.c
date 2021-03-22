#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
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

    // Run scheduling algorithm on input file, whilst reading it.
    int arrival_time;
    int pid;
    int run_time;
    char parallelisable_char;
    while (fscanf(input_process_file, "%d %d %d %c", &arrival_time, &pid, &run_time, &parallelisable_char) == 4) {
        printf("%d %d %d %c\n", arrival_time, pid, run_time, parallelisable_char);

        process *new_process = process_from_row(arrival_time, pid, run_time, parallelisable_char);
        print_process(new_process);


    }

    fclose(input_process_file);
    return 0;
}

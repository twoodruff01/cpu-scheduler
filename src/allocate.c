#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void parse_cli(int argc, char **argv, char *file_flag, int processor_flag);

int main(int argc, char **argv) {

    char *file_flag = NULL;
    int processor_flag = 0;
    parse_cli(argc, argv, file_flag, processor_flag);

    return 0;
}


/*
Doesn't do any value checking
For processor_flag, atoi will try it's best to deal with non-integer input,
but this isn't really designed to deal with that.
*/
void parse_cli(int argc, char **argv, char *file_flag, int processor_flag) {
    int option;
    while ((option = getopt(argc, argv, "f:p:c")) != -1) {
        switch (option) {
            case 'f':
                file_flag = optarg;
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
    printf("file_flag=%s, processor_flag=%d\n", file_flag, processor_flag);
}

#ifndef MULTICORE
#define MULTICORE

#include <stdbool.h>
#include "../utils.h"
#include "cpu.h"

typedef struct {
    cpu **cpu_array;
    int array_size;
    int last_index;
} multicore;


multicore *initialise_cores(int cpu_quantity);
void multicore_push(multicore **cores, cpu *new_cpu);
void multicore_add_process(multicore **cores, process *new_process);
bool multicore_is_empty(multicore *cores);
void free_cores(multicore *cores);
void multicore_sort(multicore **cores);
void multicore_heapify(multicore **cores);

void print_multicore(multicore *cores);

void _multicore_downheap(multicore **cores, int start_index);

#endif

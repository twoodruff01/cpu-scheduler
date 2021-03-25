#ifndef MIN_HEAP
#define MIN_HEAP

#include <stdbool.h>
#include "../utils.h"

typedef struct {
    process **process_array;
    int array_size;
    int last_index;

    int total_remaining_run_time;  // priority 1
    int cpu_id;  // priority 2
} cpu;


cpu *initialise_cpu(int initial_size, int id);
void cpu_push(cpu **current_cpu, process *new_process);
process *cpu_pop(cpu **current_cpu);
process *cpu_peek(cpu *current_cpu);
bool cpu_is_empty(cpu *current_cpu);
void free_cpu(cpu *current_cpu);
void cpu_sort(cpu **current_cpu);
void cpu_heapify(cpu **current_cpu);

bool cpu_less_than(cpu *c1, cpu *c2);
void swap_cpu_pointers(cpu **c1, cpu **c2);
void print_cpu(cpu *current_cpu);

void _cpu_downheap(cpu **current_cpu, int start_index);

#endif

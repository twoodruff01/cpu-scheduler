#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "max_heap.h"

#define ARRAY_SIZE 5

int main() {

    // this needs to be length+1 or you'll go past the end of the array without realising
    int *numbers = malloc(sizeof(int) * (ARRAY_SIZE + 1));
    assert(numbers);

    fill_heap(numbers, ARRAY_SIZE);
    print_heap_horizontally(numbers, ARRAY_SIZE);

    construct_heap(numbers, ARRAY_SIZE);
    print_heap_horizontally(numbers, ARRAY_SIZE);
    
    heap_sort(numbers, ARRAY_SIZE);
    print_heap_horizontally(numbers, ARRAY_SIZE);

    free(numbers);
    return 0;
}

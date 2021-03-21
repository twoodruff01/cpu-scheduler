#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "min_heap.h"

#define ARRAY_SIZE 20

int main() {

    // this needs to be length+1 or you'll go past the end of the array without realising
    int *numbers = malloc(sizeof(int) * (ARRAY_SIZE + 1));
    assert(numbers);

    fill_heap(numbers, ARRAY_SIZE);
    print_heap_horizontally(numbers, ARRAY_SIZE);

    heapify(numbers, ARRAY_SIZE);
    print_heap_horizontally(numbers, ARRAY_SIZE);

    printf("%d\n", delete_min(numbers, ARRAY_SIZE));
    print_heap_horizontally(numbers, ARRAY_SIZE);
    
    heap_sort(numbers, ARRAY_SIZE);
    print_heap_horizontally(numbers, ARRAY_SIZE);

    free(numbers);

    // min_heap *first_heap = initialise_heap(ARRAY_SIZE);

    return 0;
}

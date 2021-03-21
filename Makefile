CC=gcc

### Submission ###
# CFLAGS = -Wall -O3

### Testing ###
CFLAGS = -Wall -Wextra -W -Wshadow -O3 -g3

BUILD_SRC = src/utils.o src/heap/min_heap.o src/allocate.o
all: $(BUILD_SRC)
	$(CC) -o allocate $(BUILD_SRC) $(CFLAGS)


DEV_SRC = src/utils.o src/heap/min_heap.o src/heap/heap_main.o
heap: $(DEV_SRC)
	$(CC) -o heap_program $(DEV_SRC) $(CFLAGS)


clean:
	rm -f allocate heap_program *.o src/*.o src/heap/*.o *.txt *.out *.gch

CC=gcc

#TODO: Change flags before submission

### Submission ###
# CFLAGS = -Wall -O3

### Testing ###
CFLAGS = -Wall -Wextra -W -Wshadow -g3 -lm

BUILD_SRC = src/utils.o src/heap/cpu.o src/heap/multicore.o src/linked_list/linked_list.o src/allocate.o
-B: $(BUILD_SRC)
	$(CC) -o allocate $(BUILD_SRC) $(CFLAGS)


DEV_SRC = src/utils.o src/heap/cpu.o src/heap/multicore.o src/heap/cpu_test.o
heap: $(DEV_SRC)
	$(CC) -o heap_program $(DEV_SRC) $(CFLAGS)


clean:
	rm -f allocate heap_program *.o src/*.o src/heap/*.o src/linked_list/*.o *.txt *.out *.gch

# Implementation #

This program uses three types of structs:
1. process
2. cpu
3. multicore

---

cpu and multicore both wrap around min heaps, and are sorted differently.

### cpu: ###
- contains processes
- sorts those processes on:
    1. remaining run-time
    2. process id

### multicore: ###
- contains cpu's
- sorts those cpu's on:
    1. total remaining run-time of all sub-processes on a cpu
    2. cpu id

---

At the start of every second, all incoming processes are either added straight to the temporary cpu (which no processes will run on), or split into sub-processes which are then added to the temporary cpu too.
Parallelizeable processes themselves will be added to a linked list when they arrive, so they can keep track of when their sub-processes finish.

Next, all of the processes on the buffer are popped off in order of least run-time, and added to the actual multicore.

Every second, the multicore goes through every cpu (in order of cpu id) and runs
each cpu for one second, until there are no more processes left to add, at which point it just runs all the existing processes on all it's cpu(s) to completion.

---

Most of the complexity in this program arises from having to print out stuff in such a specific order...

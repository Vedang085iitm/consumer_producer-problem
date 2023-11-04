# Producer consumer problem

**"use command Ctrl+Shift+V (or Cmd+Shift+V on macOS)  for Preview of readme"**

The objective of this project is to implement solutions to the the multiple producers with multiple consumers synchronization problem. This is based on the producer-consumer solution discussed in class, using a buffer of finite capacity

The implementation will use multiple threads using the Linux pthreads API. The implementation will be done in C.
Command line interface

The program will be invoked with the following command-line arguments:
```bash
./solution -p <int> -c <int> -s <int> -n <int>
```
The arguments are used as follows:

 -p value denotes the number of producer threads (P)

-c value denotes the number of consumer threads (C)

-s value denotes the size of the buffer (S); buffer will store a set of S integer values.

-n denotes the maximum number of items produced by a producer thread (N).

Inputs

The parent process will create the required number of producer and consumer threads.

Each producer thread will produce a random number (ranging from 1000 to 9999) and add it to the shared buffer if the buffer is not full. After adding an item, the producer will print the following statement:

Producer Thread #: 5; Item #: 3; Added Number: 3456

Then, it will go to sleep for a random time between 0 and 3 seconds, before producing the next item; this repeats. Each producer thread will exit after producing N

items.

Each consumer thread will read the next available value from the buffer and then print the following statement:

Consumer Thread #: 2; Item #: 9; Read Number: 3456

Then, it will go to sleep for a random time between 0 and 4 seconds, before consuming the next item; this repeats. Each consumer thread will exit after consuming 0.7NP/C

items.

The parent thread waits for all children threads to finish; it then prints:

Program over...

and exits.
## Explanation of Solution

This C program simulates a multi-threaded producer-consumer scenario using the pthread library. Here's an explanation of what the program does:

1. **Initialization**: The program starts by initializing various variables and data structures. It defines a buffer for storing items, and it sets up a mutex (`mtx`) and two condition variables (`empty` and `fill`) for synchronization.

2. **Producer Threads**: The program creates multiple producer threads. Each producer thread generates random numbers (simulating items) and adds them to a shared buffer. It uses the mutex and condition variables to ensure that the buffer doesn't overflow. After adding an item, the producer thread sleeps for a random amount of time, simulating some work before producing the next item.

3. **Consumer Threads**: The program also creates multiple consumer threads. Consumer threads read items from the buffer, again using mutex and condition variables to ensure there are items to consume. After reading an item, a consumer thread sleeps for a random time, simulating some work before consuming the next item. Consumer threads keep consuming until they have consumed the required number of items or until all producer threads have finished.

4. **Command-Line Arguments**: The program accepts command-line arguments to specify the number of producer threads (`-p`), the number of consumer threads (`-c`), the buffer size (`-s`), and the maximum number of items produced by each producer thread (`-n`).

5. **Program Exit Handling**: To ensure the program doesn't get stuck, it checks for a situation where consumer threads are waiting, there are no items to consume, and there are no more producer threads. If this happens, the program will exit to prevent getting stuck. It does this by comparing the current time with a timeout threshold.

6. **Thread Management**: The program creates and manages producer and consumer threads, waiting for them to finish their work. Once all producer threads have finished, it signals to consumer threads that no more items will be added to the buffer.

7. **Program Completion**: After all threads have completed their work, the program prints a message indicating that it's over.

The modifications to the program include handling situations where the program may get stuck and ensuring it exits in such cases to prevent indefinite waiting. The program also prints messages to indicate why it exited.

## sample input
```bash
gcc solution.c -o solution -pthread  
./solution -p 1 -c 1 -s 2 -n 2
```
## Sample output

```basj
./solution -p 1 -c 1 -s 1 -n 3

#############Start#############
Producer Thread #1; Item #0; Added Number: 6383
Consumer Thread #1; Item #0; Read Number: 6383
Producer Thread #1; Item #1; Added Number: 2915
Consumer Thread #1; Item #1; Read Number: 2915
Producer Thread #1; Item #2; Added Number: 3386
Consumer Thread #1; Item #2; Read Number: 3386
Program over...
```

### A bash script has been made 
```bash
chmod +x compilation.sh
./compilation.sh
```

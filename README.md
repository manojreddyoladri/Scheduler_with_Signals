# Scheduler_with_Signals

-**Overview of the project**
In this project, we simulated a time-sharing system by using signals, timers, and a round robin scheduling algorithm. Instead of using iterations of a while() loop to model the concept of “time slices,” we used interval timers. The scheduler is installed with an interval timer. The timer starts ticking when the scheduler picks a thread to use the CPU which in turn signals the thread when its time slice is finished thus allowing the scheduler to pick another thread and so on. When a thread has completely finished its work, it leaves the scheduler to allow a waiting thread to enter. Only the timer and scheduler send signals. The threads passively handle the signals without signaling back to the scheduler.

-**Preliminary Discussion**
- The goal of this project is to understand how signals and timers work, as well as how scheduler performance can be assessed. We implemented the time sharing system using timers and signals, and then evaluate the overall performance of the program by keeping track of how long each thread is idle, running, etc.
The program will use these four signals:
  - `SIGALRM`: sent by the timer to the scheduler, to indicate another time quantum has passed.
  - `SIGUSR1`: sent by the scheduler to a worker, to tell it to suspend.
  - `SIGUSR2`: sent by the scheduler to a suspended worker, to tell it to resume.
  - `SIGTERM`: sent by the scheduler to a worker, to tell it to cancel.
- We need to set up the appropriate handlers and masks for these signals. We used these functions:
  - `clock_gettime`
  - `pthread_sigmask`
  - `pthread_kill`
  - `sigaction`
  - `sigaddset`
  - `sigemptyset`
  - `sigwait`
  - `timer_settime`
  - `timer_create`

- To understand more about how the POSIX interval timer works, you can read about it in the man pages
  [POSIX interval timer](http://man7.org/linux/man-pages/man2/timer_create.2.html)

- The round robin scheduling policy alternates between executing the two available threads until they run out of work to do.

- **Command lines to run the program**
  ```sh
  ./rr_fifoscheduler -rr 10 2 3
  ```
  The command line options used above specify:
  - `-rr` Use round robin scheduling policy
  - 10: ten threads can be in the scheduler queue at a time
  - 2: Create 2 worker threads
  - 3: each thread runs for 3 time slices

  ```sh
  ./rr_fifoscheduler -fifo 1 2 3
  ```
  The command line options used above specify:
  - fifo: Use FIFO scheduling policy
  - 1: one thread can be in the scheduler queue at a time
  - 2: create 2 worker threads
  - 3: each thread runs for 3 time slices
 
- **Procedure**
  - **Program Specification**
    - Arg1 determines the number of jobs created (threads implemented)
    - Arg2 specifies the queue size of the scheduler
    - Arg3-ArgN gives the duration (the required number of time slices) of each job. Hence if we create 2 jobs, we should supply an arg3 and arg4 for the required duration.
   
  - **Files**
    - **list.h** Defines the basic operations on a bidirectional linked list data structure. The elements of the list allow you to store pointers to whatever kind of data you like.
    - **list.c** Implements the linked list operations.
    - **smp5_tests.c, testrunner.c, testrunner.h** Test harness, defines test cases for checking your solution.
    - **scheduler.h** Describes the interface which the scheduler implements.
    - **scheduler.c** Implements the scheduling.
    - **worker.h** Provides the declaration for operations on each worker thread.
    - **worker.c** Provides the implementation of the operations on the worker threads.
    - **Makefile** Compiles the program into a binary, scheduler.
   
- **Testing and Validation**
  There are several ways you can test the program:
  - You can test the program with specific parameters by running `./scheduler -test gen ...` where the ellipsis contain the parameters you would pass to scheduler (e.g., `./scheduler -test gen 3 2 3 2 3`). If you use this you must be careful to have the right number of arguments. If you have the wrong number of arguments the test defaults to pass (false positive).
  - You can also run the individual built-in tests by running `./scheduler -test <testname>` where `<testname>` is one of:
    
    – `test_3_1_2_2_2`
    – `test_2_2_2_2`
    – `test_5_7_1_2_1_2_1`
    – `test_4_1_1_2_3_4`
    – `test_3_3_4_3_2`

  - Finally, you can run all the built-in tests by running `./scheduler -test -f0 rr`. This runs the 5 tests above.

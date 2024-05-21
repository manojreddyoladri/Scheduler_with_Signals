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

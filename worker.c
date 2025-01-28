#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "scheduler.h"



/* Handler for SIGTERM signal */
void cancel_thread()
{ 
	printf("Thread %u: terminating.\n", (unsigned int)pthread_self());

	/* signal that done in queue */
	sem_post(&queue_sem);

	pthread_exit(NULL);
} 

/* Handle the SIGUSR1 signal */
void suspend_thread()
{   
	printf("Thread %u: suspending.\n", (unsigned int)pthread_self());

	/*wait for a resume signal from the scheduler*/
	sigset_t signal_wait_set;
    sigemptyset (&signal_wait_set); // must call this or results are undefined
    sigaddset(&signal_wait_set, SIGUSR2); // let's wait for SIGUSR2
    int sig;
	pthread_sigmask(SIG_BLOCK, &signal_wait_set, NULL); // add set to
    // blocked signals
		
    // wait for signal		
    int i=1;
    while(1)
    {
        //printf("Waiting for a SIGUSR2...%d\n",i);
        // now wait for the signal
        sigwait(&signal_wait_set, &sig);
        //printf("Got the signal...waking up.\n\n");
        i++;
    }
	printf("Thread %u: resuming.\n",(unsigned int) pthread_self());
}

/*
 * waits to gain access to the scheduler queue.
 */
static int enter_scheduler_queue(thread_info_t *info)
{
	/*
	 * wait for available room in queue.
	 * create a new list entry for this thread
	 * store this thread info in the new entry.
	 */
	sem_wait(&queue_sem);
	list_elem *item = (list_elem*)malloc(sizeof(list_elem));
	info->le = item;
	item->info = info;
	item->prev = 0;
	item->next = 0;
	list_insert_tail(&sched_queue, item);
	return 0;
}

/*
 * leaves the scheduler queue
 */
void leave_scheduler_queue(thread_info_t *info)
{
	printf("Thread %lu: leaving scheduler queue.\n", info->thrid);
	/*
	 * remove the given worker from queue
	 * clean up the memory that we malloc'd for the list
	 * clean up the memory that was passed to us
	 */
	list_remove(&sched_queue, info->le);
	free(info->le);
	free(info);
}


/*
 * Initialize thread, enter scheduling queue, and execute instructions.
 * arg is a pointer to thread_info_t
 */
void *start_worker(void *arg)
{
	thread_info_t *info = (thread_info_t *) arg;
	float calc = 0.8;
	int j = 0;

	/* Block SIGALRM and SIGUSR2. */
	 sigset_t signal_set_block;
	 sigemptyset(&signal_set_block);
	 sigaddset(&signal_set_block, SIGALRM);
	 sigaddset(&signal_set_block, SIGUSR2);
	 pthread_sigmask(SIG_BLOCK, &signal_set_block, NULL);

	/* Unblock SIGUSR1 and SIGTERM. */
	sigset_t signal_set_unblock;
	sigemptyset(&signal_set_unblock);
    sigaddset(&signal_set_unblock, SIGUSR1);
    sigaddset(&signal_set_unblock, SIGTERM);
	pthread_sigmask(SIG_UNBLOCK, &signal_set_unblock, NULL);

	/* compete with other threads to enter queue. */
	if (enter_scheduler_queue(info)) {
		printf("Thread %lu: failure entering scheduler queue - %s\n", info->thrid, strerror(errno));
		free (info);
		pthread_exit(0);
	}
	printf("Thread %lu: in scheduler queue.\n", info->thrid);

	suspend_thread();

	while (1) {
		/* do some meaningless work... */
		for (j = 0; j < 10000000; j++) {
			calc = 4.0 * calc * (1.0 - calc);
		}
	}
}

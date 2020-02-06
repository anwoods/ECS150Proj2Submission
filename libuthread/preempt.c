#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"


/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction sigact;
struct itimerval timer;


static void signal_handler(){
	uthread_yield();
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
	//disable preemption = block signals
	sigprocmask(SIG_BLOCK, &sigact.sa_mask, NULL);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	//enable preemption = unblock signals
	sigprocmask(SIG_UNBLOCK, &sigact.sa_mask, NULL);
}

void preempt_start(void)
{
	/* TODO Phase 4 */
	//sigemptyset(&new_mask);
	//sigemptyset(&old_mask);
	sigaddset(&sigact.sa_mask, SIGVTALRM);

	//Configure a timer that must fire a virtual alarm at a frequency of 100 Hz and
 	//setup a timer handler that forcefully yields the currently running thread.
	//struct sigaction sigact;
	//memset? malloc?

	//sigact.sa_sigaction = ?
	//sigact.sa_flags = ?
	sigact.sa_handler = &signal_handler;

 	sigaction(SIGVTALRM, &sigact, NULL); //one action should be NULL

 	//struct itimerval timer;
 
 	timer.it_value.tv_sec = 0;
 	timer.it_value.tv_usec = HZ;

 	timer.it_interval.tv_sec = 0;
 	timer.it_interval.tv_usec = HZ;

 	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

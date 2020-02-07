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

/* struct to install signal handler */
struct sigaction sigact;

/* timer to fire alarm */
struct itimerval timer;

static void signal_handler(){
	//yield whenever SIGVTALRM is received
	uthread_yield();
}

void preempt_disable(void)
{
	//disable preemption = block signals
	sigprocmask(SIG_BLOCK, &sigact.sa_mask, NULL);
}

void preempt_enable(void)
{
	//enable preemption = unblock signals
	sigprocmask(SIG_UNBLOCK, &sigact.sa_mask, NULL);
}

void preempt_start(void)
{
	sigaddset(&sigact.sa_mask, SIGVTALRM);

	sigact.sa_handler = &signal_handler;

 	sigaction(SIGVTALRM, &sigact, NULL);
 
 	timer.it_value.tv_sec = 0;
 	timer.it_value.tv_usec = HZ;

 	timer.it_interval.tv_sec = 0;
 	timer.it_interval.tv_usec = HZ;

 	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

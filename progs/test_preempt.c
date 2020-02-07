/*
 * Thread preemption test
 *
 * Tests preemption by using an infinite loop.
 * Expected output: 
 * statement in thread1,
 * statement in thread3 repeated numerous times,
 * statement in thread2.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int thread3(void* arg)
{
	while(1){
		printf("thread3 = %d\n", uthread_self());
	}
	return 0;
}

int thread2(void* arg)
{
	uthread_create(thread3, NULL);
	uthread_yield();
	printf("thread2 = %d\n", uthread_self());
	return 0;
}

int thread1(void* arg)
{
	uthread_create(thread2, NULL);
	uthread_yield();
	printf("thread1 = %d\n", uthread_self());
	return 0;
}

int main(void)
{
	uthread_join(uthread_create(thread1, NULL), NULL);
	return 0;
}
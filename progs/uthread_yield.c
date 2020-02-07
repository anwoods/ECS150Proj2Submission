/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>
#include <uthread.h>

int thread3(void* arg)
{
	uthread_yield();
	uthread_join(0, NULL); //Thread 3 tries to join with main thread
	printf("thread3 = %d\n", uthread_self());
	return 0;
}

int thread2(void* arg)
{
	uthread_create(thread3, NULL);
	uthread_yield();
	uthread_join(uthread_self(), NULL); //Thread 2 tries to join itself
	uthread_join(1, NULL);//Thread 2 tries to join thread 1, but it has a parent
	printf("thread2 = %d\n", uthread_self());
	return 0;
}

int thread1(void* arg)
{
	uthread_create(thread2, NULL);
	uthread_yield();
	printf("thread1 = %d\n", uthread_self());
	uthread_yield();
	return 0;
}

int main(void)
{
	uthread_join(uthread_create(thread1, NULL), NULL);
	return 0;
}

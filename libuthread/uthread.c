#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

/* structure for Thread Control Blocks */
struct tcb {
	uthread_t tid;
	uthread_ctx_t ctx;
	void * stack_ptr;
	struct tcb * parent;
	int retval;
};

//keep track of number of threads to assign TIDs to new threads
int num_threads;

//queue for each thread state
queue_t ready_queue;
queue_t zombie_queue;
queue_t blocked_queue;

//pointer to TCB of currently running thread
struct tcb * cur_thread;

/* Function to pass to queue_iterate() to check if given TID matches with 
   TID of given TCB. */
static int uthread_match_tids(void * thread, void * tid_to_match){
	struct tcb * temp = (struct tcb *)thread;
	if(temp->tid == *(int*)tid_to_match){
		return 1;
	}
	return 0;
}

/* Function to initiate library of threads, called in uthread_create
   if library does not already exist. */
static void uthread_init_library(void){
	//when library is created, set up preemption
	preempt_start();

	//initialize queues
	preempt_disable();
	ready_queue = queue_create();
	preempt_enable();
	preempt_disable();
	zombie_queue = queue_create();
	preempt_enable();
	preempt_disable();
	blocked_queue = queue_create();
	preempt_enable();

	//initialize main thread TCB
	struct tcb * main_thread = malloc(1 * sizeof(struct tcb));
	main_thread->stack_ptr = uthread_ctx_alloc_stack();
	uthread_ctx_init(&(main_thread->ctx), main_thread->stack_ptr, NULL, NULL);
	num_threads = 0;
	main_thread->tid = num_threads;
	main_thread->parent = NULL;
	num_threads++;
	cur_thread = main_thread;
}

void uthread_yield(void)
{
	preempt_disable();	//disable preemption so yield can perform atomically
	if(queue_length(ready_queue) > 0){
		struct tcb * next_thread;
		struct tcb * prev_thread;
		struct tcb * temp_thread = NULL;	//create temp_thread to check if prev_thread is in zombie thread without changing value of prev_thread

		queue_dequeue(ready_queue, (void**)&next_thread);	//find next ready thread

		prev_thread = cur_thread;	//set prev_threat to currently running thread
		cur_thread = next_thread;	//next_thread is now the currently now running thread

		queue_iterate(zombie_queue, &uthread_match_tids, (void**)&prev_thread->tid, (void**)&temp_thread);
		if(temp_thread == NULL){
			//if prev thread not in zombie queue, check if in blocked queue
			queue_iterate(blocked_queue, &uthread_match_tids, (void**)&prev_thread->tid, (void**)&temp_thread);
			if(temp_thread == NULL){	//if prev thread not in blocked queue, add to ready queue
				queue_enqueue(ready_queue, prev_thread);
			}
		}
        uthread_ctx_switch(&(prev_thread->ctx), &(cur_thread->ctx));	//switch threads
	}
	preempt_enable();
}

uthread_t uthread_self(void)
{
	return cur_thread->tid;
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct tcb * new_thread = malloc(1 * sizeof(struct tcb));
	new_thread->stack_ptr = uthread_ctx_alloc_stack();		//allocate space for new thread

	if(cur_thread == NULL){
		uthread_init_library();		//if queues havent been initialized, call function to initialize them
	}

	uthread_ctx_init(&(new_thread->ctx), new_thread->stack_ptr, func, arg);		//initialize new thread
	new_thread->tid = num_threads;		//set TID
	new_thread->parent = NULL;		//initialize ptr to parent
	preempt_disable();
	queue_enqueue(ready_queue, new_thread);		//new thread now ready for execution
	preempt_enable();
	num_threads++;		//increase num_threads
	return new_thread->tid;
}

void uthread_exit(int retval)
{
	cur_thread->retval = retval;	//set running threads retval

	if(cur_thread->parent != NULL){		//if has a parent thread, move parent from blocked to ready queue
		preempt_disable();
		queue_delete(blocked_queue, cur_thread->parent);
		preempt_enable();
		preempt_disable();
		queue_enqueue(ready_queue, cur_thread->parent);
		preempt_enable();
	}
	preempt_disable();
	queue_enqueue(zombie_queue, cur_thread);	//add running thread to zombie queue
	preempt_enable();
	uthread_yield();	//yield to next ready thread
}

int uthread_join(uthread_t tid, int *retval)
{
	if(tid == 0 || tid == cur_thread->tid){		//cannot join main thread or itself
		return -1;
	}

	struct tcb * child_thread = NULL;	//create child_thread ptr
	preempt_disable();
	queue_iterate(ready_queue, &uthread_match_tids, (void**)&tid, (void**)&child_thread);	//find child_thread match in ready queue
	preempt_enable();
	if(child_thread != NULL){	//if child is in ready queue
		if(child_thread->parent != NULL){	//can't join a thread that has already been joined
			return -1;
		}

		child_thread->parent = cur_thread;		//set childs parent as calling thread
		preempt_disable();
		queue_enqueue(blocked_queue, cur_thread);		//add calling thread to blocked queue
		preempt_enable();
		uthread_yield();	//yield to next ready thread
	}

	preempt_disable();
	queue_iterate(zombie_queue, &uthread_match_tids, (void**)&tid, (void**)&child_thread);	//find child in zombie queue
	preempt_enable();
	if(child_thread != NULL){
		preempt_disable();
		queue_delete(zombie_queue, child_thread);	//delete child from zombie queue
		preempt_enable();

		if(retval != NULL){
			*retval = child_thread->retval;		//collect retval from child
		}

		free(child_thread);		//deallocate child
		num_threads--;

		//if all the queues are empty, free currently running thread and destroy all queues
		if(queue_length(blocked_queue) == 0 && queue_length(ready_queue) == 0 && queue_length(zombie_queue) == 0){
			free(cur_thread);
			queue_destroy(blocked_queue);
			queue_destroy(ready_queue);
			queue_destroy(zombie_queue);
		}
		return 0;
	}
	return -1;
}







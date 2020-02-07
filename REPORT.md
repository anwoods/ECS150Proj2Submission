
# User-Level Thread Library Implementation Report
-------------------------------------------------------------------------------
## High-Level Design Choices

### Queue Design Choices
We decided to implement the queue as a linked list, to account for the necessary  
dynamic quality of the queue, and so we could easily use pointers to the first  
and last nodes in the list to make the queue FIFO.  To do this, we created  
seperate structs for the queue and for the nodes (which is where the actual data  
item is stored).  We then had to add another function to create the nodes we  
would add to the queue. Having the queue structure contain a head and a tail  
pointer made it easier to access the beginning of the end of the queue in O(1).

### Uthread Design Choices
To implement the multithreading, we created a data structure to represent the  
Thread Control Block for the main thread and each thread added by the user.  
This structure includes most of the information recommemded by the project  
description.  However, instead of including a member variable for the thread  
state, we chose to create global queues representing each potential state  
(except for the running state, which we distinguished with a pointer, since only  
one TCB can be running at a time). The queues are good data structures to hold  
the threads in as our queue functions will be performed in O(1) time and allow  
the easy deletion and addition to the queues to keep track of the threads'  
state. Our other global variable kept track of the number of threads, so we  
could accurately assign each new thread its TID. The ready_queue holds the  
threads that are ready to become the next current running thread. The  
zombie_queue holds the threads that returned successfully from their function  
call. The blocked_queue holds the threads that are waiting for the joined child  
to complete before they can be pushed onto the ready_queue.

### Preempt Design Choices
For preempt, we made the sigaction and timer global varaibels so that other  
files such as uthread.c or context.c could access them accordingly. We  
particularly used the cources the professor provided us with to create the  
functions. As a result, this section did not require too much high level  
choices.

-------------------------------------------------------------------------------
## Testing Methods Used

### Queue Testing
To test our queue we created a new function (now deleted) that could print the  
queue.  We called it in many locations within our other functions in order to be  
sure that the queue contained what it was supposed to at each point during the  
process. We also created the file queue_tester.c which has functions to test  
many base and edge cases. For the queue_tester.c, we broke down each queue  
function and tested it for each of the events that could return a -1, and a case  
for returning a 0.

### Uthread Testing
To test our uthread we used the tester files provided to check our yield and  
create functions. We then edited the uthread_yield.c file to test the join  
function. We added lines of code that would attempt to join a thread but could  
not be done because they were trying to join a main, they were trying to join  
itself, or they were trying to join a thread that already has a parent. As a  
result the uthread_yield.c still produces the same ouput because the join  
functions we added do not change the scheduling of the threads.

### Preempt Testing
To test our preemption, in test_preempt.c we implemented a program that  
contained an infinite loop.  So, without preemption, one of the threads would  
print a certain line infinitely, but with preemption it would eventually yield  
to another thread. As a result, our test successfully prints out thread1,  
thread3 several times, and then thread 2. This proves that our timer is working  
and will call the signal handler to yield to the next ready thread in the  
ready_queue.

-------------------------------------------------------------------------------
## Sources Used
In order to understand threads and multithreading conceptually we used the  
textbook Operating Systems: Principles & Practice, 2nd edition. The professor’s  
lectures about threads, specifically his diagram visually showing the lifecycle  
of threads, was especially helpful. We talked about our high level design  
choices with the TAs and the ECS150 tutors. Our preempt execution was largely  
influenced by the gnu.org links provided in the project description. We used  
stackoverflow.com to help us understand how to create a static Makefile. We also  
used the professor's Makefile discussion slides as a guide for our Makefile.
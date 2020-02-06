# User-Level Thread Library Implementation Report
=================================================

## High-Level Design Choices

### Queue Design Choices
We decided to implement the queue as a linked list, to account for the necessary dynamic quality of the queue, and so we could easily use pointers to the first and last nodes in the list to make the queue FIFO.  To do this, we created seperate structs for the queue and for the nodes (which is where the actual data item is stored).  We then had to add another function to create the nodes we would add to the queue.

### Uthread Design Choices
To implement the multithreading, we created a data structure to represent the Thread Control Block for the main thread and each thread added by the user.
//add info and what tcb contains and why?
These tcb structs were then put into various global queues representing the potential states they were in at any given time, except for the currently running queue, for which we used a tcb pointer.  
//add why queues are good structs to keep threads in?
Our other global variable kept track of the number of threads, so we could accurately assign each new thread its TID.
//talk about other funcs?
//talk about adding func that compares tids of threads?

### Preempt Design Choices
//dont think any high level choices were made, mostly just worked from his //sources and other documentation about signals and timers


## Testing Methods Used

### Queue Testing
To test our queue we created a new function (now deleted) that could print the queue.  We called it in many locations within our other functions in order to be sure that the queue contained what it was supposed to at each point during the process.

### Uthread Testing

### Preempt Testing
To test our preemption, in test_preempt.c we implemented a program that contained an infinite loop.  So, without preemption, one of the threads would print a certain line infinitely, but with preemption it would eventually yield to another thread.


## Sources Used
In order to understand threads and multithreading conceptually we used the textbook Operating Systems: Principles & Practice, 2nd edition.
//and other online sources?
The professors lectures about threads, specifically his diagram visually showing the lifecycle of threads, was especially helpful.
Our preempt execution was largely influenced by the gnu.org links provided in the project description.


#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

//node for linked list
struct queue_node {
	struct queue_node * next_node;
	void * data_item;
};

//queue implemented using a linked list
struct queue {
	struct queue_node * head;
	struct queue_node * tail;
	int length;
};

/*
static void print_queue(queue_t queue)
{
	//temp func for testing
	struct queue_node * iterator = queue->head;
	printf("queue length = %d\n", queue->length);
	while(iterator != NULL){
		printf("%d\n", *(int*)iterator->data_item);
		iterator = iterator->next_node;
	}
}
*/

queue_t queue_create(void)
{
	queue_t new_queue = (queue_t)malloc(sizeof(struct queue));	//alloc space for queue
	if(new_queue != NULL){
		//init queue
		new_queue->head = NULL;
		new_queue->tail = NULL;
		new_queue->length = 0;
	}
	return new_queue;
}

int queue_destroy(queue_t queue)
{
	if(queue == NULL || queue->length != 0){
		//cannot destroy queue that does not exist or that still has items in it
		return -1;
	}
	else {
		free(queue);
		return 0;
	}
}

static struct queue_node* node_create(void * data)
{
	struct queue_node * new_node = (struct queue_node *)malloc(sizeof(struct queue_node)); //alloc space for new node
	if(new_node != NULL){
		//init node
		new_node->next_node = NULL;
		new_node->data_item = data;
	}
	return new_node;
}

int queue_enqueue(queue_t queue, void *data)
{
	if((queue == NULL) || (data == NULL)){
		//cannot enqueue if queue or data is NULL
		return -1;
	}
	struct queue_node * new_node = node_create(data);	//create new node to add to queue
	if(new_node == NULL){
		return -1;
	}
	if(queue->length == 0){
		//if queue is empty, new node is both head and tail
		queue->head = new_node;
		queue->tail = queue->head;
	}
	else{
		//add new node to end of queue
		queue->tail->next_node = new_node;
		queue->tail = queue->tail->next_node;
	}
	queue->length++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if(queue == NULL || *data == NULL || queue->length == 0){
		//cannot dequeue empty or nonexistent queue, or to location that does not exist
		return -1;
	}
	*data = queue->head->data_item;	//set data to item pointed to by queue head (oldest item in queue)
	struct queue_node * temp = queue->head;	//create temp node to free item pointed to by head
	queue->head = queue->head->next_node;	//set head to next node
	free(temp);
	queue->length--;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if((queue == NULL) || (data == NULL) || (queue->length == 0)){
		return -1;
	}
	struct queue_node * iterator = queue->head;	//create node to iterate through queue
	if(iterator->data_item == data){
		//if head of queue is a match, set head to next node
		queue->head = queue->head->next_node;
		free(iterator);
		queue->length--;
		return 0;
	}
	else{
		struct queue_node * prev_node = queue->head;	//keep track of previous node
		iterator = iterator->next_node;
		while(iterator != NULL){
			if(iterator->data_item == data){
				//data found, delete item
				prev_node->next_node = iterator->next_node;
				free(iterator);
				queue->length--;
				return 0;
			}
			prev_node = iterator;	//increase iterators
			iterator = iterator->next_node;
		}
	}
	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	if((queue == NULL) || (func == NULL)){
		//cannot iterate through nonexistent queue or if there is no func to call on items
		return -1;
	}
	else {
		struct queue_node * iterator = queue->head;	//create queue iterator
		while(iterator != NULL){
			//for each item in queue, call func on item
			int func_ret = func(iterator->data_item, arg);
			if(func_ret == 1){
				//if func returns 1 and @data is not NULL, collect item where func stopped and return
				if(data != NULL){
					*data = iterator->data_item;
				}
				return 0;
			}
			else {
				iterator = iterator->next_node;	//inc iterator
			}
		}
		return 0;
	}
}

int queue_length(queue_t queue)
{
	if(queue != NULL){
		//check that queue exists
		return queue->length;
	}
	return -1;
}

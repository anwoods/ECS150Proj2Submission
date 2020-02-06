#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h> 
#include <queue.h>
#include <stdio.h>


void test_create(void){
    queue_t q;

    q = queue_create();
    assert(q != NULL);
}

void test_enqueue(void){
    queue_t q;
    q = queue_create();
    int data = 3;
    queue_enqueue(q, &data);
}

void test_queue_simple(void){
    queue_t q;
    int data = 3, *ptr;

    q = queue_create();
    queue_enqueue(q, &data);
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data);
}

void test_delete_simple(void){
    queue_t q;
    int data = 3;

    q = queue_create();
    queue_enqueue(q, &data);
    queue_delete(q, &data);
}

void test_more_enqueue(void){
    queue_t q;
    int data1 = 3;
    int data2 = 5;
    int data3 = 7;
    int data4 = 9;
    int * ptr;
    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data3);
    queue_enqueue(q, &data4);
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data1);
}

void test_more_enqueue2(void){
    queue_t q;
    int data1 = 3;
    int data2 = 5;
    int data3 = 7;
    int data4 = 9;
    int * ptr;
    int * ptr2;
    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data3);
    queue_enqueue(q, &data4);
    queue_dequeue(q, (void**)&ptr);
    queue_dequeue(q, (void**)&ptr2);
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data3);
}

void test_queue_destroy_empty(void){
    queue_t q;

    q = queue_create();
    queue_destroy(q);
    q = NULL;
    assert(q == NULL);
}

void test_queue_destroy_full(void){
    queue_t q;
    int data1 = 3;
    q = queue_create();
    queue_enqueue(q, &data1);
    int res = queue_destroy(q);
    assert(res == -1);
}

void test_queue_destroy_null(void){
    queue_t q;
    
    q = queue_create();
    q = NULL;
    int res = queue_destroy(q);
    assert(res == -1);
}

void test_queue_destroy_empty2(){
    queue_t q;
    int data1 = 3;
    int * ptr;
    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data1);
    queue_dequeue(q, (void**)&ptr);
    queue_delete(q, &data1);
    int res = queue_destroy(q);
    assert(res == 0);
}

void test_queue_0_length(){
    queue_t q;

    q = queue_create();
    int res = queue_length(q);
    assert(res == 0);
}

void test_queue_1_length(){
    queue_t q;
    int data1 = 3;
    q = queue_create();
    queue_enqueue(q, &data1);
    int res = queue_length(q);
    assert(res == 1);
}

void test_queue_2_length(){
    queue_t q;
    int data1 = 3;
    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data1);
    int res = queue_length(q);
    assert(res == 2);
}

void test_queue_3_length(){
    queue_t q;
    int data1 = 3;
    int data2 = 5;
    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    int res = queue_length(q);
    assert(res == 3);
}

void test_queue_3_length_dequeue(){
    queue_t q;
    int data1 = 3;
    int data2 = 5;
    int * ptr;
    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data1);
    queue_dequeue(q, (void**)&ptr);
    int res = queue_length(q);
    assert(res == 3);
}

void test_queue_2_length_dequeue2(){
    queue_t q;
    int data1 = 3;
    int data2 = 5;
    int * ptr;
    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data1);
    queue_dequeue(q, (void**)&ptr);
    queue_dequeue(q, (void**)&ptr);
    int res = queue_length(q);
    assert(res == 2);
}

void test_queue_2_length_delete(){
    queue_t q;
    int data1 = 3;
    int data2 = 5;
    int data3 = 7;
    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data3);
    queue_delete(q, &data2);
    int res = queue_length(q);
    assert(res == 2);
}

void test_queue_2_length_delete2(){
    queue_t q;
    int data1 = 3;
    int data2 = 5;
    int data3 = 7;
    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data3);
    queue_delete(q, &data2);
    queue_delete(q, &data3);
    int res = queue_length(q);
    assert(res == 2);
}

void test_queue_NULL_length(){
    queue_t q;
    q = NULL;
    int res = queue_length(q);
    assert(res == -1);
}

void test_delete_2(void){
    queue_t q;
    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    int *ptr;

    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data3);
    queue_delete(q, &data2);
    queue_delete(q, &data3);
    queue_dequeue(q, (void**)&ptr);
}

int add(void *data, void *arg){
    printf("data item is %d\n", *(int*)data);
    printf("arg is %d\n", *(int*)arg);
    return 0;
}

int add2(void *data, void *arg){
    printf("data item is %d\n", *(int*)data);
    printf("arg is %d\n", *(int*)arg);
    if(*(int*)data == 3){
        return 1;
    }
    return 0;
}

void test_iterate(void){
    queue_t q;
    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    int data4 = 4;
    int arg = 2;
    int *ptr;

    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data3);
    queue_enqueue(q, &data4);

    queue_iterate(q, &add, &arg, (void**)&ptr);
}

void test_iterate2(void){
    queue_t q;
    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    int data4 = 4;
    int arg = 2;
    int *ptr;

    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data3);
    queue_enqueue(q, &data4);

    queue_iterate(q, &add2, &arg, (void**)&ptr);
    printf("ptr is %d\n", *ptr );
    assert(ptr == &data3);
}

void test_iterate3(){
    queue_t q;
    int data1 = 1;
    int data2 = 2;
    int data3 = 3;
    int data4 = 4;
    int arg = 2;
    int *ptr;

    q = queue_create();
    queue_enqueue(q, &data1);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data3);
    queue_enqueue(q, &data4);
    int res = queue_iterate(q, NULL, &arg, (void**)&ptr);
    assert(res == -1);
}

static int inc_item(void *data, void *arg)
{
    int *a = (int*)data;
    int inc = (int)(long)arg;

    *a += inc;

    return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(void *data, void *arg)
{
    int *a = (int*)data;
    int match = (int)(long)arg;

    if (*a == match){
        return 1;
    }

    return 0;
}

void test_iterator4(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    int *ptr;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < (int)(sizeof(data) / sizeof(data[0])); i++)
        queue_enqueue(q, &data[i]);

    /* Add value '1' to every item of the queue */
    queue_iterate(q, inc_item, (void*)1, NULL);
    assert(data[0] == 2);

    /* Find and get the item which is equal to value '5' */
    ptr = NULL;
    queue_iterate(q, find_item, (void*)5, (void**)&ptr);
    assert(ptr != NULL);
    assert(*ptr == 5);
    assert(ptr == &data[3]);
}

void run_tests_all(){
	test_create();
    test_enqueue();
	test_queue_simple();
    test_delete_simple();
    test_more_enqueue();
    test_more_enqueue2();
    test_queue_destroy_empty();
    test_queue_destroy_full();
    test_queue_destroy_null();
    test_queue_destroy_empty2();
    test_queue_0_length();
    test_queue_1_length();
    test_queue_2_length();
    test_queue_3_length();
    test_queue_3_length_dequeue();
    test_queue_2_length_dequeue2();
    test_queue_2_length_delete();
    test_queue_2_length_delete2();
    test_queue_NULL_length();
    test_delete_2();
    test_iterate();
    test_iterate2();
    test_iterate3();
    test_iterator4();
}





int main(void){
	run_tests_all();
	return 0;
}

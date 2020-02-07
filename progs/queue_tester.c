#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)			\
do {						\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {				\
		printf("PASS\n");		\
	} else	{				\
		printf("FAIL\n");		\
		exit(1);			\
	}					\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

void test_queue_destroy_not_empty(void){
    queue_t q;
    int data1 = 3;
    fprintf(stderr, "*** TEST queue_destroy_not_empty ***\n");
    q = queue_create();
    queue_enqueue(q, &data1);
    int res = queue_destroy(q);
    TEST_ASSERT(res == -1);
}

void test_queue_destroy_empty(void){
    queue_t q;
    fprintf(stderr, "*** TEST queue_destroy_empty ***\n");
    q = queue_create();
    int res = queue_destroy(q);
    q = NULL;
    TEST_ASSERT(q == NULL);
    TEST_ASSERT(res == 0);
}

void test_queue_destroy_null(void){
    queue_t q;
    fprintf(stderr, "*** TEST queue_destroy_null ***\n");
    q = queue_create();
    q = NULL;
    int res = queue_destroy(q);
    TEST_ASSERT(res == -1);
}

void test_enqueue_null_queue(void){
	queue_t q;
	int data = 3;
    fprintf(stderr, "*** TEST enqueue_null_queue ***\n");
    q = queue_create();
    q = NULL;
    int res = queue_enqueue(q, &data);
    TEST_ASSERT(res == -1);
}

void test_enqueue_null_data(void){
	queue_t q;
    fprintf(stderr, "*** TEST enqueue_null_data ***\n");
    q = queue_create();
    int res = queue_enqueue(q, NULL);
    TEST_ASSERT(res == -1);
}

void test_enqueue_success(void){
    queue_t q;
    fprintf(stderr, "*** TEST enqueue_success ***\n");
    q = queue_create();
    int data = 3;
    int res = queue_enqueue(q, &data);
    TEST_ASSERT(res == 0);
    TEST_ASSERT(queue_length(q) == 1);
}

void test_dequeue_null_queue(void){
	queue_t q;
	int *ptr;
    fprintf(stderr, "*** TEST dequeue_null_queue ***\n");
    q = NULL;
    int res = queue_dequeue(q, (void**)&ptr);
    TEST_ASSERT(res == -1);
}

void test_dequeue_null_data(void){
	queue_t q;
	int data = 3;
    fprintf(stderr, "*** TEST dequeue_null_data ***\n");
    q = queue_create();
    queue_enqueue(q, &data);
    int res = queue_dequeue(q, NULL);
    TEST_ASSERT(res == -1);
}

void test_dequeue_empty_queue(void){
	queue_t q;
	int *ptr;
    fprintf(stderr, "*** TEST dequeue_empty_queue ***\n");
    q = queue_create();
    int res = queue_dequeue(q, (void**)&ptr);
    TEST_ASSERT(res == -1);
}

void test_delete_null_queue(void){
	queue_t q;
	int data = 3;
    fprintf(stderr, "*** TEST delete_null_queue ***\n");
    q = NULL;
    int res = queue_delete(q, &data);
    TEST_ASSERT(res == -1);
}

void test_delete_null_data(void){
	queue_t q;
	int data = 3;
    fprintf(stderr, "*** TEST delete_null_data ***\n");
    q = queue_create();
    queue_enqueue(q, &data);
    int res = queue_delete(q, NULL);
    TEST_ASSERT(res == -1);
}

void test_delete_data_not_found(void){
	queue_t q;
	int data = 3;
	int not_in_queue_data = 4;
    fprintf(stderr, "*** TEST delete_data_not_found ***\n");
    q = queue_create();
    queue_enqueue(q, &data);
    int res = queue_delete(q, &not_in_queue_data);
    TEST_ASSERT(res == -1);
}

void test_delete_empty_queue(void){
	queue_t q;
	int not_in_queue_data = 4;
    fprintf(stderr, "*** TEST delete_empty_queue ***\n");
    q = queue_create();
    int res = queue_delete(q, &not_in_queue_data);
    TEST_ASSERT(res == -1);
}

void test_delete_simple(void){
    queue_t q;
    int data = 3;
    fprintf(stderr, "*** TEST delete_simple ***\n");
    q = queue_create();
    queue_enqueue(q, &data);
    int res = queue_delete(q, &data);
    TEST_ASSERT(res == 0);
    TEST_ASSERT(queue_length(q) == 0);
}

void test_delete_middle(void){
    queue_t q;
    int data = 1;
    int data2 = 2;
    int data3 = 3;
    fprintf(stderr, "*** TEST delete_middle ***\n");
    q = queue_create();
    queue_enqueue(q, &data);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data3);
    int res = queue_delete(q, &data2);
    TEST_ASSERT(res == 0);
    TEST_ASSERT(queue_length(q) == 2);
}

void test_delete_head(void){
    queue_t q;
    int data = 1;
    int data2 = 2;
    int data3 = 3;
    fprintf(stderr, "*** TEST delete_head ***\n");
    q = queue_create();
    queue_enqueue(q, &data);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data3);
    int res = queue_delete(q, &data);
    TEST_ASSERT(res == 0);
    TEST_ASSERT(queue_length(q) == 2);
}

void test_delete_tail(void){
    queue_t q;
    int data = 1;
    int data2 = 2;
    int data3 = 3;
    fprintf(stderr, "*** TEST delete_tail ***\n");
    q = queue_create();
    queue_enqueue(q, &data);
    queue_enqueue(q, &data2);
    queue_enqueue(q, &data3);
    int res = queue_delete(q, &data3);
    TEST_ASSERT(res == 0);
    TEST_ASSERT(queue_length(q) == 2);
}

/* Callback function that increments items by a certain value */
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

    if (*a == match)
        return 1;

    return 0;
}

void test_iterator(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    int *ptr;
    fprintf(stderr, "*** TEST iterator ***\n");

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Add value '1' to every item of the queue */
    queue_iterate(q, inc_item, (void*)1, NULL);
    TEST_ASSERT(data[0] == 2);

    /* Find and get the item which is equal to value '5' */
    ptr = NULL;
    queue_iterate(q, find_item, (void*)5, (void**)&ptr);
    TEST_ASSERT(ptr != NULL);
    TEST_ASSERT(*ptr == 5);
    TEST_ASSERT(ptr == &data[3]);
}

void test_iterator_null_queue(void){
	queue_t q;
	fprintf(stderr, "*** TEST iterator_null_queue ***\n");
	q = NULL;
	int res = queue_iterate(q, inc_item, (void*)1, NULL);
	TEST_ASSERT(res == -1);
}

void test_iterator_null_func(void){
	queue_t q;
	fprintf(stderr, "*** TEST iterator_null_func ***\n");
	int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);
	int res = queue_iterate(q, NULL, (void*)1, NULL);
	TEST_ASSERT(res == -1);
}

void test_length_queue_null(void){
	queue_t q;
	fprintf(stderr, "*** TEST length_queue_null ***\n");
	q = NULL;
	int res = queue_length(q);
	TEST_ASSERT(res == -1);
}


int main(void)
{
	test_create();
	test_queue_simple();
	test_queue_destroy_not_empty();
	test_queue_destroy_empty();
	test_queue_destroy_null();
	test_enqueue_null_queue();
	test_enqueue_null_data();
	test_enqueue_success();
	test_dequeue_null_queue();
	test_dequeue_null_data();
	test_dequeue_empty_queue();
	test_delete_null_queue();
	test_delete_null_data();
	test_delete_data_not_found();
	test_delete_empty_queue();
	test_delete_simple();
	test_delete_middle();
	test_delete_head();
	test_delete_tail();
	test_iterator();
	test_iterator_null_queue();
	test_iterator_null_func();
	test_length_queue_null();

	return 0;
}

#include "queue.h"
#include <stdio.h>

#define QUEUE_CAPACITY 5U

static int buffer[QUEUE_CAPACITY];
static queue_t int_queue;

int main(void)
{
    queue_init(&int_queue, buffer, sizeof(int), QUEUE_CAPACITY);

    int value = 10;
    (void)queue_push(&int_queue, &value);

    int out = 0;
    (void)queue_pop(&int_queue, &out);

    printf("Read value: %d\n", out); // Output: 10
    return 0;
}
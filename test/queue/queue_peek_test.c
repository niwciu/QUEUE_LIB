#include "unity/fixture/unity_fixture.h"
#include "queue.h"

#define QUEUE_CAPACITY 5U

TEST_GROUP(queue_peek);

static queue_t q;
static int queue_buffer[QUEUE_CAPACITY];

TEST_SETUP(queue_peek)
{
    // Initialize the queue before each test
    queue_init(&q, queue_buffer, sizeof(int), QUEUE_CAPACITY);
}

TEST_TEAR_DOWN(queue_peek)
{
    // Optional cleanup after each test
}

TEST(queue_peek, GivenEmptyQueueWhenPeekThenReturnsQueueEmpty)
{
    int item;
    // Peek on empty queue should return QUEUE_EMPTY
    TEST_ASSERT_EQUAL(QUEUE_EMPTY, queue_peek(&q, &item));
}

TEST(queue_peek, GivenPeekCalledThenQueueStateDoesNotChange)
{
    int in = 42;
    int out;

    // Push one element
    queue_push(&q, &in);

    // Save head and count before peek
    uint16_t head_before = q.head;
    uint16_t count_before = q.count;

    // Peek should succeed and return the correct value
    TEST_ASSERT_EQUAL(QUEUE_OK, queue_peek(&q, &out));
    TEST_ASSERT_EQUAL(in, out);

    // Head and count should remain unchanged
    TEST_ASSERT_EQUAL(head_before, q.head);
    TEST_ASSERT_EQUAL(count_before, q.count);
}

TEST(queue_peek, GivenNonEmptyQueueWhenPeekThenReturnsFirstElement)
{
    int a = 1;
    int b = 2;
    int out;

    // Push two elements
    queue_push(&q, &a);
    queue_push(&q, &b);

    // Peek should always return the first element
    TEST_ASSERT_EQUAL(QUEUE_OK, queue_peek(&q, &out));
    TEST_ASSERT_EQUAL(a, out);
}

TEST(queue_peek, GivenMultipleItemsWhenPeekThenAlwaysReturnsOldest)
{
    int a = 5;
    int out;

    // Push a single element
    queue_push(&q, &a);

    // Multiple peeks should always return the same element
    TEST_ASSERT_EQUAL(QUEUE_OK, queue_peek(&q, &out));
    TEST_ASSERT_EQUAL(a, out);

    TEST_ASSERT_EQUAL(QUEUE_OK, queue_peek(&q, &out));
    TEST_ASSERT_EQUAL(a, out);
}

TEST(queue_peek, GivenWrapAroundQueueWhenPeekThenReturnsCorrectElement)
{
    int out;
    int values[3] = {1, 2, 3};

    // Fill queue with two elements
    queue_push(&q, &values[0]);
    queue_push(&q, &values[1]);

    // Pop one element to move head and create wrap-around scenario
    queue_pop(&q, &out);

    // Push a third element (will wrap around to the start)
    queue_push(&q, &values[2]);

    // Peek should return the oldest element still in queue (values[1])
    TEST_ASSERT_EQUAL(QUEUE_OK, queue_peek(&q, &out));
    TEST_ASSERT_EQUAL(values[1], out);
}

TEST(queue_peek, GivenNullQueuePointerWhenPeekThenReturnsError)
{
    int out;
    // Peek on NULL queue should return QUEUE_ERROR
    TEST_ASSERT_EQUAL(QUEUE_ERROR, queue_peek(NULL, &out));
}

TEST(queue_peek, GivenNullItemPointerWhenPeekThenReturnsError)
{
    // Peek with NULL item pointer should return QUEUE_ERROR
    TEST_ASSERT_EQUAL(QUEUE_ERROR, queue_peek(&q, NULL));
}
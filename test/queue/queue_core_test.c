#include "unity/fixture/unity_fixture.h"
#include "queue.h"

#define QUEUE_CAPACITY 3U

static queue_t q;
static int buffer[QUEUE_CAPACITY];

TEST_GROUP(queue_core);

TEST_SETUP(queue_core)
{
    // Initialize the queue before each test
    queue_init(&q, buffer, sizeof(int), QUEUE_CAPACITY);
}

TEST_TEAR_DOWN(queue_core)
{
    // Optional cleanup after each test
}

// Test FIFO behavior: push and pop multiple elements in loop
TEST(queue_core, GivenQueueWhenPushAndPopInLoopThenBehavesAsFifo)
{
    int input[QUEUE_CAPACITY] = {10, 20, 30};
    int output = 0;

    for (uint8_t i = 0; i < QUEUE_CAPACITY; i++)
    {
        TEST_ASSERT_EQUAL(QUEUE_OK, queue_push(&q, &input[i]));
    }

    for (uint8_t i = 0; i < QUEUE_CAPACITY; i++)
    {
        TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&q, &output));
        TEST_ASSERT_EQUAL_INT(input[i], output);
    }

    // Queue should be empty at the end
    TEST_ASSERT_TRUE(queue_is_empty(&q));
}

// Test initialization with NULL pointer returns error
TEST(queue_core, GivenQueueWhenInitWithNullPointerThenReturnsError)
{
    TEST_ASSERT_EQUAL(QUEUE_ERROR, queue_init(NULL, buffer, sizeof(int), QUEUE_CAPACITY));
    TEST_ASSERT_EQUAL(QUEUE_ERROR, queue_init(&q, NULL, sizeof(int), QUEUE_CAPACITY));
}

// Test wrap-around: fill, pop one, push again, pop all in correct order
TEST(queue_core, GivenWrapAroundScenarioThenOrderIsPreserved)
{
    int values[QUEUE_CAPACITY] = {1, 2, 3};
    int output = 0;

    // Fill queue completely
    for (uint8_t i = 0; i < QUEUE_CAPACITY; i++)
        queue_push(&q, &values[i]);

    // Pop one element to advance head
    TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&q, &output));
    TEST_ASSERT_EQUAL_INT(values[0], output);

    // Push one more element to wrap around
    int new_value = 4;
    TEST_ASSERT_EQUAL(QUEUE_OK, queue_push(&q, &new_value));

    // Now pop all elements and check order
    int expected_order[QUEUE_CAPACITY] = {2, 3, 4};
    for (uint8_t i = 0; i < QUEUE_CAPACITY; i++)
    {
        TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&q, &output));
        TEST_ASSERT_EQUAL_INT(expected_order[i], output);
    }

    TEST_ASSERT_TRUE(queue_is_empty(&q));
}

// Test pushing to full queue fails, count remains unchanged
TEST(queue_core, GivenQueueFullWhenPushThenPushFailsAndCountUnchanged)
{
    int value = 5;
    for (uint8_t i = 0; i < QUEUE_CAPACITY; i++)
        queue_push(&q, &value);

    TEST_ASSERT_TRUE(queue_is_full(&q));

    queue_status_t result = queue_push(&q, &value);
    TEST_ASSERT_EQUAL(QUEUE_FULL, result);
    TEST_ASSERT_EQUAL(QUEUE_CAPACITY, q.count);
}

// Test popping from empty queue repeatedly returns QUEUE_EMPTY
TEST(queue_core, GivenEmptyQueueWhenPopMultipleTimesThenAlwaysReturnsQueueEmpty)
{
    int output;
    for (int i = 0; i < 3; i++)
    {
        TEST_ASSERT_EQUAL(QUEUE_EMPTY, queue_pop(&q, &output));
        TEST_ASSERT_TRUE(queue_is_empty(&q));
    }
}
#include "unity/fixture/unity_fixture.h"
#include "queue.h"

#define QUEUE_CAPACITY 3

static queue_t q;
static int buffer[QUEUE_CAPACITY];

TEST_GROUP(queue_push);

TEST_SETUP(queue_push)
{
    // Initialize the queue before each test
    queue_init(&q, buffer, sizeof(int), QUEUE_CAPACITY);
}

TEST_TEAR_DOWN(queue_push)
{
    // Optional cleanup after each test
}

// Test pushing one item into an empty queue
TEST(queue_push, GivenEmptyQueueWhenPushOneItemThenQueueIsNotEmpty)
{
    int value = 10;
    queue_status_t result = queue_push(&q, &value);

    // Push should succeed
    TEST_ASSERT_EQUAL(QUEUE_OK, result);

    // Queue should no longer be empty
    TEST_ASSERT_FALSE(queue_is_empty(&q));

    // Queue should not report full
    TEST_ASSERT_FALSE(queue_is_full(&q));

    // Count should be 1
    TEST_ASSERT_EQUAL(1, q.count);
}

// Test pushing until the queue is full
TEST(queue_push, GivenFullQueueWhenPushItemThenPushFails)
{
    int value = 1;
    queue_status_t result;

    // Fill the queue
    for (int i = 0; i < QUEUE_CAPACITY; i++)
    {
        result = queue_push(&q, &value);
        TEST_ASSERT_EQUAL(QUEUE_OK, result);
    }

    // Queue should report full
    TEST_ASSERT_TRUE(queue_is_full(&q));
    TEST_ASSERT_FALSE(queue_is_empty(&q));
    TEST_ASSERT_EQUAL(QUEUE_CAPACITY, q.count);

    // Additional push should fail with QUEUE_FULL
    result = queue_push(&q, &value);
    TEST_ASSERT_EQUAL(QUEUE_FULL, result);

    // Count should remain at capacity
    TEST_ASSERT_EQUAL(QUEUE_CAPACITY, q.count);
}

// Test pushing NULL pointer should return QUEUE_ERROR
TEST(queue_push, GivenQueueWhenPushNullPointerThenReturnsError)
{
    TEST_ASSERT_EQUAL(QUEUE_ERROR, queue_push(&q, NULL));

    // Count should remain zero
    TEST_ASSERT_EQUAL(0, q.count);
    TEST_ASSERT_TRUE(queue_is_empty(&q));
}

// Test that after each push, count increments correctly
TEST(queue_push, GivenQueueWhenPushMultipleItemsThenCountIncrements)
{
    int value = 5;
    queue_status_t result;

    for (int i = 0; i < QUEUE_CAPACITY; i++)
    {
        result = queue_push(&q, &value);
        TEST_ASSERT_EQUAL(QUEUE_OK, result);
        TEST_ASSERT_EQUAL(i + 1, q.count);
    }
}

// Test that pushing to a full queue does not change count
TEST(queue_push, GivenQueueFullWhenPushFailsThenCountUnchanged)
{
    int value = 7;
    queue_status_t result;

    // Fill the queue
    for (int i = 0; i < QUEUE_CAPACITY; i++)
    {
        result = queue_push(&q, &value);
        TEST_ASSERT_EQUAL(QUEUE_OK, result);
    }

    // Attempt to push one more item
    result = queue_push(&q, &value);
    TEST_ASSERT_EQUAL(QUEUE_FULL, result);

    // Count should remain equal to capacity
    TEST_ASSERT_EQUAL(QUEUE_CAPACITY, q.count);
}
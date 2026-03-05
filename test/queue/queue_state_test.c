#include "unity/fixture/unity_fixture.h"
#include "queue.h"

#define QUEUE_CAPACITY 3U

static queue_t q;
static int buffer[QUEUE_CAPACITY];

TEST_GROUP(queue_state);

TEST_SETUP(queue_state)
{
    // Initialize the queue before each test
    queue_init(&q, buffer, sizeof(int), QUEUE_CAPACITY);
}

TEST_TEAR_DOWN(queue_state)
{
    // Optional cleanup
}

// Test that a newly initialized queue is empty
TEST(queue_state, GivenNewQueueThenIsEmptyReturnsTrue)
{
    TEST_ASSERT_TRUE(queue_is_empty(&q));
    TEST_ASSERT_FALSE(queue_is_full(&q));
}

// Test that a NULL queue pointer returns empty as true and full as false
TEST(queue_state, GivenNullQueuePointerThenReturnsSafeValues)
{
    TEST_ASSERT_TRUE(queue_is_empty(NULL));
    TEST_ASSERT_FALSE(queue_is_full(NULL));
}

// Test that a queue with count == capacity returns full
TEST(queue_state, GivenQueueWithCountEqualCapacityThenIsFullReturnsTrue)
{
    q.count = q.capacity;
    TEST_ASSERT_TRUE(queue_is_full(&q));
    TEST_ASSERT_FALSE(queue_is_empty(&q));
}

// Test that a queue with count == 0 returns empty
TEST(queue_state, GivenQueueWithCountZeroThenIsEmptyReturnsTrue)
{
    q.count = 0;
    TEST_ASSERT_TRUE(queue_is_empty(&q));
    TEST_ASSERT_FALSE(queue_is_full(&q));
}

// Test that a partially filled queue is neither empty nor full
TEST(queue_state, GivenPartiallyFilledQueueThenIsEmptyAndIsFullReturnFalse)
{
    q.count = 1;
    TEST_ASSERT_FALSE(queue_is_empty(&q));
    TEST_ASSERT_FALSE(queue_is_full(&q));
}

// Test that incrementing count to capacity switches is_full to true
TEST(queue_state, GivenCountIncrementedToCapacityThenIsFullBecomesTrue)
{
    q.count = QUEUE_CAPACITY - 1;
    TEST_ASSERT_FALSE(queue_is_full(&q));

    q.count++;
    TEST_ASSERT_TRUE(queue_is_full(&q));
}

// Test that decrementing count from capacity switches is_full to false
TEST(queue_state, GivenCountDecrementedFromCapacityThenIsFullBecomesFalse)
{
    q.count = QUEUE_CAPACITY;
    TEST_ASSERT_TRUE(queue_is_full(&q));

    q.count--;
    TEST_ASSERT_FALSE(queue_is_full(&q));
}
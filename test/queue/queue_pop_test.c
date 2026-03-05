#include "unity/fixture/unity_fixture.h"
#include "queue.h"

#define QUEUE_CAPACITY 3

static queue_t q;
static int buffer[QUEUE_CAPACITY];

TEST_GROUP(queue_pop);

TEST_SETUP(queue_pop)
{
    // Initialize queue with element size and capacity before each test
    queue_init(&q, buffer, sizeof(int), QUEUE_CAPACITY);
}

TEST_TEAR_DOWN(queue_pop)
{
    // Optional cleanup
}

// Placeholder test
TEST(queue_pop, FirstTest)
{
    TEST_FAIL_MESSAGE("Implement your test!");
}

// Test popping from a non-empty queue returns correct value and updates count
TEST(queue_pop, GivenNonEmptyQueueWhenPopItemThenReturnsCorrectValue)
{
    // Manually set head, tail, count, and buffer
    q.head = 0;
    q.tail = 0;
    q.count = 1;
    buffer[0] = 42;

    int output = 0;
    queue_status_t result = queue_pop(&q, &output);

    // Should succeed and return the stored value
    TEST_ASSERT_EQUAL(QUEUE_OK, result);
    TEST_ASSERT_EQUAL_INT(42, output);

    // Count should decrement, queue should be empty
    TEST_ASSERT_EQUAL(0, q.count);
    TEST_ASSERT_TRUE(queue_is_empty(&q));
}

// Test popping from an empty queue returns QUEUE_EMPTY
TEST(queue_pop, GivenEmptyQueueWhenPopThenReturnsQueueEmptyStatus)
{
    q.count = 0; // Ensure queue is empty

    int output = 0;
    queue_status_t result = queue_pop(&q, &output);

    TEST_ASSERT_EQUAL(QUEUE_EMPTY, result);
    TEST_ASSERT_TRUE(queue_is_empty(&q));
}

// Test popping all items reduces count to zero
TEST(queue_pop, GivenQueueWhenPopAllItemsThenQueueIsEmpty)
{
    // Setup queue manually
    q.head = 0;
    q.tail = 0;
    q.count = QUEUE_CAPACITY;
    buffer[0] = 1;
    buffer[1] = 2;
    buffer[2] = 3;

    int output = 0;
    queue_status_t result;

    for (int i = 0; i < QUEUE_CAPACITY; i++)
    {
        result = queue_pop(&q, &output);
        TEST_ASSERT_EQUAL(QUEUE_OK, result);
        TEST_ASSERT_EQUAL_INT(buffer[i], output);
        TEST_ASSERT_EQUAL(QUEUE_CAPACITY - (i + 1), q.count);
    }

    // After all pops, queue should be empty
    TEST_ASSERT_TRUE(queue_is_empty(&q));
    TEST_ASSERT_FALSE(queue_is_full(&q));
}

// Test popping with NULL queue pointer returns error
TEST(queue_pop, GivenNullQueuePointerWhenPopThenReturnsError)
{
    int output = 0;
    TEST_ASSERT_EQUAL(QUEUE_ERROR, queue_pop(NULL, &output));
}

// Test popping with NULL item pointer returns error
TEST(queue_pop, GivenNullItemPointerWhenPopThenReturnsError)
{
    // Setup queue with one element
    q.head = 0;
    q.tail = 0;
    q.count = 1;
    buffer[0] = 10;

    TEST_ASSERT_EQUAL(QUEUE_ERROR, queue_pop(&q, NULL));

    // Count should remain unchanged
    TEST_ASSERT_EQUAL(1, q.count);
}

// Test wrap-around behavior manually without push
TEST(queue_pop, GivenWrapAroundQueueWhenPopThenReturnsCorrectElement)
{
    int in[5] = {10, 20, 30, 40, 50};
    int out = 0;
    const uint16_t capacity = 3;
    int buffer_wrap[capacity];
    queue_t q_wrap;

    queue_init(&q_wrap, buffer_wrap, sizeof(int), capacity);

    // Fill queue completely
    queue_push(&q_wrap, &in[0]); // 10
    queue_push(&q_wrap, &in[1]); // 20
    queue_push(&q_wrap, &in[2]); // 30

    // Pop one element to create space for wrap-around
    queue_pop(&q_wrap, &out);
    TEST_ASSERT_EQUAL_INT(10, out);

    // Push another element, will wrap-around to buffer[0]
    queue_push(&q_wrap, &in[3]); // 40

    // Pop remaining elements in FIFO order
    queue_pop(&q_wrap, &out);
    TEST_ASSERT_EQUAL_INT(20, out);
    queue_pop(&q_wrap, &out);
    TEST_ASSERT_EQUAL_INT(30, out);
    queue_pop(&q_wrap, &out);
    TEST_ASSERT_EQUAL_INT(40, out);

    // Queue should now be empty
    TEST_ASSERT_TRUE(queue_is_empty(&q_wrap));
}
#include "unity/fixture/unity_fixture.h"
#include "queue.h"

#define QUEUE_CAPACITY 3

static queue_t q;
static int buffer[QUEUE_CAPACITY];

TEST_GROUP(queue);

TEST_SETUP(queue)
{
    /* Init before every test */
    queue_init(&q, buffer, sizeof(int), QUEUE_CAPACITY);
}

TEST_TEAR_DOWN(queue)
{
    /* Cleanup after every test */
}

TEST(queue, GivenEmptyQueueWhenPushOneItemThenQueueIsNotEmpty)
{
    int value = 10;
    queue_status_t result = queue_push(&q, &value);
    TEST_ASSERT_EQUAL(QUEUE_OK, result);
    TEST_ASSERT_FALSE(queue_is_empty(&q));
}

TEST(queue, GivenFullQueueWhenPushItemThenPushFails)
{
    int value = 1;
    for (int i = 0; i < QUEUE_CAPACITY; i++)
    {
        queue_push(&q, &value);
    }
    TEST_ASSERT_TRUE(queue_is_full(&q));

    queue_status_t result = queue_push(&q, &value);
    TEST_ASSERT_EQUAL(QUEUE_FULL, result);
}

TEST(queue, GivenNonEmptyQueueWhenPopItemThenReturnsCorrectValue)
{
    int input[] = {10, 20, 30};
    for (int i = 0; i < 3; i++)
        queue_push(&q, &input[i]);

    int output = 0;
    queue_pop(&q, &output);
    TEST_ASSERT_EQUAL_INT(10, output);

    queue_pop(&q, &output);
    TEST_ASSERT_EQUAL_INT(20, output);
}

TEST(queue, GivenEmptyQueueWhenPopThenReturnsQueueEmptyStatus)
{
    int value = 0;
    queue_status_t result = queue_pop(&q, &value);
    TEST_ASSERT_EQUAL(QUEUE_EMPTY, result);
}

TEST(queue, GivenQueueWhenPushAndPopInLoopThenBehavesAsFifo)
{
    int input[] = {1, 2, 3, 4, 5};
    int output = 0;

    for (int i = 0; i < QUEUE_CAPACITY; i++)
    {
        TEST_ASSERT_EQUAL(QUEUE_OK, queue_push(&q, &input[i]));
    }

    TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&q, &output));
    TEST_ASSERT_EQUAL_INT(1, output);

    TEST_ASSERT_EQUAL(QUEUE_OK, queue_push(&q, &input[3]));

    TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&q, &output));
    TEST_ASSERT_EQUAL_INT(2, output);

    TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&q, &output));
    TEST_ASSERT_EQUAL_INT(3, output);

    TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&q, &output));
    TEST_ASSERT_EQUAL_INT(4, output);

    TEST_ASSERT_TRUE(queue_is_empty(&q));
}

TEST(queue, GivenQueueWhenPushUntilFullThenCountMatchesCapacity)
{
    int value = 42;
    for (int i = 0; i < QUEUE_CAPACITY; i++)
    {
        TEST_ASSERT_EQUAL(QUEUE_OK, queue_push(&q, &value));
    }
    TEST_ASSERT_TRUE(queue_is_full(&q));
    TEST_ASSERT_FALSE(queue_is_empty(&q));
    TEST_ASSERT_EQUAL(QUEUE_CAPACITY, q.count);
}

TEST(queue, GivenQueueWhenPopAllItemsThenQueueIsEmpty)
{
    int input[] = {5, 6, 7};
    int output = 0;
    for (int i = 0; i < QUEUE_CAPACITY; i++)
    {
        queue_push(&q, &input[i]);
    }

    for (int i = 0; i < QUEUE_CAPACITY; i++)
    {
        TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&q, &output));
        TEST_ASSERT_EQUAL_INT(input[i], output);
    }

    TEST_ASSERT_TRUE(queue_is_empty(&q));
    TEST_ASSERT_FALSE(queue_is_full(&q));
}

TEST(queue, GivenQueueWhenInitWithNullPointerThenReturnsError)
{
    queue_status_t result;

    result = queue_init(NULL, buffer, sizeof(int), QUEUE_CAPACITY);
    TEST_ASSERT_EQUAL(QUEUE_ERROR, result);

    result = queue_init(&q, NULL, sizeof(int), QUEUE_CAPACITY);
    TEST_ASSERT_EQUAL(QUEUE_ERROR, result);

    result = queue_init(&q, buffer, 0U, QUEUE_CAPACITY);
    TEST_ASSERT_EQUAL(QUEUE_ERROR, result);

    result = queue_init(&q, buffer, sizeof(int), 0U);
    TEST_ASSERT_EQUAL(QUEUE_ERROR, result);
}

#include "unity/fixture/unity_fixture.h"
#include "queue.h"

#define TEST_BUFFER_SIZE 4
static int test_buffer[TEST_BUFFER_SIZE];
static queue_t q;

TEST_GROUP(queue_init);

TEST_SETUP(queue_init) {}
TEST_TEAR_DOWN(queue_init) {}

TEST(queue_init, GivenValidParamsThenQueueIsInitializedCorrectly)
{
    queue_status_t status = queue_init(&q, test_buffer, sizeof(int), TEST_BUFFER_SIZE);
    TEST_ASSERT_EQUAL(QUEUE_OK, status);
    TEST_ASSERT_EQUAL_PTR(test_buffer, q.buffer);
    TEST_ASSERT_EQUAL_UINT16(sizeof(int), q.element_size);
    TEST_ASSERT_EQUAL_UINT16(TEST_BUFFER_SIZE, q.capacity);
    TEST_ASSERT_EQUAL_UINT16(0, q.head);
    TEST_ASSERT_EQUAL_UINT16(0, q.tail);
    TEST_ASSERT_EQUAL_UINT16(0, q.count);
}

TEST(queue_init, GivenNullQueuePointerThenReturnsError)
{
    queue_status_t status = queue_init(NULL, test_buffer, sizeof(int), TEST_BUFFER_SIZE);
    TEST_ASSERT_EQUAL(QUEUE_ERROR, status);
}

TEST(queue_init, GivenNullBufferPointerThenReturnsError)
{
    queue_status_t status = queue_init(&q, NULL, sizeof(int), TEST_BUFFER_SIZE);
    TEST_ASSERT_EQUAL(QUEUE_ERROR, status);
}

TEST(queue_init, GivenZeroElementSizeThenReturnsError)
{
    queue_status_t status = queue_init(&q, test_buffer, 0, TEST_BUFFER_SIZE);
    TEST_ASSERT_EQUAL(QUEUE_ERROR, status);
}

TEST(queue_init, GivenZeroCapacityThenReturnsError)
{
    queue_status_t status = queue_init(&q, test_buffer, sizeof(int), 0);
    TEST_ASSERT_EQUAL(QUEUE_ERROR, status);
}


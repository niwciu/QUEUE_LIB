#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(queue_init)
{
    RUN_TEST_CASE(queue_init, GivenValidParamsThenQueueIsInitializedCorrectly);
    RUN_TEST_CASE(queue_init, GivenNullQueuePointerThenReturnsError);
    RUN_TEST_CASE(queue_init, GivenNullBufferPointerThenReturnsError);
    RUN_TEST_CASE(queue_init, GivenZeroElementSizeThenReturnsError);
    RUN_TEST_CASE(queue_init, GivenZeroCapacityThenReturnsError);
}
TEST_GROUP_RUNNER(queue)
{
    /* Test cases to run */

    RUN_TEST_CASE(queue, GivenEmptyQueueWhenPushOneItemThenQueueIsNotEmpty);
    RUN_TEST_CASE(queue, GivenFullQueueWhenPushItemThenPushFails);
    RUN_TEST_CASE(queue, GivenNonEmptyQueueWhenPopItemThenReturnsCorrectValue);
    RUN_TEST_CASE(queue, GivenEmptyQueueWhenPopThenReturnsQueueEmptyStatus);
    RUN_TEST_CASE(queue, GivenQueueWhenPushAndPopInLoopThenBehavesAsFifo);
    RUN_TEST_CASE(queue, GivenQueueWhenPushNullPointerThenReturnsError);
    RUN_TEST_CASE(queue, GivenQueueWhenPopNullPointerThenReturnsError);
    RUN_TEST_CASE(queue, GivenQueueWhenPushUntilFullThenCountMatchesCapacity);
    RUN_TEST_CASE(queue, GivenQueueWhenPopAllItemsThenQueueIsEmpty);

    // RUN_TEST_CASE(queue, );
}

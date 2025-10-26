#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(queue_init)
{
    RUN_TEST_CASE(queue_init, GivenValidParamsThenQueueIsInitializedCorrectly);
    RUN_TEST_CASE(queue_init, GivenNullQueuePointerThenReturnsError);
    RUN_TEST_CASE(queue_init, GivenNullBufferPointerThenReturnsError);
    RUN_TEST_CASE(queue_init, GivenZeroElementSizeThenReturnsError);
    RUN_TEST_CASE(queue_init, GivenZeroCapacityThenReturnsError);
    RUN_TEST_CASE(queue_init, GivenValidParamsThenQueueIsEmptyAfterInit);
    RUN_TEST_CASE(queue_init, GivenQueueInitializedTwiceThenStateIsCorrect);
    RUN_TEST_CASE(queue_init, GivenMaxElementSizeAndCapacityThenInitSucceeds);
}

TEST_GROUP_RUNNER(queue)
{
    /* FIFO logic and init tests */

    RUN_TEST_CASE(queue, GivenEmptyQueueWhenPushOneItemThenQueueIsNotEmpty);
    RUN_TEST_CASE(queue, GivenFullQueueWhenPushItemThenPushFails);
    RUN_TEST_CASE(queue, GivenNonEmptyQueueWhenPopItemThenReturnsCorrectValue);
    RUN_TEST_CASE(queue, GivenEmptyQueueWhenPopThenReturnsQueueEmptyStatus);
    RUN_TEST_CASE(queue, GivenQueueWhenPushAndPopInLoopThenBehavesAsFifo);
    RUN_TEST_CASE(queue, GivenQueueWhenPushUntilFullThenCountMatchesCapacity);
    RUN_TEST_CASE(queue, GivenQueueWhenPopAllItemsThenQueueIsEmpty);
    RUN_TEST_CASE(queue, GivenQueueWhenInitWithNullPointerThenReturnsError);
}

TEST_GROUP_RUNNER(DV_QUEUE_001)
{
    RUN_TEST_CASE(DV_QUEUE_001, PushPopIntMaintainsDataIntegrity);
    RUN_TEST_CASE(DV_QUEUE_001, PushPopStructMaintainsMemory);
    RUN_TEST_CASE(DV_QUEUE_001, PushPopCharArrayMaintainsMemory);
    RUN_TEST_CASE(DV_QUEUE_001, PushPopWrapAroundMaintainsIntegrity);
    RUN_TEST_CASE(DV_QUEUE_001, PushPopZeroBytesElementHandlesGracefully);
    RUN_TEST_CASE(DV_QUEUE_001, PushNullPointerReturnsError);
    RUN_TEST_CASE(DV_QUEUE_001, PopNullPointerReturnsError);
    RUN_TEST_CASE(DV_QUEUE_001, CopyBytesBranchCoveredViaQueueAPI);
    RUN_TEST_CASE(DV_QUEUE_001, PushPopMultipleStructsMaintainsMemory);
    RUN_TEST_CASE(DV_QUEUE_001, CopyBytesNullPointers);
}
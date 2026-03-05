#include "unity/fixture/unity_fixture.h"


/* -------------------------- */
/* Queue Initialization Tests */
/* -------------------------- */
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
/* -------------------------- */
/* Queue Push Tests */
/* -------------------------- */
TEST_GROUP_RUNNER(queue_push)
{
    RUN_TEST_CASE(queue_push, GivenEmptyQueueWhenPushOneItemThenQueueIsNotEmpty);
    RUN_TEST_CASE(queue_push, GivenFullQueueWhenPushItemThenPushFails);
    RUN_TEST_CASE(queue_push, GivenQueueWhenPushNullPointerThenReturnsError);
    RUN_TEST_CASE(queue_push, GivenQueueWhenPushMultipleItemsThenCountIncrements);
    RUN_TEST_CASE(queue_push, GivenQueueFullWhenPushFailsThenCountUnchanged);
}

/* -------------------------- */
/* Queue Pop Tests */
/* -------------------------- */
TEST_GROUP_RUNNER(queue_pop)
{
    RUN_TEST_CASE(queue_pop, GivenNonEmptyQueueWhenPopItemThenReturnsCorrectValue);
    RUN_TEST_CASE(queue_pop, GivenEmptyQueueWhenPopThenReturnsQueueEmptyStatus);
    RUN_TEST_CASE(queue_pop, GivenQueueWhenPopAllItemsThenQueueIsEmpty);
    RUN_TEST_CASE(queue_pop, GivenNullQueuePointerWhenPopThenReturnsError);
    RUN_TEST_CASE(queue_pop, GivenNullItemPointerWhenPopThenReturnsError);
    RUN_TEST_CASE(queue_pop, GivenWrapAroundQueueWhenPopThenReturnsCorrectElement);
}

/* -------------------------- */
/* Queue Peek Tests */
/* -------------------------- */
TEST_GROUP_RUNNER(queue_peek)
{
    RUN_TEST_CASE(queue_peek, GivenEmptyQueueWhenPeekThenReturnsQueueEmpty);
    RUN_TEST_CASE(queue_peek, GivenNullQueuePointerWhenPeekThenReturnsError);
    RUN_TEST_CASE(queue_peek, GivenNullItemPointerWhenPeekThenReturnsError);
    RUN_TEST_CASE(queue_peek, GivenNonEmptyQueueWhenPeekThenReturnsFirstElement);
    RUN_TEST_CASE(queue_peek, GivenPeekCalledThenQueueStateDoesNotChange);
    RUN_TEST_CASE(queue_peek, GivenMultipleItemsWhenPeekThenAlwaysReturnsOldest);
    RUN_TEST_CASE(queue_peek, GivenWrapAroundQueueWhenPeekThenReturnsCorrectElement);
}

/* -------------------------- */
/* Queue Full / Empty Tests */
/* -------------------------- */
TEST_GROUP_RUNNER(queue_state)
{
    RUN_TEST_CASE(queue_state, GivenNewQueueThenIsEmptyReturnsTrue);
    RUN_TEST_CASE(queue_state, GivenNullQueuePointerThenReturnsSafeValues);
    RUN_TEST_CASE(queue_state, GivenQueueWithCountEqualCapacityThenIsFullReturnsTrue);
    RUN_TEST_CASE(queue_state, GivenQueueWithCountZeroThenIsEmptyReturnsTrue);
    RUN_TEST_CASE(queue_state, GivenPartiallyFilledQueueThenIsEmptyAndIsFullReturnFalse);
    RUN_TEST_CASE(queue_state, GivenCountIncrementedToCapacityThenIsFullBecomesTrue);
    RUN_TEST_CASE(queue_state, GivenCountDecrementedFromCapacityThenIsFullBecomesFalse);
}

/* -------------------------- */
/* FIFO / Corner Case Tests */
/* -------------------------- */
TEST_GROUP_RUNNER(queue_core)
{
    RUN_TEST_CASE(queue_core, GivenQueueWhenPushAndPopInLoopThenBehavesAsFifo);
    RUN_TEST_CASE(queue_core, GivenQueueWhenInitWithNullPointerThenReturnsError);
    RUN_TEST_CASE(queue_core, GivenWrapAroundScenarioThenOrderIsPreserved);
    RUN_TEST_CASE(queue_core, GivenQueueFullWhenPushThenPushFailsAndCountUnchanged);
    RUN_TEST_CASE(queue_core, GivenEmptyQueueWhenPopMultipleTimesThenAlwaysReturnsQueueEmpty);
} 

/* -------------------------- */
/* DV_QUEUE_001: MISRA Deviation Tests */
/* -------------------------- */
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
    /* Additional corner-case and MISRA coverage tests */
    RUN_TEST_CASE(DV_QUEUE_001, PushPopFloatMaintainsAlignment);
    RUN_TEST_CASE(DV_QUEUE_001, MultipleQueuesOperateIndependently);
    RUN_TEST_CASE(DV_QUEUE_001, PushPopBoundaryWrapAround);
}
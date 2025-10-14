#include "unity/fixture/unity_fixture.h"

TEST_GROUP_RUNNER(queue)
{
   /* Test cases to run */

   RUN_TEST_CASE(queue, GivenEmptyQueueWhenPushOneItemThenQueueIsNotEmpty);
   RUN_TEST_CASE(queue, GivenFullQueueWhenPushItemThenPushFails);
   RUN_TEST_CASE(queue, GivenNonEmptyQueueWhenPopItemThenReturnsCorrectValue);
   RUN_TEST_CASE(queue, GivenEmptyQueueWhenPopThenReturnsQueueEmptyStatus);
   RUN_TEST_CASE(queue, GivenQueueWhenPushAndPopInLoopThenBehavesAsFifo);
   // RUN_TEST_CASE(queue, );
}

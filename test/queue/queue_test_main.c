#include "unity/fixture/unity_fixture.h"

static void run_all_tests(void);

int main(int argc, const char **argv)
{
    int test_resoult = UnityMain(argc, argv, run_all_tests);

    return test_resoult;
}

static void run_all_tests(void)
{
    RUN_TEST_GROUP(queue_init);
    RUN_TEST_GROUP(queue_push);
    RUN_TEST_GROUP(queue_pop);
    RUN_TEST_GROUP(queue_peek);
    RUN_TEST_GROUP(queue_state);
    RUN_TEST_GROUP(queue_core);
    RUN_TEST_GROUP(DV_QUEUE_001);
}
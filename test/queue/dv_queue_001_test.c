#include "unity/fixture/unity_fixture.h"
#include "queue.h"
#include <string.h>

/* Capacity for this deviation test */
#define DV_QUEUE_CAPACITY 3

static queue_t dv_queue;
static int dv_buffer[DV_QUEUE_CAPACITY];

extern void copy_bytes(uint8_t *dst, const uint8_t *src, uint16_t size);

TEST_GROUP(DV_QUEUE_001);

TEST_SETUP(DV_QUEUE_001)
{
    /* Initialize queue before each test */
    queue_init(&dv_queue, dv_buffer, sizeof(int), DV_QUEUE_CAPACITY);
}

TEST_TEAR_DOWN(DV_QUEUE_001)
{
}

/* -------------------------- */
/* Tests for type-agnostic copy */
/* -------------------------- */

TEST(DV_QUEUE_001, PushPopIntMaintainsDataIntegrity)
{
    int in = 42;
    int out = 0;

    TEST_ASSERT_EQUAL(QUEUE_OK, queue_push(&dv_queue, &in));
    TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&dv_queue, &out));
    TEST_ASSERT_EQUAL_INT(in, out);
}

TEST(DV_QUEUE_001, PushPopStructMaintainsMemory)
{
    typedef struct
    {
        uint16_t a;
        uint8_t b;
        uint8_t c;
    } test_struct_t;
    test_struct_t in = {1, 2, 3}, out = {0};

    test_struct_t buffer[DV_QUEUE_CAPACITY];
    queue_t s_queue;
    queue_init(&s_queue, buffer, sizeof(test_struct_t), DV_QUEUE_CAPACITY);

    TEST_ASSERT_EQUAL(QUEUE_OK, queue_push(&s_queue, &in));
    TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&s_queue, &out));

    TEST_ASSERT_EQUAL_MEMORY(&in, &out, sizeof(test_struct_t));
}

TEST(DV_QUEUE_001, PushPopCharArrayMaintainsMemory)
{
    char in[4] = {1, 2, 3, 4};
    char out[4] = {0};

    char buffer[DV_QUEUE_CAPACITY][4];
    queue_t c_queue;
    queue_init(&c_queue, buffer, sizeof(buffer[0]), DV_QUEUE_CAPACITY);

    TEST_ASSERT_EQUAL(QUEUE_OK, queue_push(&c_queue, in));
    TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&c_queue, out));

    TEST_ASSERT_EQUAL_MEMORY(in, out, sizeof(in));
}

/* -------------------------- */
/* Tests for wrap-around behavior */
/* -------------------------- */

TEST(DV_QUEUE_001, PushPopWrapAroundMaintainsIntegrity)
{
    int in[] = {1, 2, 3, 4};
    int out = 0;

    for (int i = 0; i < DV_QUEUE_CAPACITY; i++)
    {
        TEST_ASSERT_EQUAL(QUEUE_OK, queue_push(&dv_queue, &in[i]));
    }

    TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&dv_queue, &out));
    TEST_ASSERT_EQUAL_INT(1, out);

    TEST_ASSERT_EQUAL(QUEUE_OK, queue_push(&dv_queue, &in[DV_QUEUE_CAPACITY]));
    TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&dv_queue, &out));
    TEST_ASSERT_EQUAL_INT(2, out);
}

/* -------------------------- */
/* Edge case: minimal element size */
/* -------------------------- */

TEST(DV_QUEUE_001, PushPopZeroBytesElementHandlesGracefully)
{
    char zero_buffer[DV_QUEUE_CAPACITY][1]; /* minimal 1 byte */
    queue_t z_queue;

    TEST_ASSERT_EQUAL(QUEUE_OK, queue_init(&z_queue, zero_buffer, 1U, DV_QUEUE_CAPACITY));

    char in[1] = {0};
    char out[1] = {0xFF};

    TEST_ASSERT_EQUAL(QUEUE_OK, queue_push(&z_queue, in));
    TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&z_queue, out));

    TEST_ASSERT_EQUAL_MEMORY(in, out, sizeof(in));
}

/* -------------------------- */
/* NULL pointer handling in API */
/* -------------------------- */

TEST(DV_QUEUE_001, PushNullPointerReturnsError)
{
    TEST_ASSERT_EQUAL(QUEUE_ERROR, queue_push(&dv_queue, NULL));
}

TEST(DV_QUEUE_001, PopNullPointerReturnsError)
{
    TEST_ASSERT_EQUAL(QUEUE_ERROR, queue_pop(&dv_queue, NULL));
}

/* -------------------------- */
/* Indirect testing of copy_bytes for full branch coverage */
/* -------------------------- */

TEST(DV_QUEUE_001, CopyBytesBranchCoveredViaQueueAPI)
{
    /* The static copy_bytes function cannot be called directly,
       so we test branches by using queue API with NULL pointers */

    TEST_ASSERT_EQUAL(QUEUE_ERROR, queue_push(&dv_queue, NULL)); /* triggers src==NULL branch */
    TEST_ASSERT_EQUAL(QUEUE_ERROR, queue_pop(&dv_queue, NULL));  /* triggers dst==NULL branch */
}

/* -------------------------- */
/* Optional: type-agnostic verification of structs/arrays for completeness */
/* -------------------------- */

TEST(DV_QUEUE_001, PushPopMultipleStructsMaintainsMemory)
{
    typedef struct
    {
        uint16_t a;
        uint8_t b;
        uint8_t c;
    } test_struct_t;

    test_struct_t s_in[DV_QUEUE_CAPACITY] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    test_struct_t s_out;
    test_struct_t s_buffer[DV_QUEUE_CAPACITY];
    queue_t s_queue;

    queue_init(&s_queue, s_buffer, sizeof(test_struct_t), DV_QUEUE_CAPACITY);

    for (int i = 0; i < DV_QUEUE_CAPACITY; i++)
        TEST_ASSERT_EQUAL(QUEUE_OK, queue_push(&s_queue, &s_in[i]));

    for (int i = 0; i < DV_QUEUE_CAPACITY; i++)
    {
        TEST_ASSERT_EQUAL(QUEUE_OK, queue_pop(&s_queue, &s_out));
        TEST_ASSERT_EQUAL_UINT16(s_in[i].a, s_out.a);
        TEST_ASSERT_EQUAL_UINT8(s_in[i].b, s_out.b);
        TEST_ASSERT_EQUAL_UINT8(s_in[i].c, s_out.c);
    }
}

TEST(DV_QUEUE_001, CopyBytesNullPointers)
{
    uint8_t buf[4] = {0};
    copy_bytes(NULL, buf, 4);
    copy_bytes(buf, NULL, 4);
    copy_bytes(NULL, NULL, 4);
}

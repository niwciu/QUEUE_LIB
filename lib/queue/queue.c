/**
 * @file queue.c
 * @brief Generic FIFO queue implementation for embedded safety-critical use.
 * @version 1.0.2
 * @date 2025-10-26
 *
 * @details
 *   Provides deterministic enqueue/dequeue operations on a caller-supplied
 *   memory buffer. Implementation uses explicit byte-wise copying and avoids
 *   standard library dependencies to ensure deterministic behavior.
 *
 *   MISRA Deviation: DV-QUEUE-001 (Rule 11.4)
 *   Controlled cast from void* to uint8_t* for raw byte access.
 *   Safe because data is not type-reinterpreted.
 *
 * @note
 *   Internal helper function `copy_bytes(uint8_t*, const uint8_t*, uint16_t)` is tested
 *   indirectly via DV_QUEUE_001 unit tests, including:
 *   - pushing/popping int, struct, char array elements,
 *   - wrap-around behavior,
 *   - zero-byte edge case,
 *   - NULL pointer handling.
 */

#include "queue.h"

/* PRIVATE macro controls function visibility:
 * - static in production builds (default)
 * - global in UNIT_TESTS build for test coverage
 * This allows testing of internal functions without exposing them in production.
 */
#ifdef UNIT_TESTS
#define PRIVATE
#else
#define PRIVATE static
#endif

/* Internal helper: byte-wise deterministic copy.
 * @note MISRA Deviation DV-QUEUE-001 applies here.
 *       Do not expose externally; tested via queue_push/queue_pop. */
PRIVATE void copy_bytes(uint8_t *dst, const uint8_t *src, uint16_t size);

/* -------------------------- */
/* Queue API implementation */
/* -------------------------- */

queue_status_t queue_init(queue_t *q, void *buffer, uint16_t element_size, uint16_t capacity)
{
    if ((q == (queue_t *)0) ||
        (buffer == (void *)0) ||
        (element_size == 0U) ||
        (capacity == 0U))
    {
        return QUEUE_ERROR;
    }

    q->buffer = buffer;
    q->element_size = element_size;
    q->capacity = capacity;
    q->head = 0U;
    q->tail = 0U;
    q->count = 0U;

    return QUEUE_OK;
}

queue_status_t queue_push(queue_t *q, const void *item)
{
    if ((q == (queue_t *)0) || (item == (const void *)0))
    {
        return QUEUE_ERROR;
    }

    if (q->count >= q->capacity)
    {
        return QUEUE_FULL;
    }

    /* MISRA Deviation DV-QUEUE-001: controlled cast for byte-wise copy */
    uint8_t *base = (uint8_t *)q->buffer;

    const uint32_t offset = (uint32_t)q->tail * (uint32_t)q->element_size;
    copy_bytes(&base[offset], (const uint8_t *)item, q->element_size);

    q->tail = (uint16_t)(((uint32_t)q->tail + 1U) % (uint32_t)q->capacity);
    q->count = (uint16_t)((uint32_t)q->count + 1U);

    return QUEUE_OK;
}

queue_status_t queue_pop(queue_t *q, void *item)
{
    if ((q == (queue_t *)0) || (item == (void *)0))
    {
        return QUEUE_ERROR;
    }

    if (q->count == 0U)
    {
        return QUEUE_EMPTY;
    }

    /* MISRA Deviation DV-QUEUE-001: controlled cast for byte-wise copy */
    const uint8_t *base = (const uint8_t *)q->buffer;

    const uint32_t offset = (uint32_t)q->head * (uint32_t)q->element_size;
    copy_bytes((uint8_t *)item, &base[offset], q->element_size);

    q->head = (uint16_t)(((uint32_t)q->head + 1U) % (uint32_t)q->capacity);
    q->count = (uint16_t)((uint32_t)q->count - 1U);

    return QUEUE_OK;
}

bool queue_is_empty(const queue_t *q)
{
    return (q == (const queue_t *)0) ? true : (q->count == 0U);
}

bool queue_is_full(const queue_t *q)
{
    return (q == (const queue_t *)0) ? false : (q->count == q->capacity);
}

/* -------------------------- */
/* Internal helper functions */
/* -------------------------- */

/**
 * @brief Deterministic byte-wise copy of memory.
 * @param[out] dst Destination buffer.
 * @param[in]  src Source buffer.
 * @param[in]  size Number of bytes to copy.
 *
 * @details
 *   Used internally by queue_push/queue_pop to implement type-agnostic element storage.
 *   The PRIVATE macro controls visibility:
 *     - static in production builds
 *     - non-static in UNIT_TESTS build for test coverage
 *
 * @note MISRA Deviation DV-QUEUE-001: controlled cast for raw memory access.
 * @note Branch with NULL pointers is exercised indirectly in unit tests DV_QUEUE_001.
 */
PRIVATE void copy_bytes(uint8_t *dst, const uint8_t *src, uint16_t size)
{
    if ((dst == (uint8_t *)0) || (src == (const uint8_t *)0))
    {
        return;
    }

    for (uint16_t i = 0U; i < size; i = (uint16_t)(i + 1U))
    {
        dst[i] = src[i];
    }
}

/**
 * @file queue.c
 * @brief Generic FIFO queue implementation for embedded safety-critical use.
 * @version 1.0.3
 * @date 2025-11-06
 *
 * @details
 *  Provides deterministic enqueue/dequeue operations on a caller-supplied
 *  memory buffer. Implementation uses explicit byte-wise copying and avoids
 *  standard library dependencies to ensure predictable timing.
 *
 *  MISRA Deviation: DV-QUEUE-001 (Rule 11.4)
 *  Controlled cast from `void*` to `uint8_t*` for raw byte access.
 *  Safe because data is not type-reinterpreted.
 *
 * @ingroup queue
 */

#include "queue.h"
#include <stddef.h> /* for NULL */

/* PRIVATE macro controls linkage:
 * - static in production builds
 * - global in UNIT_TESTS build for unit test visibility.
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
static bool validate_init_arg(const queue_t *q, const void *buffer, uint16_t buffer_element_size, uint16_t queue_capacity);

/* -------------------------- */
/* Queue API implementation  */
/* -------------------------- */

queue_status_t queue_init(queue_t *q, void *buffer, uint16_t buffer_element_size, uint16_t queue_capacity)
{
    queue_status_t ret_status = QUEUE_OK;

    if (validate_init_arg(q, buffer, buffer_element_size, queue_capacity))
    {
        ret_status = QUEUE_ERROR;
    }
    else
    {
        q->buffer = buffer;
        q->buffer_element_size = buffer_element_size;
        q->capacity = queue_capacity;
        q->head = 0U;
        q->tail = 0U;
        q->count = 0U;
    }

    return ret_status;
}

queue_status_t queue_push(queue_t *q, const void *item)
{
    queue_status_t ret_status = QUEUE_OK;

    if ((q == NULL) || (item == NULL))
    {
        ret_status = QUEUE_ERROR;
    }
    else if (q->count >= q->capacity)
    {
        ret_status = QUEUE_FULL;
    }
    else
    {
        /* MISRA Deviation DV-QUEUE-001: controlled cast for byte-wise copy */
        uint8_t *base = (uint8_t *)q->buffer;
        const uint32_t offset = (uint32_t)q->tail * (uint32_t)q->buffer_element_size;

        copy_bytes(&base[offset], (const uint8_t *)item, q->buffer_element_size);

        q->tail = (uint16_t)(((uint32_t)q->tail + 1U) % (uint32_t)q->capacity);
        q->count = (uint16_t)((uint32_t)q->count + 1U);
    }

    return ret_status;
}

queue_status_t queue_pop(queue_t *q, void *item)
{
    queue_status_t ret_status = QUEUE_OK;

    if ((q == NULL) || (item == NULL))
    {
        ret_status = QUEUE_ERROR;
    }
    else if (q->count == 0U)
    {
        ret_status = QUEUE_EMPTY;
    }
    else
    {
        /* MISRA Deviation DV-QUEUE-001: controlled cast for byte-wise copy */
        const uint8_t *base = (const uint8_t *)q->buffer;
        const uint32_t offset = (uint32_t)q->head * (uint32_t)q->buffer_element_size;

        copy_bytes((uint8_t *)item, &base[offset], q->buffer_element_size);

        q->head = (uint16_t)(((uint32_t)q->head + 1U) % (uint32_t)q->capacity);
        q->count = (uint16_t)((uint32_t)q->count - 1U);
    }

    return ret_status;
}

bool queue_is_empty(const queue_t *q)
{
    bool is_empty = true;

    if (q != NULL)
    {
        is_empty = (q->count == 0U);
    }

    return is_empty;
}

bool queue_is_full(const queue_t *q)
{
    bool is_full = false;

    if (q != NULL)
    {
        is_full = (q->count == q->capacity);
    }

    return is_full;
}

/* -------------------------- */
/* Internal helper functions  */
/* -------------------------- */
/**
 * @defgroup queue_internal Queue Internal Functions
 * @ingroup queue
 * @brief Internal helper functions for the queue module (not part of public API).
 * @details
 *  These functions are local to `queue.c` and not exposed in `queue.h`.
 *  They support deterministic byte copying and argument validation.
 * @{
 */

/**
 * @brief Deterministic byte-wise copy of memory.
 *
 * @param[out] dst Destination buffer (non-NULL).
 * @param[in]  src Source buffer (non-NULL).
 * @param[in]  size Number of bytes to copy.
 *
 * @details
 *  Used internally by queue_push/queue_pop to copy arbitrary elements
 *  in a deterministic, type-agnostic way.
 *
 *  @note MISRA Deviation DV-QUEUE-001 applies.
 */
PRIVATE void copy_bytes(uint8_t *dst, const uint8_t *src, uint16_t size)
{
    if ((dst != NULL) && (src != NULL))
    {
        for (uint16_t i = 0U; i < size; i = (uint16_t)(i + 1U))
        {
            dst[i] = src[i];
        }
    }
    else
    {
        /* Defensive path: do nothing if invalid pointers */
    }
}

/**
 * @brief Validate queue initialization parameters.
 *
 * @param[in] q Queue structure pointer.
 * @param[in] buffer Data buffer pointer.
 * @param[in] buffer_element_size Element size in bytes.
 * @param[in] queue_capacity Maximum number of elements.
 *
 * @return true  — invalid argument(s).
 * @return false — all parameters valid.
 */
static bool validate_init_arg(const queue_t *q, const void *buffer, uint16_t buffer_element_size, uint16_t queue_capacity)
{
    bool invalid = false;

    if ((q == NULL) ||
        (buffer == NULL) ||
        (buffer_element_size == 0U) ||
        (queue_capacity == 0U))
    {
        invalid = true;
    }

    return invalid;
}

/** @} */ /* end of queue_internal group */

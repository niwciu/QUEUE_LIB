/**
 * @file queue.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @version 1.0.0
 * @date 2025-10-14
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "queue.h"
#include <stdio.h>

static void copy_bytes(void *restrict dst, const void *restrict src, uint16_t size);

/**
 * @brief Initializes a queue instance.
 * @param[in,out] q             Pointer to the queue instance.
 * @param[in]     buffer        Pointer to the memory buffer to be used by the queue.
 * @param[in]     element_size  Size of each element in bytes.
 * @param[in]     capacity      Maximum number of elements that can be stored.
 */
queue_status_t queue_init(queue_t *q, void *buffer, uint16_t element_size, uint16_t capacity)
{
    if ((q == NULL) || (buffer == NULL) || (element_size == 0U) || (capacity == 0U))
    {
        /* Invalid parameters – initialization aborted */
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

/**
 * @brief Pushes an element to the queue.
 * @param[in,out] q     Pointer to the queue instance.
 * @param[in]     item  Pointer to the element to be added.
 * @return Queue operation status code.
 */
queue_status_t queue_push(queue_t *q, const void *item)
{
    if ((q == NULL) || (item == NULL))
    {
        return QUEUE_ERROR;
    }

    if (q->count >= q->capacity)
    {
        return QUEUE_FULL;
    }

    uint8_t *base = (uint8_t *)q->buffer;
    uint16_t offset = (uint16_t)(q->tail * q->element_size);

    copy_bytes(&base[offset], item, q->element_size);

    q->tail = (uint16_t)((q->tail + 1U) % q->capacity);
    q->count++;

    return QUEUE_OK;
}

/**
 * @brief Pops an element from the queue.
 * @param[in,out] q     Pointer to the queue instance.
 * @param[out]    item  Pointer to the location where the popped element will be stored.
 * @return Queue operation status code.
 */
queue_status_t queue_pop(queue_t *q, void *item)
{
    if ((q == NULL) || (item == NULL))
    {
        return QUEUE_ERROR;
    }

    if (q->count == 0U)
    {
        return QUEUE_EMPTY;
    }

    uint8_t *base = (uint8_t *)q->buffer;
    uint16_t offset = (uint16_t)(q->head * q->element_size);

    copy_bytes(item, &base[offset], q->element_size);

    q->head = (uint16_t)((q->head + 1U) % q->capacity);
    q->count--;

    return QUEUE_OK;
}

/**
 * @brief Checks if the queue is empty.
 * @param[in] q  Pointer to the queue instance.
 * @return true if the queue is empty, false otherwise.
 */
bool queue_is_empty(const queue_t *q)
{
    bool result = true;

    if (q != NULL)
    {
        result = (q->count == 0U);
    }

    return result;
}

/**
 * @brief Checks if the queue is full.
 * @param[in] q  Pointer to the queue instance.
 * @return true if the queue is full, false otherwise.
 */
bool queue_is_full(const queue_t *q)
{
    bool result = false;

    if (q != NULL)
    {
        result = (q->count == q->capacity);
    }

    return result;
}

/**
 * @brief Copies one element byte by byte (used instead of memcpy).
 * @note  This deterministic implementation avoids non-deterministic behavior
 *        and standard library dependencies, ensuring compliance with MISRA-C.
 */
static void copy_bytes(void *restrict dst, const void *restrict src, uint16_t size)
{
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;

    for (uint16_t i = 0U; i < size; i++)
    {
        d[i] = s[i];
    }
}

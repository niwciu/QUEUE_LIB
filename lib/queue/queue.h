/**
 * @file queue.h
 * @author
 *      niwciu (niwciu@gmail.com)
 * @brief
 *      Header file for a MISRA-C:2012 compliant, deterministic FIFO queue implementation.
 * @version 1.0.0
 * @date 2025-10-14
 *
 * @copyright
 *      Copyright (c) 2025
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Queue operation status codes.
 */
typedef enum
{
    QUEUE_OK = 0,  /**< Operation completed successfully. */
    QUEUE_FULL,    /**< Queue is full; push operation failed. */
    QUEUE_EMPTY,   /**< Queue is empty; pop operation failed. */
    QUEUE_ERROR    /**< General error (e.g., invalid pointer or parameters). */
} queue_status_t;

/**
 * @brief Queue control structure.
 * @details
 *  The structure defines all internal state required for FIFO operation.
 *  It contains user-provided buffer pointer, element size, and management indices.
 */
typedef struct
{
    void *buffer;          /**< Pointer to the queue data buffer. */
    uint16_t element_size; /**< Size of a single element in bytes. */
    uint16_t capacity;     /**< Maximum number of elements that can be stored. */
    uint16_t head;         /**< Read index (front of the queue). */
    uint16_t tail;         /**< Write index (back of the queue). */
    uint16_t count;        /**< Current number of elements in the queue. */
} queue_t;


void queue_init(queue_t *q, void *buffer, uint16_t element_size, uint16_t capacity);
queue_status_t queue_push(queue_t *q, const void *item);
queue_status_t queue_pop(queue_t *q, void *item);
bool queue_is_empty(const queue_t *q);
bool queue_is_full(const queue_t *q);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _QUEUE_H_ */

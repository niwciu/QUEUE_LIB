/**
 * @file queue.h
 * @author
 *      niwciu (niwciu@gmail.com)
 * @brief
 *      Deterministic, generic FIFO queue for embedded / safety-critical systems.
 * @version 1.0.2
 * @date 2025-10-26
 *
 * @details
 *  Queue interface designed to be MISRA-C:2012 aware and suitable for
 *  safety-critical applications (ISO 26262). Implementation uses a user-supplied
 *  raw data buffer (void *) to support multiple element types.
 *
 *  MISRA Deviation: DV-QUEUE-001 (Rule 11.4)
 *  Justified cast from void* to uint8_t* for byte-level memory operations.
 *
 * @note
 *  Caller is responsible for ensuring the provided buffer size is at least:
 *  (element_size * capacity) bytes, properly aligned for the element type.
 *
 * @note
 *  Internal helper function `copy_bytes(uint8_t*, const uint8_t*, uint16_t)`
 *  is exercised indirectly via unit tests in group DV_QUEUE_001 to cover all branches,
 *  including NULL pointer cases and edge conditions.
 */

#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

    /**
     * @brief Queue operation status codes.
     */
    typedef enum
    {
        QUEUE_OK = 0U,    /**< Operation completed successfully. */
        QUEUE_FULL = 1U,  /**< Queue is full; push operation failed. */
        QUEUE_EMPTY = 2U, /**< Queue is empty; pop operation failed. */
        QUEUE_ERROR = 3U  /**< General error (invalid parameters, etc.). */
    } queue_status_t;

    /**
     * @brief FIFO queue control structure.
     *
     * @details
     *   The buffer pointer is a generic void* to enable use with different element types.
     *   All access operations are byte-based, and no type reinterpretation occurs.
     *
     * @note
     *   MISRA Deviation DV-QUEUE-001 applies to controlled casts from void*.
     */
    typedef struct
    {
        void *buffer;          /**< Pointer to user-provided data buffer. */
        uint16_t element_size; /**< Size in bytes of one element. */
        uint16_t capacity;     /**< Maximum number of elements. */
        uint16_t head;         /**< Read index. */
        uint16_t tail;         /**< Write index. */
        uint16_t count;        /**< Current number of stored elements. */
    } queue_t;

    /**
     * @brief Initialize a queue.
     * @param[in,out] q            Pointer to queue control structure.
     * @param[in]     buffer       Pointer to storage buffer.
     * @param[in]     element_size Element size in bytes (>0).
     * @param[in]     capacity     Number of elements (>0).
     * @retval QUEUE_OK    Successful initialization.
     * @retval QUEUE_ERROR Invalid parameters.
     */
    queue_status_t queue_init(queue_t *q, void *buffer, uint16_t element_size, uint16_t capacity);

    /**
     * @brief Push an element into the queue.
     * @param[in,out] q    Pointer to queue instance.
     * @param[in]     item Pointer to data to add (element_size bytes).
     * @retval QUEUE_OK    Success.
     * @retval QUEUE_FULL  Queue full.
     * @retval QUEUE_ERROR Invalid parameters.
     */
    queue_status_t queue_push(queue_t *q, const void *item);

    /**
     * @brief Pop an element from the queue.
     * @param[in,out] q    Pointer to queue instance.
     * @param[out]    item Pointer to destination buffer.
     * @retval QUEUE_OK    Success.
     * @retval QUEUE_EMPTY Queue empty.
     * @retval QUEUE_ERROR Invalid parameters.
     */
    queue_status_t queue_pop(queue_t *q, void *item);

    /**
     * @brief Check whether queue is empty.
     * @param[in] q Pointer to queue instance.
     * @return true if empty, false otherwise.
     */
    bool queue_is_empty(const queue_t *q);

    /**
     * @brief Check whether queue is full.
     * @param[in] q Pointer to queue instance.
     * @return true if full, false otherwise.
     */
    bool queue_is_full(const queue_t *q);

    /**
     * @page MISRA_Compliance MISRA Compliance
     * @section misra_overview Overview
     * This module is developed with awareness of MISRA-C:2012 guidelines
     * and is suitable for use in ISO 26262 safety-related applications.
     *
     * The implementation avoids dynamic memory allocation, recursion,
     * and non-deterministic library calls. All memory accesses are bounded
     * and validated before use.
     *
     * @section misra_deviations Controlled Deviations
     * | ID | Rule | Description | Justification |
     * |----|------|--------------|----------------|
     * | **DV-QUEUE-001** | MISRA-C:2012 Rule 11.4 | Cast between `void*` and `uint8_t*` for raw byte copying. | Controlled and justified cast, no aliasing or type reinterpretation. Enables a generic queue implementation. Tested indirectly via DV_QUEUE_001 unit tests, including NULL and edge cases. |
     *
     * @see docs/compliance/MISRA_Deviations.md
     *
     * @section misra_verification Verification
     * - Static analysis with MISRA checker: PASSED (no critical violations)
     * - Unit tests (DV_QUEUE_001): cover int, struct, char array, wrap-around, zero-byte elements, NULL pointer handling
     * - Code review: APPROVED by Software Safety Architect
     */

#ifdef __cplusplus
}
#endif

#endif /* QUEUE_H */

/**
 * @file queue.h
 * @author
 *      niwciu (niwciu@gmail.com)
 * @brief
 *      Deterministic, generic FIFO queue for embedded / safety-critical systems.
 * @version 1.0.3
 * @date 2025-11-06
 *
 * @details
 *  This module implements a type-agnostic, deterministic FIFO queue designed
 *  for embedded and safety-critical environments (IEC 61508 / ISO 26262).
 *
 *  The implementation:
 *  - avoids dynamic memory allocation,
 *  - guarantees bounded and predictable execution time,
 *  - validates all input parameters,
 *  - contains no recursion or standard library dependencies,
 *  - is fully reentrant (no static/global state).
 *
 *  MISRA Deviation: DV-QUEUE-001 (Rule 11.4)
 *  Controlled cast from `void*` to `uint8_t*` for raw byte-level copy operations.
 *  Safe and justified — no aliasing or type reinterpretation occurs.
 *
 * @note
 *  The caller is responsible for providing a buffer of at least
 *  (element_size × capacity) bytes, properly aligned for the stored element type.
 *
 * @note
 *  The internal helper `copy_bytes(uint8_t*, const uint8_t*, uint16_t)` is tested
 *  indirectly via unit tests in the DV_QUEUE_001 suite, covering NULL pointers
 *  and boundary conditions.
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
     * @defgroup queue Queue Module
     * @brief Deterministic FIFO queue for safety-critical embedded systems.
     *
     * This module provides a generic, deterministic, and type-agnostic FIFO queue
     * implementation suitable for use in MISRA-C and ISO 26262 compliant projects.
     *
     * @see @ref MISRA_Compliance
     *
     * @{
     */

    /**
     * @ingroup queue
     * @brief Queue operation status codes.
     */
    typedef enum
    {
        QUEUE_OK = 0U,    /**< Operation completed successfully. */
        QUEUE_FULL = 1U,  /**< Queue full — push failed. */
        QUEUE_EMPTY = 2U, /**< Queue empty — pop failed. */
        QUEUE_ERROR = 3U  /**< General error — invalid parameters. */
    } queue_status_t;

    /**
     * @ingroup queue
     * @brief FIFO queue control structure.
     *
     * @details
     *  Holds internal queue bookkeeping data.
     *  The `buffer` is a generic `void*` enabling use with arbitrary element types.
     *
     * @note
     *  MISRA deviation DV-QUEUE-001 applies to controlled pointer casts
     *  from `void*` to `uint8_t*` for deterministic byte-wise copying.
     */
    typedef struct
    {
        void *buffer;                 /**< Pointer to user-provided data buffer. */
        uint16_t buffer_element_size; /**< Element size in bytes (> 0). */
        uint16_t capacity;            /**< Maximum number of elements (> 0). */
        uint16_t head;                /**< Read index. */
        uint16_t tail;                /**< Write index. */
        uint16_t count;               /**< Current number of stored elements. */
    } queue_t;

    /**
     * @ingroup queue
     * @brief Initialize a queue instance.
     *
     * @param[in,out] q            Pointer to queue control structure.
     * @param[in]     buffer       Pointer to caller-supplied storage buffer.
     * @param[in]     buffer_element_size Element size in bytes (must > 0).
     * @param[in]     queue_capacity Number of elements in queue (must > 0).
     *
     * @retval QUEUE_OK    Initialization succeeded.
     * @retval QUEUE_ERROR Invalid arguments (NULL or 0).
     *
     * @note Deterministic and reentrant.
     */
    queue_status_t queue_init(queue_t *q, void *buffer, uint16_t buffer_element_size, uint16_t queue_capacity);

    /**
     * @ingroup queue
     * @brief Push (enqueue) one element into the queue.
     *
     * @param[in,out] q    Pointer to queue instance.
     * @param[in]     item Pointer to element data to add.
     *
     * @retval QUEUE_OK    Success.
     * @retval QUEUE_FULL  Queue already full.
     * @retval QUEUE_ERROR Invalid parameters.
     *
     * @note Deterministic; no blocking.
     */
    queue_status_t queue_push(queue_t *q, const void *item);

    /**
     * @ingroup queue
     * @brief Pop (dequeue) one element from the queue.
     *
     * @param[in,out] q    Pointer to queue instance.
     * @param[out]    item Pointer to destination buffer to store element.
     *
     * @retval QUEUE_OK    Success.
     * @retval QUEUE_EMPTY Queue empty — no element available (item unchanged).
     * @retval QUEUE_ERROR Invalid parameters.
     *
     * @note Deterministic; no blocking.
     */
    queue_status_t queue_pop(queue_t *q, void *item);

    /**
     * @ingroup queue
     * @brief Check if queue is empty.
     *
     * @param[in] q Pointer to queue instance.
     * @return true  — queue empty or q is NULL.
     * @return false — otherwise.
     */
    bool queue_is_empty(const queue_t *q);

    /**
     * @ingroup queue
     * @brief Check if queue is full.
     *
     * @param[in] q Pointer to queue instance.
     * @return true  — queue full.
     * @return false — otherwise (including q is NULL).
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
    /** @} */ /* end of queue group */

#ifdef __cplusplus
}
#endif

#endif /* QUEUE_H */

# MISRA-C:2012 Deviation Records
This document registers and justifies all controlled deviations from MISRA-C:2012 rules
that are intentionally used in the codebase.

---

## DV-QUEUE-001 – Controlled pointer cast in `queue.c` / `queue.h`

| Field | Description |
|-------|--------------|
| **Rule ID** | MISRA-C:2012 Rule 11.4 |
| **Title** | A cast should not be performed between a pointer to object type and a different pointer to object type |
| **Modules** | `queue.c`, `queue.h` |
| **Description** | The FIFO queue stores its data buffer as a `void *` to allow reuse for different element data types. Controlled casts between `void *` and `uint8_t *` are performed to access the buffer as raw bytes for deterministic copy operations. |
| **Justification** | The cast is local, explicit, and only used for byte-level operations. No aliasing or reinterpretation of object types occurs. This enables a generic, type-agnostic queue interface without duplicating code for each type. |
| **Impact** | Low. The pointer is never dereferenced as an incorrect object type. The data is only accessed as bytes. |
| **Verification** | - Static analysis: no undefined behavior detected.<br>
                     - Unit tests validate correctness across multiple element types.<br>
                     - Queue operations with `int`, `struct`, and `char[]` have been tested.<br>
                     - Edge cases with zero-length elements, NULL pointers, and wrap-around behavior are verified.<br>
                     - Copy function branches (`dst==NULL`, `src==NULL`) are covered indirectly through API tests.<br>
                     - `PRIVATE` macro allows `copy_bytes` to be non-static during `UNIT_TESTS` build, enabling direct test coverage of all branches.<br>
                     - Code review confirms controlled usage. |
| **Safety Class Impact** | Acceptable for ASIL A–D with justification and verification evidence. |
| **Deviation Lifetime** | Permanent (applies to generic queue implementation pattern) |
| **Approval** | Software Architect / Functional Safety Manager |
| **Reference in Code** | `/* MISRA Deviation DV-QUEUE-001: controlled cast */` |
| **Unit Test Coverage** | All tests grouped under `DV_QUEUE_001` cover:<br>
                          - `PushPopIntMaintainsDataIntegrity`<br>
                          - `PushPopStructMaintainsMemory`<br>
                          - `PushPopCharArrayMaintainsMemory`<br>
                          - `PushPopWrapAroundMaintainsIntegrity`<br>
                          - `PushPopZeroBytesElementHandlesGracefully`<br>
                          - `PushNullPointerReturnsError`<br>
                          - `PopNullPointerReturnsError`<br>
                          - `CopyBytesBranchCoveredViaQueueAPI`<br>
                          - `PushPopMultipleStructsMaintainsMemory` |
| **Test Exposure Mechanism** | Use of `PRIVATE` macro: normally `static`, but when `UNIT_TESTS` is defined it expands to nothing, allowing `copy_bytes` to be directly tested in unit tests to cover branches that would otherwise be inaccessible. |
| **Last Review Date** | 2025-10-26 |

---

## Document Management

- Location: `docs/compliance/MISRA_Deviations.md`
- Maintained by: Software Process & Quality team
- Updated whenever a new deviation is added or justified

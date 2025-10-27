# MISRA-C:2012 Deviation Record

**Project:** QUEUE_LIB  
**Document ID:** MISRA_Deviations.md  
**Version:** 1.0  
**Author:** niwciu  
**Date:** 2025-10-26  
**Status:** Approved  
**Maintained by:** Author (self-review)  
**Location:** `docs/compliance/MISRA_Deviations.md`

This document registers and justifies all controlled deviations from **MISRA-C:2012** rules that are intentionally applied within the QUEUE_LIB codebase.  
Each deviation is verified through static analysis, self-review, and automated unit testing.

---

## DV-QUEUE-001 – Controlled Pointer Cast in `queue.c` / `queue.h`

| **Field** | **Description** |
|------------|----------------|
| **Rule ID** | MISRA-C:2012 Rule 11.4 |
| **Rule Title** | *A cast should not be performed between a pointer to object type and a different pointer to object type* |
| **Modules** | `queue.c`, `queue.h` |
| **Description** | The FIFO queue stores its internal data buffer as a `void *` to allow reuse with different element types. Controlled casts between `void *` and `uint8_t *` are performed to access the buffer as raw bytes for deterministic copy operations inside the `copy_bytes()` helper function. |
| **Justification** | The cast is explicit, local, and limited to byte-level access. It does not result in aliasing or reinterpretation of object representations. This approach enables a generic, type-independent queue implementation without code duplication. |
| **Mitigation / Control Measures** | 1. Casts are limited to the `copy_bytes()` function and never used elsewhere.<br>2. The pointer is never dereferenced as an incorrect type.<br>3. The implementation was reviewed manually and verified with `cppcheck` and GCC warnings enabled.<br>4. All queue operations are validated through automated unit tests. |
| **Impact** | *Low* – operation is deterministic, no undefined behavior observed, and only local to a single function. |
| **Verification Activities** | - **Static analysis:** `cppcheck` (open-source, full rule set) and `gcc` with `-Wall -Wextra -pedantic` show no issues related to pointer use.<br>- **Self-review:** author manually confirmed correctness and safety of pointer handling.<br>- **Unit testing:** implemented with *Unity* framework; executed locally and in GitHub Actions CI.<br>- **Coverage evidence:** GCOVR HTML report (published via GitHub Pages) confirms 100% statement and high branch coverage of queue operations.<br>- **Edge cases tested:** zero-length elements, NULL pointers, wrap-around, multiple struct types. |
| **Test Reference (Tag)** | `DV_QUEUE_001` |
| **Unit Tests Covering Deviation** | - `PushPopIntMaintainsDataIntegrity`<br>- `PushPopStructMaintainsMemory`<br>- `PushPopCharArrayMaintainsMemory`<br>- `PushPopWrapAroundMaintainsIntegrity`<br>- `PushPopZeroBytesElementHandlesGracefully`<br>- `PushNullPointerReturnsError`<br>- `PopNullPointerReturnsError`<br>- `CopyBytesBranchCoveredViaQueueAPI`<br>- `PushPopMultipleStructsMaintainsMemory` |
| **Test Exposure Mechanism** | The `PRIVATE` macro allows normally-`static` internal functions (e.g. `copy_bytes`) to be visible when `UNIT_TESTS` is defined, enabling direct coverage of otherwise inaccessible branches. |
| **Safety Class Impact** | Acceptable for general embedded and deterministic systems; deviation justified and verified through testing and analysis. |
| **Deviation Lifetime** | *Permanent* – applies to the generic queue implementation pattern. |
| **Approval** | Self-approved (author review, 2025-10-26) |
| **Reference in Code** | `/* MISRA Deviation DV-QUEUE-001: controlled cast */` |

---

## Verification & Coverage Summary

| **Verification Area** | **Tool / Method** | **Result / Evidence** | **Publication** |
|------------------------|------------------|------------------------|-----------------|
| **Static analysis** | `cppcheck` (open-source) + `gcc -Wall -Wextra -pedantic` | No issues related to pointer usage, overflow, or uninitialized data. | Local / CI logs |
| **Formatting compliance** | `clang-format` (project `.clang-format` rules) | Codebase consistently formatted; aids readability and MISRA §2.1 compliance. | Repo root |
| **Cyclomatic complexity** | `lizard` | All functions < 10 (low complexity); HTML report generated. | [Code Complexity Report](https://niwciu.github.io/QUEUE_LIB/reports/code_complexity_report.html) |
| **Unit testing** | *Unity* framework | All test cases passed locally and in CI. | [GCOVR Coverage Report](https://niwciu.github.io/QUEUE_LIB/reports/gcovr.html) |
| **Code coverage** | `gcov` + `gcovr` | 100% statement, 95% branch coverage on `queue.c`. | [Coverage HTML Report](https://niwciu.github.io/QUEUE_LIB/reports/gcovr.html) |
| **Continuous integration** | GitHub Actions | Automated build, test, and deployment pipelines successful. | [CI Status](https://github.com/niwciu/QUEUE_LIB/actions) |
| **Manual review** | Self-review by author | Verified pointer safety, MISRA compliance, and test completeness. | Documented here |

---

## Document Management

- Updates are performed whenever a new deviation is introduced or justification is revised.  
- Document maintained under version control (`git`) and included in CI artifact deployment.  
- The current version of this document is published at:  
  [`https://niwciu.github.io/QUEUE_LIB/docs/compliance/MISRA_Deviations.md`](https://niwciu.github.io/QUEUE_LIB/docs/compliance/MISRA_Deviations.md)

# 📦 QUEUE_LIB – Changelog

All notable changes to this project will be documented in this file.  
This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.0.1] – 2025-11-06
**Type:** Maintenance / MISRA Compliance / Documentation Update  
**Previous Release:** [1.0.0]

### 🧩 Added
- **Static helper function:** `validate_init_arg()` for input validation during initialization.  
- **Doxygen groups:** `@defgroup queue` (public API) and `@defgroup queue_internal` (internal static functions).  
- Extended MISRA compliance documentation (DV-QUEUE-001 justification).  
- Added explicit grouping and cross-linking for MkDocs + Doxygen integration.  

### 🔄 Changed
- Changed linkage of `validate_init_arg()` from `PRIVATE` to `static` (MISRA Rule 8.8).  
- Replaced literal pointer constants `(type*)0` with `<stddef.h>` `NULL` (MISRA Rule 21.6).  
- Renamed `element_size` → `buffer_element_size` for consistent naming and readability.  
- Added explicit boolean handling in `queue_is_empty()` and `queue_is_full()`.  
- Improved `@file`, `@details`, and `@ingroup` Doxygen documentation blocks.  
- Unified and clarified MISRA Deviation Table formatting and references.  

### 🧱 Fixed
- Ensured proper internal linkage (no unintended external visibility of internal helpers).  
- Defensive handling in `copy_bytes()` for null-pointer safety.  

### 📋 Notes
- **No API or functional behavior changes** from `v1.0.0`.  
- **Full backward compatibility** preserved.  
- **Static analysis:** PASSED (cppcheck, gcc -Wall -Wextra -pedantic).  
- **Unit tests:** All DV_QUEUE_001 tests passed.  
- Ready for **MISRA-C:2012 and ISO 26262** compliant integration.

---

## [1.0.0] – 2025-10-26
**Type:** Initial Release  

### 🧩 Features
- Deterministic, static FIFO queue (no dynamic allocation).  
- Simple API:
  - `queue_init`
  - `queue_push`
  - `queue_pop`
  - `queue_is_empty`
  - `queue_is_full`
- Type-agnostic design — supports any data type.  
- Safe and MISRA-C:2012–aware implementation.  
- Includes example usage and Unity-based unit tests.  
- Licensed under **MIT License**.  

### 📘 Summary
First public release of **QUEUE_LIB**, a deterministic FIFO queue implementation for embedded and safety-critical applications.  
Designed for use in **ISO 26262 / IEC 61508**–compliant software modules.  

---

### 🧭 Version History

| Version | Date       | Type / Focus                         | Status        |
|----------|------------|--------------------------------------|----------------|
| 1.0.0    | 2025-10-26 | Initial Release                      | ✅ Stable |
| 1.0.1    | 2025-11-06 | MISRA Compliance & Documentation Fix | ✅ Stable |

---

© 2025 [niwciu](mailto:niwciu@gmail.com)  
Licensed under the [MIT License](LICENSE).

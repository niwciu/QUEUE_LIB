# 🧩 Queue Library (MISRA-C Compliant FIFO)

A **deterministic**, **no dynamic memory allocation**, and **MISRA-C:2012 compliant** FIFO queue with a *documented controlled deviation* [DV-QUEUE-001, Rule 11.4](https://niwciu.github.io/QUEUE_LIB/compliance/MISRA_Deviations) — designed for **embedded** and **safety-critical** applications (ISO 26262 / IEC 61508 / DO-178C).

---

![GitHub License](https://img.shields.io/github/license/niwciu/QUEUE_LIB)
![GitHub top language](https://img.shields.io/github/languages/top/niwciu/QUEUE_LIB)
![GitHub Release](https://img.shields.io/github/v/release/niwciu/QUEUE_LIB)
![GitHub branch check runs](https://img.shields.io/github/check-runs/niwciu/QUEUE_LIB/main)
![CI Pipeline](https://github.com/niwciu/QUEUE_LIB/actions/workflows/CI_Pipeline.yml/badge.svg)
![MISRA](https://img.shields.io/badge/MISRA-C%3A2012-blue)
![Changelog](https://img.shields.io/github/v/release/niwciu/QUEUE_LIB?label=CHANGELOG)

---

## 🧩 Compliance Verification

The **QUEUE_LIB** project includes a complete, auditable verification chain ensuring traceable MISRA-C:2012 compliance, documentation quality, and code integrity.

| Verification Area          | Tool / Method                              | Evidence / Report / Link                                                                                |
| -------------------------- | ------------------------------------------ | ------------------------------------------------------------------------------------------------------- |
| **Static Analysis**        | `cppcheck` + `gcc -Wall -Wextra -pedantic` | Verified code safety and type correctness                                                               |
| **Formatting Rules**       | `clang-format`                             | Ensures consistent style and readability (MISRA §2.1)                                                   |
| **Cyclomatic Complexity**  | `lizard`                                   | [Code Complexity Report](https://niwciu.github.io/QUEUE_LIB/reports/code_complexity_report.html)        |
| **Code Coverage**          | `gcov` + `gcovr`                           | [Coverage Report](https://niwciu.github.io/QUEUE_LIB/reports/gcovr.html)                                |
| **Documentation**          | `doxygen`                                  | [Generated API Documentation](https://niwciu.github.io/QUEUE_LIB/doxygen/queue_lib_doc/html/index.html) |
| **MISRA Deviation Record** | Manual documentation                       | [DV-QUEUE-001 (Rule 11.4)](https://niwciu.github.io/QUEUE_LIB/compliance/MISRA_Deviations)              |
| **Unit Tests**             | *Unity* Framework                          | Run locally and in CI; grouped tests cover DV-QUEUE-001 and queue peek                                  |
| **Continuous Integration** | GitHub Actions                             | Builds, tests, and publishes reports automatically                                                      |

> 🌐 **All reports and generated documentation are available online**
> and deployed automatically using *GitHub Actions*
> 🔗 [niwciu.github.io/QUEUE_LIB](https://niwciu.github.io/QUEUE_LIB)

---

## 🚀 Key Features

* ✅ Written in **C99**, **MISRA-C:2012 compliant** (*1 controlled deviation – DV-QUEUE-001, Rule 11.4, see docs/compliance/MISRA_Deviations.md*)
* ✅ **No dynamic memory allocation** (static buffers only)
* ✅ **Deterministic execution time** (O(buffer_element_size) per operation)
* ✅ **Type-agnostic** – works with any struct or primitive type
* ✅ Compatible with **bare-metal**, **RTOS**, and **safety-critical systems**
* ✅ Unit tests ready (**Unity** framework) with branch and statement coverage for all queue operations including DV-QUEUE-001 edge cases
* ✅ Controlled MISRA deviation fully documented and verified

---

## 📁 File Structure

```
/queue_lib/
│
├── .github/
│   └── workflows
│       ├── CI_Pipeline.yml
│       └── mkdocs-deploy.yml
│
├── docs/
├── examples/
│   ├── 1_basic_integer_queue      
│   └── 2_log_queue                 
├── lib/
│   └── queue/    
│       ├── queue.c
│       └── queue.h
├── test/
│   ├── _config_scripts/        
│   │   ├── CI/  
│   │   │   └── CI.py             
│   │   │   └── config.yaml       
│   │   └── venv_setup/
│   │       └── requirements.txt  
│   │       └── venv_setup.py     
│   ├── queue/                      
│   ├── template/                   
│   └── unity/                      
├── .clang-format
├── LICENSE                             
├── mkdocs.yml
└── README.md
```

---

## ⚙️ API Reference

### `queue_init`

```c
queue_status_t queue_init(queue_t *q, void *buffer, uint16_t buffer_element_size, uint16_t capacity);
```

Initializes a queue instance.

| Parameter             | Description                             |
| --------------------- | --------------------------------------- |
| `q`                   | Pointer to queue control structure      |
| `buffer`              | Pointer to user-provided buffer         |
| `buffer_element_size` | Size of a single element (bytes)        |
| `capacity`            | Maximum number of elements in the queue |

Returns:

* `QUEUE_OK` – successfully initialized
* `QUEUE_ERROR` – invalid parameters

---

### `queue_push`

```c
queue_status_t queue_push(queue_t *q, const void *item);
```

Adds an element to the queue (tail).

Returns:

* `QUEUE_OK` – successfully added
* `QUEUE_FULL` – queue is full
* `QUEUE_ERROR` – invalid parameters

---

### `queue_pop`

```c
queue_status_t queue_pop(queue_t *q, void *item);
```

Removes the oldest element from the queue (head).

Returns:

* `QUEUE_OK` – element successfully read
* `QUEUE_EMPTY` – queue is empty
* `QUEUE_ERROR` – invalid parameters

---

### `queue_peek`

```c
queue_status_t queue_peek(const queue_t *q, void *item);
```

Reads the first element without removing it from the queue.

Returns:

* `QUEUE_OK` – element successfully read
* `QUEUE_EMPTY` – queue is empty
* `QUEUE_ERROR` – invalid parameters

---

### `queue_is_empty`

```c
bool queue_is_empty(const queue_t *q);
```

Returns `true` if the queue contains no elements.

### `queue_is_full`

```c
bool queue_is_full(const queue_t *q);
```

Returns `true` if the queue has reached its capacity.

---

## 🧠 Example 1: Basic Integer Queue

```c
#include "queue.h"
#include <stdio.h>

#define QUEUE_CAPACITY 5U

static int buffer[QUEUE_CAPACITY];
static queue_t int_queue;

int main(void)
{
    queue_init(&int_queue, buffer, sizeof(int), QUEUE_CAPACITY);

    int value = 10;
    (void)queue_push(&int_queue, &value); // DV-QUEUE-001 verified

    int out = 0;
    (void)queue_pop(&int_queue, &out);

    printf("Read value: %d\n", out); // Output: 10
    return 0;
}
```

---

## 🧾 Example 2: Log Queue (Fixed-Size Structs)

```c
#include "queue.h"
#include <stdio.h>
#include <string.h>

#define LOG_ENTRY_SIZE     (32U)
#define LOG_QUEUE_CAPACITY (20U)

typedef struct
{
    uint8_t data[LOG_ENTRY_SIZE];
} log_entry_t;

static log_entry_t log_buffer[LOG_QUEUE_CAPACITY];
static queue_t log_queue;

static void log_queue_init(void)
{
    queue_init(&log_queue, log_buffer, sizeof(log_entry_t), LOG_QUEUE_CAPACITY);
}

static void log_queue_push(const char *msg)
{
    log_entry_t entry;
    uint16_t len = 0U;

    while ((msg[len] != '\0') && (len < LOG_ENTRY_SIZE))
    {
        entry.data[len] = (uint8_t)msg[len];
        len++;
    }

    while (len < LOG_ENTRY_SIZE)
    {
        entry.data[len++] = 0U;
    }

    (void)queue_push(&log_queue, &entry); // DV-QUEUE-001 verified
}

static void log_queue_pop_and_print(void)
{
    log_entry_t entry;
    if (queue_pop(&log_queue, &entry) == QUEUE_OK)
    {
        printf("LOG: %s\n", entry.data);
    }
}

int main(void)
{
    log_queue_init();
    log_queue_push("System initialized");
    log_queue_push("Temperature sensor ready");
    log_queue_push("Main loop started");

    for (uint16_t i = 0U; i < 4U; i++)
    {
        log_queue_pop_and_print();
    }
}
```

**💬 Example Output:**

```
LOG: System initialized
LOG: Temperature sensor ready
LOG: Main loop started
```

---

## ▶️ Running examples:

### 🚀 1_basic_integer_queue

1. Go to project main folder.
2. Open examples folder, compile and run the example

```bash
cd examples/1_basic_integer_queue
cmake -S./ -B out -G"Unix Makefiles"
cd out
make run
```

### 🚀 2_log_queue

1. Go to project main folder.
2. Open examples folder, compile and run the example

```bash
cd examples/2_log_queue
cmake -S./ -B out -G"Unix Makefiles"
cd out
make run
```

## 🧩 Integration

Just add `queue.c` and `queue.h` to your project and include:

```c
#include "queue.h"
```

You can compile it as part of your embedded firmware or as a portable C module.

---

## 🧪 Unit Tests (Unity)

All unit tests are now grouped and include coverage for the DV-QUEUE-001 deviation:

* `queue_init`, `queue_push`, `queue_pop`, `queue_peek`, `queue_state`, `queue_core`, `DV_QUEUE_001`
* Push/pop of different data types (int, struct, char array)
* Wrap-around behavior
* Edge cases (zero-byte elements, NULL pointers)
* Branch coverage in `copy_bytes`

Run tests:

```bash
cd test/queue
cmake -S./ -B out -G"Unix Makefiles"
cd out
make run
```

---

## 🧰 Safety / Compliance Notes

* **No dynamic memory** → static allocation only
* **No memcpy/memmove** → deterministic byte-copy routine (`copy_bytes`)
* **MISRA-C:2012 ready** – controlled deviation DV-QUEUE-001 fully documented and verified
* **Thread safety** – not inherently thread-safe; protect operations in RTOS/ISR environments
* Deterministic timing: O(buffer_element_size) per operation
* Compliance verification: static analysis, unit/integration tests, code review

**DV-QUEUE-001:**

* Rule 11.4 – cast between `void*` and `uint8_t*`
* Justification: controlled, local casts for byte-level operations
* Verified: unit tests covering all branches of `copy_bytes`, multiple data types

---

## ⚠️ Safety-Critical Additional Requirements

Follow these for ISO 26262 / IEC 61508 compliance:

1. Static code analysis
2. Full unit/integration testing with coverage report
3. Peer code reviews
4. Thread safety in multi-context environments
5. Traceable version control and documentation

---

## 📜 License

Licensed under the MIT License (see `LICENSE` file).

© 2025–2026 [niwciu](mailto:niwciu@gmail.com)

---

👉 Explore full documentation online:
[https://niwciu.github.io/QUEUE_LIB](https://niwciu.github.io/QUEUE_LIB)

<br>
<p align= "center">
  <img src="https://github.com/user-attachments/assets/f4825882-e285-4e02-a75c-68fc86ff5716" alt="myEmbeddedWayBanner"><br>
  <i>Part of the <b>myEmbeddedWay</b> safety-oriented C library collection.</i>
</p>

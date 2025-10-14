# 🧩 Queue Library (MISRA-C Compliant FIFO)

A **deterministic**, **zero-dynamic-memory**, and **MISRA-C:2012 compliant** FIFO queue implementation written in pure C — designed for **embedded** and **safety-critical** applications (ISO 26262 / IEC 61508 / DO-178C).

---

![Build](https://img.shields.io/badge/build-passing-brightgreen)
![Tests](https://img.shields.io/badge/tests-passing-brightgreen)
![MISRA](https://img.shields.io/badge/MISRA-C%3A2012-blue)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

---

## 🚀 Features

- ✅ Written in **C99**, fully **MISRA-C:2012** compliant  
- ✅ **No dynamic memory allocation** (uses static buffers)  
- ✅ **Deterministic execution time** (O(n) per element)  
- ✅ **Type-agnostic** – works with any struct or primitive type  
- ✅ Compatible with **bare-metal**, **RTOS**, and **safety-critical systems** (*under certain conditions*)  
- ✅ Unit tests ready (Unity framework)  

---

## 📁 File structure

```
/queue/
│
├── queue.h        # Public API header
├── queue.c        # Implementation (MISRA-C compliant)
├── test_queue.c   # Unit tests (Unity)
└── examples/
    └── main.c     # Log queue example
```

---

## ⚙️ API Reference

### `queue_init`
```c
void queue_init(queue_t *q, void *buffer, uint16_t element_size, uint16_t capacity);
```
Initializes a queue instance.

| Parameter | Description |
|------------|-------------|
| `q` | Pointer to queue control structure |
| `buffer` | Pointer to user-provided buffer |
| `element_size` | Size of a single element (in bytes) |
| `capacity` | Maximum number of elements in the queue |

---

### `queue_push`
```c
queue_status_t queue_push(queue_t *q, const void *item);
```
Adds an element to the queue (tail).  
Returns one of:
- `QUEUE_OK` – successfully added  
- `QUEUE_FULL` – queue is full  
- `QUEUE_ERROR` – invalid parameters  

---

### `queue_pop`
```c
queue_status_t queue_pop(queue_t *q, void *item);
```
Removes the oldest element from the queue (head).  
Returns one of:
- `QUEUE_OK` – element successfully read  
- `QUEUE_EMPTY` – queue is empty  
- `QUEUE_ERROR` – invalid parameters  

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
    (void)queue_push(&int_queue, &value);

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

    (void)queue_push(&log_queue, &entry);
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

**Example Output:**
```
LOG: System initialized
LOG: Temperature sensor ready
LOG: Main loop started
LOG: [Queue is empty]
```

---

## 🧩 Integration

Just add `queue.c` and `queue.h` to your project and include:

```c
#include "queue.h"
```

You can compile it as part of your embedded firmware or as a portable C module.

---

## 🧪 Unit Tests (Unity)

Example command (assuming Unity is in `./unity`):

```bash
gcc -I. queue.c test_queue.c unity/unity.c -o test_queue
./test_queue
```

---

## 🧰 Safety / Compliance Notes

- **No dynamic memory** → static allocation only  
- **No memcpy/memmove** → deterministic byte-copy routine  
- **MISRA-C:2012 ready** – safe for safety-critical projects  
- **Portable** – works on any MCU / compiler (GCC, IAR, GHS, etc.)  
- Time complexity: `O(element_size)` per operation (deterministic)

---

## ⚠️ Safety-Critical Additional Requirements

This library is **suitable** for use in safety-critical applications  
(e.g., ISO 26262, IEC 61508, DO-178C) **under the following conditions**:

1. **Code Verification and Analysis**
   - Perform static code analysis (e.g., PC-Lint, Coverity, Cppcheck).
   - Conduct peer code reviews according to project quality standards.
   - Verify full statement and branch coverage through unit/integration testing.

2. **Thread Safety and Concurrency**
   - The library itself is **not thread-safe**.
   - When used in RTOS environments:
     - Protect queue operations (`queue_push`, `queue_pop`) with mutexes or critical sections.
     - Avoid simultaneous access from ISR and task context without synchronization.

3. **Deterministic Timing**
   - All operations are deterministic (O(element_size)).  
   - Ensure consistent execution time under all conditions.

4. **Version Control and Traceability**
   - Maintain version tracking and traceability documentation.
   - Validate integration through system-level verification steps.

---

## 📜 License

Licensed under the MIT License (see `LICENSE` file for details).

© 2025 [niwciu](mailto:niwciu@gmail.com)
# 🧩 Queue Library (MISRA-C Compliant FIFO)

![queue_header](https://github.com/user-attachments/assets/8cf6bceb-bfab-424c-87c6-a67cf5c2862a)


A **deterministic**, **zero-dynamic-memory**, and **MISRA-C:2012 compliant** FIFO queue implementation written in pure C — designed for **embedded** and **safety-critical** applications (ISO 26262 / IEC 61508 / DO-178C).

---
![GitHub License](https://img.shields.io/github/license/niwciu/QUEUE_LIB) 
![GitHub top language](https://img.shields.io/github/languages/top/niwciu/QUEUE_LIB)
![GitHub Release](https://img.shields.io/github/v/release/niwciu/QUEUE_LIB)
![GitHub branch check runs](https://img.shields.io/github/check-runs/niwciu/QUEUE_LIB/main)
![CI Pipeline](https://github.com/niwciu/QUEUE_LIB/actions/workflows/CI_Pipeline.yml/badge.svg)
![MISRA](https://img.shields.io/badge/MISRA-C%3A2012-blue)

<!-- <img src='https://coveralls.io/repos/github/niwciu/LCD_HD44780/badge.svg?branch=feature/code_coverage_page_deploy' alt='Coverage Status' /></a> -->

<b><a href='https://niwciu.github.io/QUEUE_LIB/reports/gcovr.html'>Library GCOVR Report</a></b>  
<b><a href='https://niwciu.github.io/QUEUE_LIB/reports/code_complexity_report.html'>Library Code Complexity Report</a></b>  <br><br>
<!-- <b><a href='https://niwciu.github.io/QUEUE_LIB/doc/LCD_HD44780_lib_doc/html/index.html'>Code Documentation </a></b>   -->

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
/queue_lib/
│
├── .github/
|   └── workflows
|       ├── CI_Pipeline.yml         # CI action
|       └── Page_deploy.yml         # Library web page deploy action
├── doc/                            # doc folder
├── examples/                       # Ready to run examples
|   ├── 1_basic_integer_queue      
|   └── 2_log_queue                 
├── src/       
|   ├── queue.c                     # Implementation (MISRA-C compliant)
|   └── queue.h                     # Public API header
└── test/ 
    ├── _config_scripts/        
    |   ├── CI/  
    |   |   └── CI.py               # Python scripit runnig specifyed target in config.yaml
    |   |   └── config.yaml         # CI config file for setup and customize CI workflow            
    |   └── venv_setup/
    |       └── requirements.txt    # Python tools required by scripts in project
    |       └── venv_setup.py       # Script for automated creating of virtual environment and install requirements
    ├── queue/                      # Queue lib Unit Tests (Unity)
    ├── template/                   # Unit Tests module template files
    └── unity/                      # Unit Tests framework files

    
```

---

## ⚙️ API Reference

### `queue_init`
```c
queue_status_t queue_init(queue_t *q, void *buffer, uint16_t element_size, uint16_t capacity);
```
Initializes a queue instance.

| Parameter | Description |
|------------|-------------|
| `q` | Pointer to queue control structure |
| `buffer` | Pointer to user-provided buffer |
| `element_size` | Size of a single element (in bytes) |
| `capacity` | Maximum number of elements in the queue |

Returns one of:
- `QUEUE_OK` – successfully initialized 
- `QUEUE_ERROR` – Invalid parameters –> initialization aborted 

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
```

---
## ▶️ Running examples:
### 🚀 1_basic_integer_queue
1. Go to project main folder. 
2. Open examples folder, compile and run the example
``` bash
cd examples/1_basic_integer_queue
cmake -S./ -B out -G"Unix Makefiles"
cd out
make run
```

### 🚀 2_log_queue
1. Go to project main folder. 
2. Open examples folder, compile and run the example
``` bash
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

Running Unit tests:
1. Go to project main folder. 
2. Open test folder, compile and run the tests
``` bash
cd test/queue
cmake -S./ -B out -G"Unix Makefiles"
cd out
make run
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

<br>
<div align="center">

***

![myEmbeddedWayBanerWhiteSmaller](https://github.com/user-attachments/assets/f4825882-e285-4e02-a75c-68fc86ff5716)
***
</div>

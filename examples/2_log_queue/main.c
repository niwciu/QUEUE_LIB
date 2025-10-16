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
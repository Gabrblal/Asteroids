#ifndef INTERVAL_THREAD_H
#define INTERVAL_THREAD_H

#include <stdbool.h>
#include <stdint.h>

typedef struct IntervalThread IntervalThread;

/**
 * @brief Create a new interval thread, that calls the provided function
 * every interval.
 * 
 * If the function takes longer than the interval, then it will be called again
 * immediately after it has returned.
 * 
 * @param interval The number of milliseconds between each function call.
 * @param func The function to call every interval.
 * @param name An optional name for the thread.
 * 
 * @returns A pointer to the interval thread.
 */
IntervalThread *interval_thread_create(
    void(*func)(void *data),
    void *data,
    uint32_t interval,
    const char *name
);

/**
 * @brief Waits for the current function execution to return before destroying
 * the interval thread and deallocating it's memory.
 * 
 * @param interval_thread The interval thread to destroy.
 */
void interval_thread_destroy(IntervalThread *thread);

#endif // INTERVAL_THREAD_H

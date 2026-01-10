#include "pico.h"
#include "rp2040_scheduler_logic.h"
#include "pico/platform.h"

// RP2040 has two cores
#define CORECOUNT 2
#define CORE0 0

#define MAX_THREADS 8
#define NO_THREAD (-1)

// scheduler
// every non running thread is identified by its psp
// a running thread has the core number in it
// a dead thread has NO_THREAD in it (-1)
// only THREAD STATES: READY OR RUNNING
stack_pointer_t thread_table[MAX_THREADS];

// definitions for the idle threads
stack_pointer_t idle_threads[CORECOUNT];

/// @brief initialized at scheduler start with all have no thread in it
/// @param  none
void sl_init(void)
{
    for (int i = 0; i < MAX_THREADS; i++)
    {
        thread_table[i].w = NO_THREAD;
    }
}

void sl_first(int core_number, stack_pointer_t psp)
{
    idle_threads[core_number] = psp;
}

void sl_new(stack_pointer_t psp)
{
    int index = 0;
    while (thread_table[index].w != NO_THREAD && index < MAX_THREADS)
    {
        index++;
        if (index >= MAX_THREADS)
            panic("too much threads");
    }
    thread_table[index] = psp;
}

stack_pointer_t sl_next(int core, stack_pointer_t psp)
{
    if (thread_table[1].w != core)
    {
        psp = thread_table[0];
        thread_table[0].w = core;
    }
    else
    {
        if (thread_table[0].w == core)
        {
            thread_table[0] = psp;
            psp = thread_table[1];
            thread_table[1].w = core;
        }
        else
        {
            thread_table[1] = psp;
            psp = thread_table[0];
            thread_table[0].w = core;
        }
    }
}

void sl_end(int core_number)
{
    
}

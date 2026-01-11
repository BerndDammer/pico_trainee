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

struct
{
    // definitions for the idle threads
    stack_pointer_t idle_threads[CORECOUNT];
    uint32_t index[CORECOUNT]; // index to table or -1 when in idle
    stack_pointer_t thread_table[MAX_THREADS];
} scheduler_data;

/// @brief initialized at scheduler start with all have no thread in it
/// @param  none
void sl_init(void)
{
    for (int i = 0; i < MAX_THREADS; i++)
    {
        scheduler_data.thread_table[i].w = NO_THREAD;
    }
}

void sl_first(int core_number, stack_pointer_t psp)
{
    scheduler_data.idle_threads[core_number] = psp;
    scheduler_data.index[core_number] = -1;
}

void sl_new(stack_pointer_t psp)
{
    int search_count = 8;
    bool searching = true;
    int search_index = 0;
    while (searching)
    {
        if (scheduler_data.thread_table[search_index].w != NO_THREAD)
        {
            scheduler_data.thread_table[search_index] = psp;
            return;
        }
        search_count--;
        search_index++;
        searching = search_count > 0;
    }
    panic("too much threads");
}

stack_pointer_t sl_next(int core, stack_pointer_t psp)
{

    if (scheduler_data.index[core] == -1)
    {
        int search_count = 8;
        bool searching = true;
        int search_index = 0;
        // try leaving idle
        while (searching)
        {
            if (scheduler_data.thread_table[search_index].signed_w > CORECOUNT)
            {
                scheduler_data.idle_threads[core] = psp;
                psp = scheduler_data.thread_table[search_index];
                scheduler_data.thread_table[search_index].w = core;
                scheduler_data.index[core] = search_index;
                searching = false;
                // TODO only one return
                return psp;
            }
            search_count--;
            search_index++;
            searching = search_count > 0;
        }
        // nothing found; stay idle
        return psp;
    }
    else
    {
        int search_index = scheduler_data.index[core];
        int search_count = 8;
        bool searching = true;
        // first put actual back in list
        scheduler_data.thread_table[search_index] = psp;

        // search for ready thread
        // maybe same again
        while (searching)
        {
            // round robin logic
            // first increment seach index
            if(++search_index >= MAX_THREADS) search_index = 0;
            if (scheduler_data.thread_table[search_index].signed_w > CORECOUNT)
            {
                psp = scheduler_data.thread_table[search_index];
                scheduler_data.thread_table[search_index].w = core;
                scheduler_data.index[core] = search_index;
                searching = false;
                // TODO only one return
                return psp;
            }
            search_count--;
            searching = search_count > 0;
        }
        panic("Problen in scheduler logic");
    }
    return (psp);
}

void sl_end(int core_number)
{
    scheduler_data.thread_table[core_number].w = NO_THREAD;
    // TODO Must switch !!!!!!
    // svc call with thread change function
}
// all scheduling functions through SVC calls ????
// svc must have same stacking as pendsv ?????

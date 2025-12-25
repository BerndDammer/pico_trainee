#include <stdio.h>
#include <string.h> // memcpy

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/exception.h"
#include "pico/platform.h"
#include "cmsis_gcc.h"
#include "rp2040_scheduler.h"
#include "rp2040_scheduler_stubs.h"
#include "project_main.h"

// RP2040 has two cores
#define CORECOUNT 2
#define CORE0 0

#define ENTER_SCHEDULER
#define LEAVE_SCHEDULER
#define MAX_THREADS 8
#define NO_THREAD (-1)

#define YOU_SHOULD_NOT_BE_HERE \
    while (true)               \
        ; //

struct cpu_info
{
    uint32_t psp;
    uint32_t msp;
    uint32_t xPSR;
    uint32_t wait_loop_counter;

} cpu_info_at_start[CORECOUNT];

extern void unallowed_thread_return_end(void);
extern void init_thread_table(void);

//extern void dummy(void);

void init_sys_tick(int core)
{
}

void __attribute__((noreturn)) main(void)
{
    int core = get_core_num();
    struct cpu_info *cpi = &cpu_info_at_start[core];
    // get stackpointers at start
    cpi->msp = __get_MSP();
    cpi->psp = __get_PSP();
    cpi->xPSR = __get_xPSR();

    if (core == CORE0)
    {
        // relocate core0 VTOR to scratchy_base
        stdio_init_all();
        __disable_irq();
        // init_thread_table();
    }

    init_sys_tick(core);

    exception_set_exclusive_handler(SVCALL_EXCEPTION, SVC_Handler);

    project_main();

    __enable_irq();

    // the final one and only action at MSP without event
    cpi->wait_loop_counter = 0;
    while (true)
    {
        __WFI();
        cpi->wait_loop_counter++;
    }
}

void unallowed_thread_return_end(void)
{
    // YOU_SCHOULD_NOT_BE_HERE
    __SEV();
    while (true)
    {
    }
}

// scheduler

struct thread_stack_frame thread_list[MAX_THREADS];

void init_thread_table(void)
{
    ENTER_SCHEDULER;
    LEAVE_SCHEDULER;
}

void create_thread(
    struct thread_stack_frame *initial_stack,
    uint8_t *stack_base,
    uint32_t stack_size)
{
    initial_stack->lr = (uint32_t)unallowed_thread_return_end;
    register size_t s = sizeof(struct thread_stack_frame);
    memcpy(&stack_base[stack_size - s], initial_stack, s);

    ENTER_SCHEDULER;
    LEAVE_SCHEDULER;
}

void SVC_Handler_Main(uint32_t lr,
                      struct thread_stack_frame *msp,
                      struct thread_stack_frame *psp,
                      uint32_t xPSR)
{
    if (lr & 4)
    {
        ENTER_SCHEDULER;
        LEAVE_SCHEDULER;
    }
    else
    {
        YOU_SHOULD_NOT_BE_HERE;
    }
};

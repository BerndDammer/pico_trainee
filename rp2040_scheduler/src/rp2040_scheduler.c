#include <stdio.h>
#include <string.h> // memcpy

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "hardware/exception.h"
#include "pico/platform.h"
#include "pico/multicore.h"
#include "cmsis_gcc.h"
// #include "core_cm0plus.h" //conflicts with others
#include "rp2040_scheduler.h"
#include "rp2040_scheduler_stubs.h"
#include "project_main.h"
#include "linker_workaround.h"
#include "hardware/structs/systick.h"

// RP2040 has two cores
#define CORECOUNT 2
#define CORE0 0

#define ENTER_SCHEDULER
#define LEAVE_SCHEDULER
#define MAX_THREADS 8
#define NO_THREAD (struct thread_stack_frame *)(-1)
#define LOWEST_PRIORITY 0XFF
#define THREAD_MODE_RETURN_CODE 0XFFFFFFF9

#define YOU_SHOULD_NOT_BE_HERE __BKPT(0X33);

struct cpu_info
{
    uint32_t psp;
    uint32_t msp;
    CONTROL_t control;
    uint32_t wait_loop_counter;

} cpu_info_at_start[CORECOUNT];

extern void init_thread_table(void);

void SysTick_Handler(void)
{
    scb_hw->icsr = 1 << 28; // Set PendSV Pending
}

void PendSV_Handler_Main(uint32_t lr,
                         struct thread_stack_frame *msp,
                         struct thread_stack_frame *psp,
                         CONTROL_t control)
{
    ENTER_SCHEDULER;

    LEAVE_SCHEDULER;
}

void init_sys_tick(int core)
{
    // TODO check clock source
    systick_hw->csr = 0;                                 // switch systick off
    systick_hw->cvr = 0L;                                // current value
    systick_hw->rvr = clock_get_hz(clk_sys) / 1UL - 1UL; // reload
    systick_hw->calib;                                   // calibration

    exception_set_exclusive_handler(SYSTICK_EXCEPTION, SysTick_Handler);
    exception_set_priority(SYSTICK_EXCEPTION, LOWEST_PRIORITY);

    exception_set_exclusive_handler(PENDSV_EXCEPTION, PendSV_Handler);
    exception_set_priority(PENDSV_EXCEPTION, LOWEST_PRIORITY);

    // last action activate the counter
    systick_hw->csr = 7;
}

void __attribute__((noreturn)) main(void)
{
    int core = get_core_num();
    struct cpu_info *cpi = &cpu_info_at_start[core];
    // get stackpointers at start
    cpi->msp = __get_MSP();
    cpi->psp = __get_PSP();
    cpi->control.w = __get_CONTROL();

    if (core == CORE0)
    {
        // relocate core0 VTOR to scratch_y_base
        __disable_irq();
        memcpy(CORE0_VTOR, CORE0_VTOR_SOURCE, VTOR_SIZE);
        scb_hw->vtor = (uint32_t)CORE0_VTOR;
        __enable_irq();
        stdio_init_all();

        __disable_irq();
        init_thread_table();

        // start core 1
        memcpy(CORE0_VTOR, CORE0_VTOR_SOURCE, VTOR_SIZE);
        multicore_reset_core1();
        multicore_launch_core1_raw(main, (uint32_t *)CORE1_STACK_TOP, (uint32_t)CORE1_VTOR);
    }

    init_sys_tick(core);

    exception_set_exclusive_handler(SVCALL_EXCEPTION, SVC_Handler);
    exception_set_priority(SVCALL_EXCEPTION, LOWEST_PRIORITY);

    __enable_irq();

    if (core == CORE0)
    {
        project_main();
    }

    // TODO eenter first Thread


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
    __SEV();
    YOU_SHOULD_NOT_BE_HERE;
    while (true)
    {
    }
}

// scheduler

struct thread_stack_frame *thread_list[MAX_THREADS];

void init_thread_table(void)
{
    ENTER_SCHEDULER;
    for (int i = 0; i < MAX_THREADS; i++)
    {
        thread_list[i] = NO_THREAD;
    }
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
                      CONTROL_t control)
{
    if (lr != THREAD_MODE_RETURN_CODE)
    {
        YOU_SHOULD_NOT_BE_HERE;
    }
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

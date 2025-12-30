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
#define THREAD_MODE_PSP_RETURN_CODE 0XFFFFFFD

#define YOU_SHOULD_NOT_BE_HERE __BKPT(0X33);

struct cpu_info
{
    uint32_t psp;
    uint32_t msp;
    CONTROL_t control;
    uint32_t idle_loop_counter;

} cpu_info_at_start[CORECOUNT];

extern void thread_end_by_return(void);

extern void init_thread_table(void);
extern void __attribute__((noreturn)) enter_idle_thread(uint32_t core_num);

void SysTick_Handler(void)
{
    scb_hw->icsr = 1 << 28; // Set PendSV Pending
}

struct thread_stack_frame *My_PendSV_Handler_Main(uint32_t lr,
                            struct thread_stack_frame *msp,
                            struct thread_stack_frame *psp,
                            CONTROL_t control)
{
    ENTER_SCHEDULER;

    LEAVE_SCHEDULER;
    return(psp);
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

    exception_set_exclusive_handler(PENDSV_EXCEPTION, My_PendSV_Handler);
    exception_set_priority(PENDSV_EXCEPTION, LOWEST_PRIORITY);

    // last action activate the counter
    // TODO use sys_clk
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

        init_thread_table();
    }

    // init systick scheduler on both cores
    init_sys_tick(core);
    exception_set_exclusive_handler(SVCALL_EXCEPTION, My_SVC_Handler);
    exception_set_priority(SVCALL_EXCEPTION, LOWEST_PRIORITY);

    if (core == CORE0)
    {
        __enable_irq();
        project_core0_main();
        __disable_irq();

        // start core 1
        // project_coreX_main not in parallel
        memcpy(CORE0_VTOR, CORE0_VTOR_SOURCE, VTOR_SIZE);
        multicore_reset_core1();
        multicore_launch_core1_raw(main, (uint32_t *)CORE1_STACK_TOP, (uint32_t)CORE1_VTOR);
    }
    else
    {
        __enable_irq();
        project_core1_main();
        __disable_irq();
    }

    enter_idle_thread(core);
}

void thread_end_by_return(void)
{
    // TODO delete active thread and enter idle if not enough threads remaining
    __SEV();
    YOU_SHOULD_NOT_BE_HERE;
    while (true)
    {
    }
}

// scheduler
// every non running thread is identified by its psp
// a running thread has the core number in it
// a dead thread has NO_THREAD in it (-1)
struct thread_stack_frame *thread_table[MAX_THREADS];

// definitions for the idle threads
struct thread_stack_frame *idle_threads[CORECOUNT];

#define IDLE_STACK_SIZE 64
uint8_t idle_stack_frames[NUM_CORES][IDLE_STACK_SIZE];

/// @brief initialized at scheduler start with all have no thread in it
/// @param  none
void init_thread_table(void)
{
    ENTER_SCHEDULER;
    for (int i = 0; i < MAX_THREADS; i++)
    {
        thread_table[i] = NO_THREAD;
    }
    LEAVE_SCHEDULER;
}

// TODO must execute as sv call or possible dead lock
/// @brief creates a thread
/// @param initial_stack
///        put start address in initial_stack.pc
///        start parameter in initial_stack.r0 to .r3
/// @param stack_base you have to assign the stack by your own
/// @param stack_size
void create_thread(
    standard_thread_start thread_function,
    uint8_t *stack_base,
    uint32_t stack_size,
    uint32_t parameter)
{
    struct thread_stack_frame *psp;
    size_t s = sizeof(struct thread_stack_frame);
    psp = (struct thread_stack_frame *)(stack_base + stack_size - s);

    psp->pc = thread_function;
    psp->r0 = parameter;
    psp->lr = (uint32_t)thread_end_by_return;
    psp->xPSR = 0; // consistent state

    ENTER_SCHEDULER;
    LEAVE_SCHEDULER;
}
// all scheduling function run on thread level 0XC0
// lowes on rp2040
// a guard must only be used because all two cores can enter at the same time

// TODO used ????
void My_SVC_Handler_Main(uint32_t lr,
                         struct thread_stack_frame *msp,
                         struct thread_stack_frame *psp,
                         CONTROL_t control)
{
    if (lr != THREAD_MODE_PSP_RETURN_CODE)
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

//-----------------------------------------------------------
//             idle treads
//-------------------------------------------------
// every core gots one idle thread and enters it after initializion
//

uint32_t idle_thread(uint32_t r0)
{
    struct cpu_info *cpi = &cpu_info_at_start[r0];
    cpi->idle_loop_counter = 0;
    while (true)
    {
        __WFI();
        cpi->idle_loop_counter++;
    }
    YOU_SHOULD_NOT_BE_HERE;
}

// TODO put in thread list
// void __attribute__((noreturn)) enter_idle_thread_stub(
//     void *msp,
//     void *psp,
//     CONTROL_t control,
//     uint32_t lr_return_code)
// {
//     // intentionally set to msp stack top
//     // this kills all msp foreground tasks
//     asm("msr msp,r0");
//     asm("msr psp,r1");     // set stack for first thread start
//     asm("msr control,r2"); // set to protected mode
//     asm("isb");            // ARM Tech manual!
//     asm("mov pc,r3");      // forces return from handler stack from psp unloaad
//     while (true)
//         ; // never here; disable compiler warning
//     YOU_SHOULD_NOT_BE_HERE;
// }
// TODO put in thread list

void __attribute__((noreturn)) enter_idle_thread(uint32_t core_num)
{
    struct thread_stack_frame idle_frame;
    void *msp = core_num ? CORE1_STACK_TOP : CORE0_STACK_TOP;
    void *psp = idle_stack_frames[core_num] + IDLE_STACK_SIZE - (sizeof(struct thread_stack_frame));

    idle_frame.pc = idle_thread;
    idle_frame.r0 = core_num;
    idle_frame.xPSR = 0; // enable interrupts

    idle_threads[core_num] = (struct thread_stack_frame *)core_num;
    // TODO setting control here correct ?????
    // nPRIV = 1 Thread mode nonPrivileged; Handler Mode always privileged
    // SPSEL = 0 here MPS; changed to PSP by return code
    CONTROL_t c;
    {
        c.w = 0;
        c.bits.nPRIV = 1;
        c.bits.SPSEL = 0;
    }
    enter_idle_thread_stub(msp, psp, c, THREAD_MODE_PSP_RETURN_CODE);
    YOU_SHOULD_NOT_BE_HERE;
}

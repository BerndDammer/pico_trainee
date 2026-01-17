#include <stdio.h>
#include <string.h> // memcpy

#include "pico.h"
#include "pico/stdlib.h"
#include "pico/platform.h"
#include "pico/multicore.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "hardware/exception.h"
#include "hardware/structs/systick.h"

#include "rp2040_scheduler.h"
#include "rp2040_scheduler_logic.h"
#include "rp2040_scheduler_stubs.h"

// TODO has pico lib QQQQ
#include "cmsis_gcc.h"

#define CORE0 0

#define ENTER_SCHEDULER
#define LEAVE_SCHEDULER
#define LOWEST_PRIORITY 0XFF

#define YOU_SHOULD_NOT_BE_HERE panic("You should not be here\n")

#include "debug.h"

#define IDLE_STACK_SIZE 256 / 4
uint32_t idle_stacks[NUM_CORES][IDLE_STACK_SIZE];

extern uint32_t thread_end_by_return(uint32_t p);
extern uint32_t __attribute__((noreturn)) idle_thread(uint32_t r0);

/////////////////////////////////////////////////////////////////////////////////////////////
//
//   HANDLER
//
void SysTick_Handler(void)
{
    scb_hw->icsr = 1 << 28; // Set PendSV Pending
}

stack_pointer_t PendSV_Handler_Main(stack_pointer_t psp)
{
    DEBUG_PEND_SV;

    ENTER_SCHEDULER;
    psp = sl_next(get_core_num(), psp);
    LEAVE_SCHEDULER;

    return (psp);
}

void init_sys_tick(int core)
{
    // PendSV before SysTick
    // exception_set_exclusive_handler(PENDSV_EXCEPTION, PendSV_Handler);
    // pendsv only running AFTER setting to psp running mode
    // or PendSV_Handler crashes
    exception_set_exclusive_handler(PENDSV_EXCEPTION, (exception_handler_t)PendSV_Handler);
    exception_set_priority(PENDSV_EXCEPTION, LOWEST_PRIORITY);

    exception_set_exclusive_handler(SYSTICK_EXCEPTION, SysTick_Handler);
    exception_set_priority(SYSTICK_EXCEPTION, LOWEST_PRIORITY);

    systick_hw->csr = 0;                                   // switch systick off
    systick_hw->rvr = clock_get_hz(clk_sys) / 500UL - 1UL; // reload 2ms / 500Hz
    systick_hw->cvr = 0L;                                  // current value
    systick_hw->calib;                                     // calibration
    // last action activate the counter
    systick_hw->csr = 7;

    // 120 times slower
    // systick_hw->rvr = 0X003FFFFF;
    // systick_hw->csr = 3;
}

void __attribute__((noreturn)) main(void)
{
    int core = get_core_num();
    DEBUG_INIT;

    if (core == CORE0)
    {
        sl_init();

        project_core0_main();

        multicore_reset_core1();
        multicore_launch_core1(main);
    }
    else
    {
        project_core1_main();
        busy_wait_us(1000UL);
    }
    // init systick scheduler on both cores
    init_sys_tick(core);

    exception_set_exclusive_handler(SVCALL_EXCEPTION, SVC_Handler);
    exception_set_priority(SVCALL_EXCEPTION, LOWEST_PRIORITY);

    if (core == CORE0)
    {
        // multicore_reset_core1();
        // multicore_launch_core1(main);
    }
    else
    {
        // core 1 gives core 0 start QQQQQ
    }
    { // final destination
        // decision: dont change msp
        // less change of sdk environment
        stack_pointer_t psp;
        psp.pw = &idle_stacks[core][IDLE_STACK_SIZE];
        ENTER_SCHEDULER;
        sl_first(core, psp);
        LEAVE_SCHEDULER;

        startup_thread_suicide_to_idle_thread(core, &idle_thread, psp);
    }
}

uint32_t thread_end_by_return(uint32_t p)
{
    // TODO delete active thread and enter idle if not enough threads remaining
    // TODO can't work this way
    ENTER_SCHEDULER;
    sl_end(get_core_num());
    LEAVE_SCHEDULER;
}

// all scheduling function run on thread level 0XC0
// lowest on rp2040
// a guard must only be used because all two cores can enter at the same time
// TODO join before end ?????
// SVC_Handler_Psp          LR = 0XFFFFFFFD
// SVC_Handler_Msp_Thread   LR = 0XFFFFFFF9
// SVC_Handler_Msp_Handler  LR = 0XFFFFFFF1
void SVC_Handler_PSP_ThreadStack(uint32_t svc_code, struct thread_stack_frame *psp)
{
    switch (svc_code)
    {
    case SVC_THREAD_CREATE:
    {
        stack_pointer_t stack;
        size_t s = sizeof(struct full_stack_frame);
        stack.w = psp->r1 + psp->r2 - s;
        stack.full_stack->pc.w = psp->r0;
        stack.full_stack->r0 = psp->r3; // parameter
        stack.full_stack->lr.starter = &thread_end_by_return;
        stack.full_stack->xPSR.EPSR.T = 1; // consistent state; without thumb HardFault

        ENTER_SCHEDULER;
        sl_new(stack);
        LEAVE_SCHEDULER;
    }
    break;
    case SVC_TEST_LOW:
        scb_hw->icsr = 1 << 28; // Set PendSV Pending
        break;
    default:
        panic("Unsupported SVC call");
        break;
    }
}

stack_pointer_t SVC_Handler_PSP_FullStack(uint32_t svc_code, stack_pointer_t psp)
{
    switch (svc_code)
    {
    case SVC_THREAD_YIELD:
        scb_hw->icsr = 1 << 28; // Set PendSV Pending
        break;
    case SVC_TEST_HIGH:
        scb_hw->icsr = 1 << 28; // Set PendSV Pending
        break;
    default:
        panic("Unsupported SVC call");
        break;
    }
    return psp;
}

void SVC_Handler_MSP_Thread(uint32_t svc_code, struct thread_stack_frame *msp)
{
    switch (svc_code)
    {
    case SVC_THREAD_CREATE:
    {
        stack_pointer_t stack;
        size_t s = sizeof(struct full_stack_frame);
        stack.w = msp->r1 + msp->r2 - s;
        stack.full_stack->pc.w = msp->r0;
        stack.full_stack->r0 = msp->r3; // parameter
        stack.full_stack->lr.starter = &thread_end_by_return;
        stack.full_stack->xPSR.EPSR.T = 1; // consistent state; without thumb HardFault

        ENTER_SCHEDULER;
        sl_new(stack);
        LEAVE_SCHEDULER;
    }
    break;
    case SVC_THREAD_YIELD:
        panic("Are you nuts\n");
        break;
    case SVC_TEST_LOW:
    case SVC_TEST_HIGH:
    {
        scb_hw->icsr = 1 << 28; // Set PendSV Pending
    }
    break;
    default:
        panic("You shold not be here!\n");
        break;
    }
}

void SVC_Handler_MSP_Handler(uint32_t svc_code, struct thread_stack_frame *msp)
{
    switch (svc_code)
    {
    case SVC_TEST_LOW:
    case SVC_TEST_HIGH:
    {
        scb_hw->icsr = 1 << 28; // Set PendSV Pending
    }
    break;
    case SVC_THREAD_YIELD:
        panic("Are you nuts\n");
        break;
    default:
        panic("You should not be here!\n");
        break;
    }
}

//-----------------------------------------------------------
//             idle treads
//-------------------------------------------------
// every core gots one idle thread and enters it after initializion
//

uint32_t __attribute__((noreturn)) idle_thread(uint32_t r0)
{
    while (true)
    {
        __WFE();
    }
    YOU_SHOULD_NOT_BE_HERE;
}

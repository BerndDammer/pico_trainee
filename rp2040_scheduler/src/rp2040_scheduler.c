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
#include "linker_workaround.h"

// TODO has pico lib QQQQ
#include "cmsis_gcc.h"

#define CORE0 0

#define ENTER_SCHEDULER
#define LEAVE_SCHEDULER
#define LOWEST_PRIORITY 0XFF

#define YOU_SHOULD_NOT_BE_HERE __BKPT(0X33);

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
    DEBUG_SYS_TICK;
    scb_hw->icsr = 1 << 28; // Set PendSV Pending
}

stack_pointer_t PendSV_Handler_Main(
    stack_pointer_t psp,
    uint32_t lr,
    stack_pointer_t msp,
    CONTROL_t control)
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

    // last action activate the counter
    // TODO use sys_clk
    systick_hw->csr = 7;

    // 120 times slower
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

        // multicore_reset_core1();
        // multicore_launch_core1(main);
    }
    else
    {
        project_core1_main();
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

        sl_first(core, psp);
        // while(true)__enable_irq();
        // interrupts re-enabled in stub function
        __disable_irq();
        startup_thread_suicide_to_idle_thread(core, &idle_thread, psp);
    }
}

uint32_t thread_end_by_return(uint32_t p)
{
    // TODO delete active thread and enter idle if not enough threads remaining
    ENTER_SCHEDULER;
    sl_end(get_core_num());
    LEAVE_SCHEDULER;
}

// all scheduling function run on thread level 0XC0
// lowes on rp2040
// a guard must only be used because all two cores can enter at the same time
// TODO join before end ?????
void SVC_Handler_Main(uint32_t svc_code, stack_pointer_t psp)
{
    switch (svc_code)
    {
    case SVC_THREAD_CREATE:
    {
        stack_pointer_t stack;
        size_t s = sizeof(struct full_stack_frame);
        stack.w = psp.frame_stack->r1 + psp.frame_stack->r2 - s;
        stack.full_stack->pc.w = psp.frame_stack->r0;
        stack.full_stack->r0 = psp.frame_stack->r3; // parameter
        stack.full_stack->lr.starter = &thread_end_by_return;
        stack.full_stack->xPSR.EPSR.T = 1; // consistent state; without thumb HardFault

        ENTER_SCHEDULER;
        sl_new(stack);
        LEAVE_SCHEDULER;
    }
    break;
    case SVC_THREAD_YIELD:
        scb_hw->icsr = 1 << 28; // Set PendSV Pending
        break;
    default:
        panic("Unsupported SVC call");
        break;
    }
};

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

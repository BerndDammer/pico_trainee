#ifndef RP2040_SCHEDULER
#define RP2040_SCHEDULER

//#include "cmsis_gcc.h"
#include "pico.h"

//------------------------------------------
// CORE0 STACK TOP and Vector Table in SCRATCH Y
// CORE1 STACK TOP and Vector Table in SCRATCH X
// 
// NO ... VECTOR TABLE STAYS at bottom of ram
//
// complete main ram are free for
// memory allocation
// heap
// and process stack

// all schedulings are done on lowest interrupt priority [11XX.XXXX]
// this scheduler schould be always interruptable
// and disables interrups only at startup
//
// dont use 0B11XXXXXX interrupt priority
// all sceduling handlers run in this priority level
//
// spinlock 30 and 31 are reserved for this scheduler
//
// all types of thread synchronisation is up to you
// all threads run round robin
//
// use sdk timer for clocking
//
// if nothing to do then yield
//
// takeing spinlocks reserved by pico-sdk for RTOS usage
// PICO_SPINLOCK_ID_OS1
// PICO_SPINLOCK_ID_OS2
//


typedef uint32_t (*standard_thread_start)(uint32_t parameter);

/// @brief creates a thread
/// @param initial_stack
///        put start address in initial_stack.pc
///        start parameter in initial_stack.r0 to .r3
/// @param stack_base you have to assign the stack by your own
/// @param stack_size// allocate stack ram is up to you
extern void thread_create(
    standard_thread_start thread_function,
    uint32_t *stack_base,
    uint32_t stack_size,
    uint32_t parameter);

extern void thread_yield(void);

// implement these functions to start your project
// _core0_ runs before and not parallel to _core1_xx
// decide which interrupt is active on which core
//
// define your threads and hardware interrupts
// after returning the init funktions your threads will be activated
// by both cores
//
extern void project_core0_main(void);
extern void project_core1_main(void);

#endif

#ifndef RP2040_SCHEDULER
#define RP2040_SCHEDULER

#include "cmsis_gcc.h"

//------------------------------------------
// CORE0 STACK TOP and Vector Table in SCRATCH Y
// CORE1 STACK TOP and Vector Table in SCRATCH X
//
// complete main ram are free for
// memory allocation
// heap
// and process stack

// all schedulings are done on lowest interrupt priority [11XX.XXXX]
// this scheduler schould be always interruptable
// and disables interrups only at startup

typedef uint32_t (*standard_thread_start)(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3);

struct thread_stack_frame
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    standard_thread_start pc;
    uint32_t xPSR;
};

extern void create_thread(
    struct thread_stack_frame *initial_stack,
    uint8_t *stack_base,
    uint32_t stack_size);

#endif

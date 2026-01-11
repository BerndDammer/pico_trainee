// #include "cmsis_gcc.h"
#ifndef RP2040_SCHEDULER_STUBS
#define RP2040_SCHEDULER_STUBS

#include "pico.h"
#include "rp2040_scheduler.h"
#include "rp2040_scheduler_stubs_gas.h"

typedef union
{
    struct
    {
        unsigned int unused : 28;
        unsigned int V : 1;
        unsigned int C : 1;
        unsigned int Z : 1;
        unsigned int N : 1;
    } APSR;
    struct
    {
        unsigned int ISR : 6;
        unsigned int unused : 26;
    } IPSR;
    struct
    {
        unsigned int unused1 : 24;
        unsigned int T : 1;
        unsigned int unused2 : 7;
    } EPSR;
    uint32_t w;
} xPSR_t;

typedef union
{
    uint8_t *bytes;
    struct thread_stack_frame *frame_stack;
    struct full_stack_frame *full_stack;
    uint32_t w;
    uint32_t *pw;
    int32_t signed_w;
} stack_pointer_t;

typedef union
{
    uint16_t *opcode;
    uint32_t w;
    standard_thread_start starter;
} program_counter_t;

struct thread_stack_frame
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    program_counter_t lr;
    program_counter_t pc;
    xPSR_t xPSR;
};

struct full_stack_frame
{
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    program_counter_t lr;
    program_counter_t pc;
    xPSR_t xPSR;
};

typedef union
{
    struct
    {
        unsigned int nPRIV : 1;
        unsigned int SPSEL : 1;
        unsigned int unused : 30;
    } bits;
    uint32_t w;
} CONTROL_t;

extern void SVC_Handler(void);
extern void SVC_Handler_Main(uint32_t svc_code, stack_pointer_t psp);

extern void PendSV_Handler(void);
extern stack_pointer_t PendSV_Handler_Main(
    stack_pointer_t psp,
    uint32_t lr,
    stack_pointer_t msp,
    CONTROL_t control);

void __attribute__((noreturn)) startup_thread_suicide_to_idle_thread(
    uint32_t parameter,
    standard_thread_start start_func,
    stack_pointer_t psp);

#endif

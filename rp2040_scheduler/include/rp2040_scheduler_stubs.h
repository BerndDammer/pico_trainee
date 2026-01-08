#include "cmsis_gcc.h"
#include "rp2040_scheduler.h"
#include "rp2040_scheduler_stubs_gas.h"

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

struct full_stack_frame
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    standard_thread_start pc;
    uint32_t xPSR;
    uint32_t r11;
    uint32_t r10;
    uint32_t r9;
    uint32_t r8;
    uint32_t r7;
    uint32_t r6;
    uint32_t r5;
    uint32_t r4;
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
extern void SVC_Handler_Main(uint32_t lr,
                                struct thread_stack_frame *msp,
                                struct thread_stack_frame *psp,
                                CONTROL_t control);

extern void PendSV_Handler(void);
extern struct full_stack_frame *PendSV_Handler_Main(
    struct full_stack_frame *psp,
    uint32_t lr,
    void *msp,
    CONTROL_t control);

void __attribute__((noreturn)) startup_thread_suicide_to_idle_thread(
    void *msp,
    void *psp,
    CONTROL_t control,
    uint32_t lr_return_code);

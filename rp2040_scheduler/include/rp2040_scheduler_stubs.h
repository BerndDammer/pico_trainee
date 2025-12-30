#include "cmsis_gcc.h"
#include "rp2040_scheduler.h"

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

extern void My_SVC_Handler(void);
extern void My_SVC_Handler_Main(uint32_t lr,
                                struct thread_stack_frame *msp,
                                struct thread_stack_frame *psp,
                                CONTROL_t control);

extern void My_PendSV_Handler(void);
extern struct thread_stack_frame *My_PendSV_Handler_Main(uint32_t lr,
                                   struct thread_stack_frame *msp,
                                   struct thread_stack_frame *psp,
                                   CONTROL_t control);

void __attribute__((noreturn)) enter_idle_thread_stub(
    void *msp,
    void *psp,
    CONTROL_t control,
    uint32_t lr_return_code);

#define SVC_THREAD_YIELD 2

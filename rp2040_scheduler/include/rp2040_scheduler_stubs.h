#include "cmsis_gcc.h"
#include "rp2040_scheduler.h"

extern void SVC_Handler(void);
extern void SVC_Handler_Main(uint32_t lr, 
    struct thread_stack_frame *msp, 
    struct thread_stack_frame *psp,
    uint32_t xPSR
);

#include "cmsis_gcc.h"
#include "rp2040_scheduler.h"
 
extern void SVC_Handler(void);
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

extern void SVC_Handler_Main(uint32_t lr,
                             struct thread_stack_frame *msp,
                             struct thread_stack_frame *psp,
                             CONTROL_t control);

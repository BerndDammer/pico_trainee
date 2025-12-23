
#include "pico/stdlib.h"
#include <stdio.h>

//#include <cmsis.h>

// define svc handler ??
//int __svc(0x00) svc_service_add(int x, int y);
//int __svc(0x01) svc_service_sub(int x, int y);
//int __svc(0x02) svc_service_incr(int x);



// SVC handler - Assembly wrapper to extract
// SVC handler - main code to handle processing
// Input parameter is stack frame starting address
// obtained from assembly wrapper.
void SVC_Handler_main(unsigned int *svc_args)
{
    // Stack frame contains:
    // r0, r1, r2, r3, r12, r14, the return address and xPSR
    // - Stacked R0 = svc_args[0]
    // - Stacked R1 = svc_args[1]
    // - Stacked R2 = svc_args[2]
    // - Stacked R3 = svc_args[3]
    // - Stacked R12 = svc_args[4]
    // - Stacked LR = svc_args[5]
    // - Stacked PC = svc_args[6]
    // - Stacked xPSR= svc_args[7]
    unsigned int svc_number;
    svc_number = ((char *)svc_args[6])[-2];
    switch (svc_number)
    {
    case 0:
        svc_args[0] = svc_args[0] + svc_args[1];
        break;
    case 1:
        svc_args[0] = svc_args[0] - svc_args[1];
        break;
    case 2:
        svc_args[0] = svc_args[0] + 1;
        break;
    default: // Unknown SVC request
        break;
    }
    return;
}

void dummy( void )
{
//NVIC_SetPriority(SVCall_IRQn, 0x02);
//SCB -> ICSR ¼ SCB->ICSR j(1 << 28); // Set PendSV pending status
//NVIC_SetPriority(PendSV_IRQn, 0x0FF); // Set PendSV to lowest level

}

// Set PendSV



void PendSV_Handler(void)
{
    printf("[PendSV] Timer interrupt triggered\n");
    return;
}

extern unsigned int SVC_Handler;

// Hard Fault handler in C, with stack frame location and LR value
    // extracted from the assembly wrapper as input parameters
    void hard_fault_handler_c(unsigned int *hardfault_args, unsigned lr_value)
{
    unsigned int stacked_r0;
    unsigned int stacked_r1;
    unsigned int stacked_r2;
    unsigned int stacked_r3;
    unsigned int stacked_r12;
    unsigned int stacked_lr;
    unsigned int stacked_pc;
    unsigned int stacked_psr;
    stacked_r0 = ((unsigned long)hardfault_args[0]);
    stacked_r1 = ((unsigned long)hardfault_args[1]);
    stacked_r2 = ((unsigned long)hardfault_args[2]);
    stacked_r3 = ((unsigned long)hardfault_args[3]);
    stacked_r12 = ((unsigned long)hardfault_args[4]);
    stacked_lr = ((unsigned long)hardfault_args[5]);
    stacked_pc = ((unsigned long)hardfault_args[6]);
    stacked_psr = ((unsigned long)hardfault_args[7]);
    printf("[Hard fault handler]\n");
    printf("R0 = %x\n", stacked_r0);
    printf("R1 = %x\n", stacked_r1);
    printf("R2 = %x\n", stacked_r2);
    printf("R3 = %x\n", stacked_r3);
    printf("R12 = %x\n", stacked_r12);
    printf("Stacked LR = %x\n", stacked_lr);
    printf("Stacked PC = %x\n", stacked_pc);
    printf("Stacked PSR = %x\n", stacked_psr);
    printf("Current LR = %x\n", lr_value);
    while (1)
    {
        //__halt();
        //__set_PSP(22);
        //MSR PSP, R0
        printf("Current LR = %x\n", SVC_Handler);
    }
        ; // endless loop
}

     

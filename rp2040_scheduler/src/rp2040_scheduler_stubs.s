// include defines only
// more makes problems
//
// filename must have uppercase .S
//
#include "rp2040_scheduler_stubs_gas.h"

.section .text
.global My_SVC_Handler
.type My_SVC_Handler, %function
.global My_PendSV_Handler
.type My_PendSV_Handler, %function


//--------- SVC HANDLER --------------
My_SVC_Handler: 
        LDR   R0, =My_SVC_Handler_Main
        MOV   IP, R0
        MOV   R0, LR
        MRS   R1, MSP
        MRS   R2, PSP
        MRS   R3, CONTROL
        BX    IP
     

//--------- PENDSV HANDLER --------------
My_PendSV_Handler: 
        // this and only this exception is used for thread switching
        // the save registers R4-R11 must be saved here
        MRS     R2, PSP
        PUSH    {R4-R7}
        # PUSH    {R8-R11}



        LDR   R0, =My_PendSV_Handler_Main
        MOV   IP, R0
        MOV   R0, LR
        MRS   R1, MSP
        MRS   R2, PSP
        MRS   R3, CONTROL
        BLX    IP
        // pop registers from psp
        // force a 
        POP     {R4-R7}
        

.global thread_yield
.type thread_yield, %function
//--------- SVC ENTRY THREAD  --------------
// can be foreground because it triggers PendSV
thread_yield:
        SVC     SVC_THREAD_YIELD
        BX      LR


//--------- main thread termination -------
.global enter_idle_thread_stub
.type enter_idle_thread_stub, %function
enter_idle_thread_stub:
                                // intentionally set to msp stack top
                                // this kills all msp foreground tasks
        MSR     MSP, R0
        MSR     PSP, R1         // set stack for first thread start
        MSR     CONTROL,R2      // set to protected mode
        ISB                     // ARM Tech manual!
        MOV     PC, R3          // forces return from handler stack from psp unloaad

.end        
//---------

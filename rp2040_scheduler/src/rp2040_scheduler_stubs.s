//#include "rp2040_scheduler_stubs.h"

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
        LDR   R0, =My_PendSV_Handler_Main
        MOV   IP, R0
        MOV   R0, LR
        MRS   R1, MSP
        MRS   R2, PSP
        MRS   R3, CONTROL
        BX    IP

.global thread_yield
.type thread_yield, %function
//--------- SVC ENTRY THREAD  --------------
// can be foreground because it triggers PendSV
thread_yield:
        SVC     =SVC_THREAD_YIELD
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

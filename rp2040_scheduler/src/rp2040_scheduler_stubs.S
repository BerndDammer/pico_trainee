// include defines only
// more makes problems
//
// filename must have uppercase .S
//
#include "rp2040_scheduler_stubs_gas.h"

.section .text
.syntax unified 

.global SVC_Handler
.type SVC_Handler, %function

.global PendSV_Handler
.type PendSV_Handler, %function

.global PendSV_Handler_Main
.type PendSV_Handler_Main, %function

// FP Frame Pointer R11
// SL Stack Limit R10
// IP Intra Procedure R12
//
// Opcodes using R8-R11
// ADD; ADD SP,...; SUB SP,...; BLX; BX; CMP; MOV; MRS; MSR


//--------- SVC HANDLER --------------
// may come from PSP or MSP at start thread
SVC_Handler:
        MOV     R3, LR
        MOVS    R2, #4
        TST     R3, R2
        BEQ.N   use_msp
        MRS     R1, PSP
        B.N     use_psp
use_msp:
        MRS     R1, MSP
use_psp:                
        LDR     R2, [R1,#6*4]     // stacked pc; index word count im op code
        SUBS    R2, #2
        LDRB    R0, [R2]     
        LDR     R3, =SVC_Handler_Main
        BX      R3
     

//--------- PENDSV HANDLER --------------
// this and only this exception is used for thread switching
// the save registers R4-R11 must be saved here
//
// IP is alt-name for R12
//
// linked in by name
PendSV_Handler:
        PUSH    {LR}                    // save whatever comming from

        MRS     R0, PSP
        SUBS    R0, #32                 // R0 has PSP after saving {R4-R11}

        MOV     R1, R0                  // lower part fixed register
        STMIA   R1!, {R4-R7}

        MOV     R4, R8                  // higher part fixed register
        MOV     R5, R9
        MOV     R6, R10
        MOV     R7, R11
        STMIA   R1!, {R4-R7}            // R1 has already correct pointer

        // R0 contains actual valid PSP
        // for full register save
        // not the real PSP register
        MOV     R1, LR
        MRS     R2, MSP
        MRS     R3, CONTROL

        BL.W    PendSV_Handler_Main
        // R0 contains the PSP of the now Thread to activate

        // restore save register {R4-R11}
        MOV     R1, R0
        ADDS    R1, #16
        LDMIA   R1!, {R4-R7}
        //---------------------- R1 contains now PSP for Thread return
        MOV     R8, R4
        MOV     R9, R5
        MOV     R10, R6
        MOV     R11, R7

        LDMIA   R0!, {R4-R7}

        // inject return to Thread Mode
        MSR     PSP, R1
        POP     {PC}                    // get aqired excepion code

/////////////////////////////////////////////////////////////
//
// Stubs for svc calls
//

.global thread_create
.type thread_create, %function
//--------- SVC ENTRY THREAD  --------------
// can be foreground because it triggers PendSV
thread_create:
        SVC     #SVC_THREAD_CREATE
        BX      LR

.global thread_yield
.type thread_yield, %function
//--------- SVC ENTRY THREAD  --------------
// can be foreground because it triggers PendSV
thread_yield:
        SVC     #SVC_THREAD_YIELD
        BX      LR




/////////////////////////////////////////////////////////////
//
//
//--------- main thread termination -------
.global startup_thread_suicide_to_idle_thread
.type startup_thread_suicide_to_idle_thread, %function

startup_thread_suicide_to_idle_thread:
                                // intentionally set to msp stack top
                                // this kills all msp foreground tasks
        MSR     PSP, R2         // set stack for first thread start
        MSR     MSP, R3

        // first activate psp
        // then enable interrupts
        // last switch to unprivileged mode
        MOVS    R3,#2           // psp stack and privileged
        MSR     CONTROL,R3      // set to protected mode
        ISB                     // ARM Tech manual!
        CPSIE   I               // final enable interrupts
        MOVS    R3,#3           // psp stack and protected mode
        MSR     CONTROL,R3      // set to protected mode
        ISB                     // ARM Tech manual!
        BX      R1          

.end        
//---------

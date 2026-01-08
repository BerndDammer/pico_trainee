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

// FP Frame Pointer R11
// SL Stack Limit R10
// IP Intra Procedure R12
//
// Opcodes using R8-R11
// ADD; ADD SP,...; SUB SP,...; BLX; BX; CMP; MOV; MRS; MSR


//--------- SVC HANDLER --------------
SVC_Handler: 
        LDR   R0, =SVC_Handler_Main
        MOV   IP, R0
        MOV   R0, LR
        MRS   R1, MSP
        MRS   R2, PSP
        MRS   R3, CONTROL
        BX    IP
     

//--------- PENDSV HANDLER --------------
// this and only this exception is used for thread switching
// the save registers R4-R11 must be saved here
//
// IP is alt-name for R12
//
// linked in by name
PendSV_Handler: 
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

        BL.W    My_PendSV_Handler_Main
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
        LDR     R1, loc_return_code    // can't load 32 bit direct !
        BX      R1

        .align 4
loc_return_code:                //return to Thread mode with PSP
        .word   0XFFFFFFFD

.global thread_yield
.type thread_yield, %function
//--------- SVC ENTRY THREAD  --------------
// can be foreground because it triggers PendSV
thread_yield:
        SVC     #SVC_THREAD_YIELD
        BX      LR


//--------- main thread termination -------
.global startup_thread_suicide_to_idle_thread
.type startup_thread_suicide_to_idle_thread, %function

startup_thread_suicide_to_idle_thread:
                                // intentionally set to msp stack top
                                // this kills all msp foreground tasks
        MSR     MSP, R0
        MSR     PSP, R1         // set stack for first thread start
        MSR     CONTROL,R2      // set to protected mode
        ISB                     // ARM Tech manual!
        MOV     PC, R3          // forces return from handler stack from psp unloaad

.end        
//---------

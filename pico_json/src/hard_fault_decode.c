/*
 * hard_fault_decode.c
 *
 *  Created on: 15.03.2023
 *      Author: manni4
 */

#include "hard_fault_decode.h"
#include "reset_fix_mem.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void appendprintf(char *buffer, const char *format, ...)
{
    va_list a_list;
    va_start(a_list, format);

    char *next = buffer + strlen(buffer);
    vsprintf(next, format, a_list);
}

// TODO: exists im CortexM0 ????
//const unsigned int *CFSR = (unsigned int*) 0XE000ED28;
//const unsigned int *HFSR = (unsigned int*) 0XE000ED2C;

unsigned int lllr;
void myHardfaultException( //
        unsigned int r0,  //
        unsigned int r1,  //
        unsigned int r2,  //
        unsigned int r3,  //
        unsigned int r12, //
        void *lr,         //
        void *pc,         //
        unsigned int xpsr //
        )
{
    register unsigned int act_lr __asm( "lr" );
    lllr = act_lr;
    // TODO: why does this not work
    //register unsigned int _psp __asm( "psp" );

    // TODO: do this without __ASM(
    //register unsigned int real_r0 __asm( "r0" );
    //__asm(" MRS R0,PSP");
    //volatile unsigned int *procStackPointer = (void*) real_r0;
    // psp has 0xFFFFFFFC

    *reset_buffer = 0;

    //if ((unsigned int)procStackPointer > 0X20042000)
    {
        appendprintf(reset_buffer, "Hardfault\n");
        //appendprintf(reset_buffer, "PSP:  %#10X  \n", procStackPointer);
        appendprintf(reset_buffer, "R0:   %#10X  \n", r0);
        appendprintf(reset_buffer, "R1:   %#10X  \n", r1);
        appendprintf(reset_buffer, "R2:   %#10X  \n", r2);
        appendprintf(reset_buffer, "R3:   %#10X  \n", r3);
        appendprintf(reset_buffer, "R12:  %#10X  \n", r12);
        appendprintf(reset_buffer, "LR:   %#10X  \n", lr);
        appendprintf(reset_buffer, "PC:   %#10X  \n", pc);
        appendprintf(reset_buffer, "XPSR: %#10X  \n", xpsr);
        appendprintf(reset_buffer, "actlr %#10X  \n", lllr);
        //appendprintf(reset_buffer, "CFSR: %#10X  \n", *CFSR);
        //appendprintf(reset_buffer, "HFSR: %#10X  \n", *HFSR);

    }
    /*
     else
     {
     appendprintf(reset_buffer, "Hardfault\n");
     appendprintf(reset_buffer, "R0:   %#10X    psp[0]  %#10X\n", r0, procStackPointer[0]);
     appendprintf(reset_buffer, "R1:   %#10X    psp[1]  %#10X\n", r1, procStackPointer[1]);
     appendprintf(reset_buffer, "R2:   %#10X    psp[2]  %#10X\n", r2, procStackPointer[2]);
     appendprintf(reset_buffer, "R3:   %#10X    psp[3]  %#10X\n", r3, procStackPointer[3]);
     appendprintf(reset_buffer, "R12:  %#10X    psp[4]  %#10X\n", r12, procStackPointer[4]);
     appendprintf(reset_buffer, "LR:   %#10X    psp[5]  %#10X\n", lr, procStackPointer[5]);
     appendprintf(reset_buffer, "PC:   %#10X    psp[6]  %#10X\n", pc, procStackPointer[6]);
     appendprintf(reset_buffer, "XPSR: %#10X    psp[7]  %#10X\n", xpsr, procStackPointer[7]);
     }
     */

    goodby_doit();
}


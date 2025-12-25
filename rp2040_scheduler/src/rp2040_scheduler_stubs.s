#include "rp2040_scheduler_stubs.h"

.section .text
.global SVC_Handler
.type SVC_Handler, %function


//--------- SVC HANDLER --------------
SVC_Handler: 
        LDR   R0,=SVC_Handler_Main
        MOV   R12,R0
        MOVS  R0, LR
        MRS   R1, MSP
        MRS   R2, PSP
        MRS   R3, xPSR

        BX    R12
.end        
//---------

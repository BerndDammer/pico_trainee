;--------- SVC HANDLER --------------
SVC_Handler  PROC
        ;  EXPORT  SVC_Handler            [WEAK]
        IMPORT SVC_Handler_main
        MOVS  R0, #4
        MOV   R1, LR
        TST   R0, R1
        BEQ   stacking_used_MSP
        MRS   R0, PSP ; first parameter - stacking was using PSP
        LDR   R1, =SVC_Handler_main
        BX    R1
stacking_used_MSP
        MRS   R0, MSP ; first parameter - stacking was using MSP
        LDR   R1, =SVC_Handler_main
        BX    R1
        ENDP
;--------- 
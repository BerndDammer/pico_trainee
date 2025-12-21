// SVC handler - Assembly wrapper to extract
//         stack frame starting address
__asm void SVC_Handler(void)
{
  MOVS   r0, #4
  MOV r1, LR
  TST r0, r1
  BEQ stacking_used_MSP
  MRS R0, PSP ; first parameter - stacking was using PSP
  LDR R1,=__cpp(SVC_Handler_main)
  BX  R1
stacking_used_MSP
  MRS R0, MSP ; first parameter - stacking was using MSP
  LDR R1,=__cpp(SVC_Handler_main)
  BX  R1
}





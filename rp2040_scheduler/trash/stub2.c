// SVC handler - Assembly wrapper to extract
//         stack frame starting address
extern void SVC_Handler_main(void); //necessary ???

void SVC_Handler_TST2TST(void) //Trigger text
{
  __asm volatile
  (
    ".syntax unified          \n"
    ".global SVC_Handler_main \n"     //necessary ???
    ".type SVC_Handler_main, %function          \n"   //necessary ???
    "MOVS   r0, #4            \n"
    "MOV r1, LR               \n"
    "TST r0, r1               \n"
    "BEQ stacking_used_MSP    \n"
    "MRS R0, PSP              \n" //first parameter - stacking was using PSP\n\t
    "LDR R1,=SVC_Handler_main \n"
    "BX  R1                   \n"
  "stacking_used_MSP:         \n"
    "MRS R0, MSP              \n" 
    "LDR R1,=SVC_Handler_main \n"
    "BX  R1\n"
  );
}






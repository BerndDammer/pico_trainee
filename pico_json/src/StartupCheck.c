/*
 * StartupCheck.c
 *
 *  Created on: 13.03.2023
 *      Author: manni4
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/unique_id.h"
#include "hardware/exception.h"
#include "hardware/watchdog.h"

#include"StartupCheck.h"
#include"jsontest.h"

#include "reset_fix_mem.h"

#include "hardware/structs/vreg_and_chip_reset.h"
#include "hardware/regs/vreg_and_chip_reset.h"

#include "hard_fault_decode.h"

void dumpBoardID(void)
{

    pico_unique_board_id_t board_id;
    pico_get_unique_board_id(&board_id);

    printf("Unique identifier:");
    for (int i = 0; i < PICO_UNIQUE_BOARD_ID_SIZE_BYTES; ++i)
    {
        printf(" %02x", board_id.id[i]);
    }
    printf("\n");
}

exception_handler_t oldNmiException;
exception_handler_t oldHardfaultException;
exception_handler_t oldSvcallException;
exception_handler_t oldPendsvException;
exception_handler_t oldSystickException;

// TODO: getting crash parameter
static void myNmiException(void)
{
    sprintf(reset_buffer, "NMI\n");
    goodby_doit();
}
/*
static void myHardfaultException(void)
{
    sprintf(reset_buffer, "Hardfault\n");
    goodby_doit();
}
*/
// TODO: used by FreeRTOS
static void mySvcallException(void)
{
    sprintf(reset_buffer, "Svcall\n");
    goodby_doit();
}
// TODO: used by FreeRTOS
static void myPendsvException(void)
{
    sprintf(reset_buffer, "Pendsv\n");
    goodby_doit();
}
// TODO: really patch this
// TODO: used by FreeRTOS
static void mySystickException(void)
{
    sprintf(reset_buffer, "Systick\n");
    goodby_doit();
}

void set_exceptions(void)
{
    oldNmiException = exception_set_exclusive_handler(NMI_EXCEPTION,
            myNmiException);
    oldHardfaultException = exception_set_exclusive_handler(HARDFAULT_EXCEPTION,
            (void (*)(void))myHardfaultException);
    oldSvcallException = exception_set_exclusive_handler(SVCALL_EXCEPTION,
            mySvcallException);
    oldPendsvException = exception_set_exclusive_handler(PENDSV_EXCEPTION,
            myPendsvException);
    // TODO: really do this
    oldSystickException = exception_set_exclusive_handler(SYSTICK_EXCEPTION,
            mySystickException);
}
void checkWatchdog(void)
{
    printf("watchdog_caused_reboot : %i\n", watchdog_caused_reboot());
    printf("watchdog_enable_caused_reboot : %i\n",
            watchdog_enable_caused_reboot());

}

////////////////////////////////////
/// cpp uncatched throw calls _exit
///
void terminate_handler(void)
{
    strcpy(reset_buffer, "cpp terminate_handler\n");
    goodby_doit();
}


void startup_check(void)
{
    int mask = vreg_and_chip_reset_hw->chip_reset;
    set_exceptions();
#ifdef MISSING    
    set_terminate_handler(terminate_handler);
#endif
    if ((mask & VREG_AND_CHIP_RESET_CHIP_RESET_HAD_POR_BITS)
            && !goodby_ask_and_reset())
    {
        resetfix_counter = 0;
        *reset_buffer = 0;
        strcpy(reset_buffer, "Cold Reset");
    }
    else
    {
        resetfix_counter++;
    }
    printf("---------------------------------Startup Check ||| Counter %i\n",
            resetfix_counter);
    // TODO: dump single bits
    printf("VREG_AND_CHIP_RESET_CHIP_RESET %#10X\n", mask);
}

void startup_check_dump(void)
{
    printf("---Startup Check Info | Counter %i\n", resetfix_counter);

    //sprintf( reset_buffer, "ResetFixCounter : %i\n", resetfix_counter);

    dumpBoardID();
    printf(" rp2040_chip_version : %i\n", rp2040_chip_version());
    printf(" rp2040_rom_version : %i\n", rp2040_rom_version());
    printf(" __get_current_exception : %i\n", __get_current_exception());
    checkWatchdog();
    printf("VREG_AND_CHIP_RESET_CHIP_RESET %#10X\n",
    vreg_and_chip_reset_hw->chip_reset);
}



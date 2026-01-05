/*
 * reset_fix_mem.c
 *
 *  Created on: 13.03.2023
 *      Author: manni4
 */

#include "reset_fix_mem.h"

//#include "pico/platform.h"
#include "pico.h"

// TODO: how to use macros
static unsigned long long __uninitialized_ram(resetfix_token);
unsigned int __uninitialized_ram(resetfix_counter);
char __uninitialized_ram( reset_buffer ) [1024];

#include "hardware/regs/m0plus.h"
#include "hardware/structs/scb.h"

//                                                  1111222233334444
const unsigned long long GOODBY_TOKEN = (unsigned long long) 0x3AFF000144332211ull;

void goodby_doit(void)
{
    resetfix_token = GOODBY_TOKEN;
    // this is seen as a power on reset
    scb_hw->aircr = 0x05FA << 16 | 4; // arm M0(+) Manual
}

int goodby_ask_and_reset(void)
{
    int result;
    result = resetfix_token == GOODBY_TOKEN;
    resetfix_token = 0;
    return result;
}

void force_nmi(void)
{
    scb_hw->icsr = M0PLUS_ICSR_NMIPENDSET_BITS;
}

void force_pendsv(void)
{
    scb_hw->icsr = M0PLUS_ICSR_PENDSVSET_BITS;
}
void force_systick(void)
{
    scb_hw->icsr = M0PLUS_ICSR_PENDSTSET_BITS;
}
/*
 int c;
 int *counter = &c;
 //static __uninitialized_ram(int counter) [1];
 */

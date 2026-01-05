/*
 * reset_fix_mem.h
 *
 *  Created on: 13.03.2023
 *      Author: manni4
 */

#ifndef RESET_FIX_MEM_H_
#define RESET_FIX_MEM_H_

#define RESET_BUFFER_SIZE 1024

extern unsigned int resetfix_counter;
extern char reset_buffer[RESET_BUFFER_SIZE];

void goodby_doit(void);
int goodby_ask_and_reset(void);

void force_nmi(void);
void force_pendsv(void);
void force_systick(void);

#endif /* RESET_FIX_MEM_H_ */

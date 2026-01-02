/*
 * panic.c
 *
 *  Created on: 25.04.2023
 *      Author: manni4
 */

#ifndef PANIC_C_
#define PANIC_C_

#include <stdio.h>
#include "pico/stdlib.h"
#include "panic.h"

/*
 * this panic functions lets other threads alive
 */
void app_panic(char *alarm_text)
{
//	panic(alarm_text);
	printf(alarm_text);
	printf("\nLocked in app_panic\n");
	while(1);
}



#endif /* PANIC_C_ */

/*
 * async_main.c
 *
 *  Created on: 24.04.2023
 *      Author: manni4
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//#include "pico/stdlib.h"
#include "pico/types.h"

#include "pico/async_context.h"
#include "pico/async_context_poll.h"

#include "async_main.h"
#include "async_cyw43.h"
#include "async_lwip.h"
#include "async_console.h"
#include "async_blinky.h"
#include "async_heartbeat.h"
#include "panic.h"


volatile unsigned int poll_counter;
void async_main_loop(void)
{
	async_context_t *async_context_cyw43;
	//async_context_t *async_context_lwip;;
	async_context_t *async_context_console;
	async_context_t *async_context_blinky;
	async_context_t *async_context_heartbeat;

	printf("\nStarting");
	printf("\nTime ms since start %i", //
			to_ms_since_boot(get_absolute_time()) //
					);

	async_context_cyw43 = async_cyw43_init();
	if (async_context_cyw43 == NULL )
	{
		app_panic("cyw43 iii");
	}
	async_lwip_init(async_context_cyw43 );

	async_context_console = async_console_init();
	if (async_context_console == NULL )
	{
		app_panic("console iii");
	}
	async_context_blinky = async_blinky_init();
	if (async_context_blinky == NULL )
	{
		app_panic("blinky iii");
	}

	async_context_heartbeat = async_heartbeat_init();
	if (async_context_heartbeat == NULL )
	{
		app_panic("heartbeat iii");
	}

	while (true)
	{
		async_context_poll( async_context_cyw43);
		//async_context_poll( async_context_lwip);
		async_context_poll( async_context_console);
		async_context_poll( async_context_blinky);
		async_context_poll( async_context_heartbeat);

		poll_counter++;
	}
}


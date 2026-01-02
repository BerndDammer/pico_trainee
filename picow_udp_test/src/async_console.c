/*
 * async_console.c
 *
 *  Created on: 27.04.2023
 *      Author: manni4
 */

#include <stdio.h>
#include "async_console.h"
#include "panic.h"

#include "pico/stdlib.h"
#include "pico/types.h"
#include "pico/async_context.h"
#include "pico/async_context_poll.h"

#include "lwip/netif.h"

#define STR_BUF_LEN 100

static void async_console_menu()
{
	printf("\n\n");
	printf("------------------------------------\n");
	printf("LWIP and UDP Multicast Test\n");
	printf("a Show netif data\n");
	printf("h Show hostname data\n");
	printf("x hardware address\n");
	printf("press key to select\n");
	printf("------------------------------------\n");

}

async_context_poll_t async_context_console;

struct char_callback_para_t
{
	char c;
	bool has;
} cbp;

async_when_pending_worker_t process_char_worker;

//void chars_available_callback(struct char_callback_para_t *char_callback_para) {
void chars_available_callback(void *char_callback_para)
{
	cbp.c = getchar_timeout_us(1);
	cbp.has = true;
	async_context_set_work_pending(&async_context_console.core,
			&process_char_worker);
}

void process_char(async_context_t *context,
		struct async_when_pending_worker *worker)
{
	switch (cbp.c)
	{
	case 'a':
	{
		char buffer[STR_BUF_LEN];

		ip4addr_ntoa_r(&netif_default->ip_addr, buffer, STR_BUF_LEN);
		printf("\nnetif.ip_addr %s", buffer);

		ip4addr_ntoa_r(&netif_default->netmask, buffer, STR_BUF_LEN);
		printf("\nnetif.netmask %s", buffer);

		ip4addr_ntoa_r(&netif_default->gw, buffer, STR_BUF_LEN);
		printf("\nnetif.gw %s\n", buffer);
	}
		break;
	case 'x':
	{
		printf("\nHardware address: ");
		for (int i = 0; i < netif_default->hwaddr_len; i++)
		{
			printf("%02X", netif_default->hwaddr[i]);
			if (i != netif_default->hwaddr_len - 1)
			{
				printf(":");
			}
		}
		printf("\n");
	}
		break;
	case 'h':
	{
		printf("\nhostname: %s\n", netif_default->hostname);
	}
		break;
	case ' ':
		async_console_menu();
		break;
	}
	cbp.has = false;
}

async_context_t* async_console_init(void)
{
	if (!async_context_poll_init_with_defaults(&async_context_console))
	{
		app_panic("Async context console init fail");
		return NULL ;
	}
	stdio_set_chars_available_callback(chars_available_callback, (void*) &cbp);

	process_char_worker.do_work = process_char;
	async_context_add_when_pending_worker(&async_context_console.core,
			&process_char_worker);

	return &async_context_console.core;
}


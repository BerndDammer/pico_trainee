/*
 * task_blinker.c
 *
 *  Created on: 12.06.2023
 *      Author: manni4
 */

#include "task_console.h"

#include <stdio.h>

#include "pico.h"
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "pico/async_context_poll.h"
#include "pico/runtime.h"

#include "pico/cyw43_arch.h"
#include "cyw43.h"	  //
#include "cyw43_ll.h" // scan result
#include "central.h"



//int cb_scan(central_t *para, const cyw43_ev_scan_result_t *sr)
int cb_scan(void *para, const cyw43_ev_scan_result_t *sr)
{
	uint8_t b[256];
	strncpy(b,sr->ssid,sr->ssid_len);
	b[sr->ssid_len] = 0;
	puts(b);
}

void process_char(async_context_t *taskp,
				  async_when_pending_worker_t *worker)
{
	console_task_t *task = worker->user_data;
	switch (task->c)
	{
	case 'a':
	{
		cyw43_arch_enable_ap_mode("MOTU", "nevernever", CYW43_AUTH_WPA3_WPA2_AES_PSK);

		central.status = AP_ACTIVATED;
	}
	break;
	case 's':
	{
		// d.nops = cyw43_arch_wifi_connect_async("flat", "wilhelmine", CYW43_AUTH_WPA3_WPA2_AES_PSK);
	}
	case 'm':
	{
		uint8_t mac[6];
		if (central.cyw43_self != NULL)
		{
			int result;
			result = cyw43_wifi_get_mac(central.cyw43_self, CYW43_ITF_STA, mac);
			printf("STA MAC [%02X:%02X:%02X:%02X:%02X:%02X]\n",
				   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
			result = cyw43_wifi_get_mac(central.cyw43_self, CYW43_ITF_AP, mac);
			printf("AP  MAC [%02X:%02X:%02X:%02X:%02X:%02X]\n",
				   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		}
	}
	case 'h':
	{
		if (central.cyw43_self != NULL)
		{
			cyw43_wifi_scan(central.cyw43_self, NULL, &central, cb_scan);
			puts("scan started");
		}
	}
	case ' ':
	{
		if (central.cyw43_self != NULL)
		{
			printf("cyw43_wifi_link_status %i\n", cyw43_wifi_link_status(central.cyw43_self, CYW43_ITF_STA));
		}
	}
	break;
	default:
		printf("\n\n");
		printf("------------------------------------\n");
		printf("CYW43 Test\n");
		printf("a activate access point\n");
		printf("s activate station\n");
		printf("m show mac\n");
		printf("h scan\n");
		printf("d de-activate\n");
		printf("press key to select\n");
		printf("------------------------------------\n");
		break;
	}
	task->has = false;
}

void chars_available_callback(void *taskp)
{
	console_task_t *task = taskp;
	task->c = getchar_timeout_us(1);
	task->has = true;
	async_context_set_work_pending(&task->async_context.core,
								   &task->worker);
}

void console_task_init(console_task_t *task)
{
	if (!async_context_poll_init_with_defaults(&task->async_context))
	{
		panic("Async context console init fail");
		return;
	}
	stdio_set_chars_available_callback(chars_available_callback, task);

	task->worker.do_work = process_char;
	task->worker.user_data = task;
	async_context_add_when_pending_worker(&(task->async_context.core),
										  &task->worker);
}

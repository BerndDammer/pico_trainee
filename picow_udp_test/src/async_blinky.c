/*
 * async_console.c
 *
 *  Created on: 27.04.2023
 *      Author: manni4
 */

#include <stdio.h>
#include "async_blinky.h"
#include "panic.h"

#include "pico/stdlib.h"
#include "pico/types.h"
#include "pico/async_context.h"
#include "pico/async_context_poll.h"

#include "pico/cyw43_arch.h"

#define BLINK_TIMEOUT_MS 500
#define LED_PIN 0

async_context_poll_t async_context_blinky;
async_at_time_worker_t s_process_blink;
bool isOn = false;

void process_blink(async_context_t *context,
		struct async_work_on_timeout *worker)
{
	isOn = !isOn;
	cyw43_arch_gpio_put(LED_PIN, isOn);

	async_context_add_at_time_worker_in_ms(&async_context_blinky.core,
			&s_process_blink, BLINK_TIMEOUT_MS);
}

async_context_t* async_blinky_init(void)
{
	if (!async_context_poll_init_with_defaults(&async_context_blinky))
	{
		app_panic("Async context console init fail");
		return NULL ;
	}

	s_process_blink.do_work = process_blink;
	async_context_add_at_time_worker_in_ms(&async_context_blinky.core,
			&s_process_blink, BLINK_TIMEOUT_MS);

	return &async_context_blinky.core;
}


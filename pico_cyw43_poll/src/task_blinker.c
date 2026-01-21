/*
 * task_blinker.c
 *
 *  Created on: 12.06.2023
 *      Author: manni4
 */

#include "task_blinker.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define BLINK_TIMEOUT_MS 300


void process_blink(async_context_t *context,
				   struct async_work_on_timeout *worker)
{
	blinky_task_t *task = worker->user_data;

	task->toggle = !task->toggle;
    cyw43_arch_gpio_put(0, task->toggle);

	async_context_add_at_time_worker_in_ms(
		&task->async_context.core,
		&task->worker,
		BLINK_TIMEOUT_MS);
}

void blinky_task_init(blinky_task_t *task)
{
	{
		//something to init ????
	}

	if (!async_context_poll_init_with_defaults(&task->async_context))
	{
		panic("Async context console init fail");
	}

	task->worker.do_work = process_blink;
	task->worker.user_data = task;

	async_context_add_at_time_worker_in_ms(
		&task->async_context.core,
		&task->worker,
		BLINK_TIMEOUT_MS);
}

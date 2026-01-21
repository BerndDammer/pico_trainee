/*
 * task_blinker.h
 *
 *  Created on: 12.06.2023
 *      Author: manni4
 */

#ifndef TASK_CONSOLE_H_
#define TASK_CONSOLE_H_


#include "pico/async_context_poll.h"

typedef struct
{
	async_context_poll_t async_context;
	async_when_pending_worker_t worker;
	char c;
	bool has;
} console_task_t;

void console_task_init(console_task_t *task);

#endif /* TASK_CONSOLE_H_ */

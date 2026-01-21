/*
 * task_blinker.h
 *
 *  Created on: 12.06.2023
 *      Author: manni4
 */

#ifndef TASK_BLINKER_H_
#define TASK_BLINKER_H_

#include "pico/async_context_poll.h"

typedef struct
{
	async_context_poll_t async_context;
    async_at_time_worker_t worker;
    bool toggle;
} blinky_task_t;

void blinky_task_init(blinky_task_t *task);

#endif /* TASK_BLINKER_H_ */

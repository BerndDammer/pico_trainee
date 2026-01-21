/*
 * task_demux.h
 *
 *  Created on: 20.06.2023
 *      Author: manni4
 */

#ifndef TASK_TUD_H_
#define TASK_TUD_H_

#include "pico/async_context_poll.h"

typedef struct
{
	async_context_poll_t async_context;
    async_when_pending_worker_t worker;
    bool active;
} tud_task_t;

void tud_task_init(tud_task_t *task);


#endif /* TASK_TUD_H_ */

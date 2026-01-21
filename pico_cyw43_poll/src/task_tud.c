
#include "task_tud.h"
#include <stdio.h>

#include "pico/async_context_poll.h"
#include "pico/cyw43_arch.h"

#include "central.h"

void process_tud(async_context_t *context,
				 struct async_when_pending_worker *worker)
{
	tud_task_t *task = worker->user_data;

	if (task->active)
	{
		//
	}
	cyw43_arch_poll();
	async_context_set_work_pending(&task->async_context.core, &task->worker);
}

// void tud_task_set_active_init(tud_task_t *task, bool active)
// {
// 	task->active = active;
// }

void tud_task_init(tud_task_t *task)
{

	// tusb_init();

	if (!async_context_poll_init_with_defaults(&task->async_context))
	{
		panic("Async context console init fail");
		return;
	}
	task->worker.do_work = process_tud;
	task->worker.user_data = task;

	cyw43_arch_set_async_context(&task->async_context.core); // must be called before init
	// d.nops = cyw43_arch_init();
	cyw43_arch_init_with_country(CYW43_COUNTRY_GERMANY); // only once or hard-fault
	central.status = INITIALIZED;

	async_context_add_when_pending_worker(&task->async_context.core, &task->worker);
	async_context_set_work_pending(&task->async_context.core, &task->worker);
}

#include "pico.h"
#include "pico/stdio.h"

#include "pico/async_context_poll.h"

#include "task_blinker.h"
#include "task_console.h"
#include "task_tud.h"

#include "central.h"


central_t central;



int main(void)
{
	stdio_init_all();

	central.status = UNINIT;

	console_task_t console_task;
	blinky_task_t blinky_task;
	tud_task_t tud_task;

	console_task_init(&console_task);
	blinky_task_init(&blinky_task);
	tud_task_init(&tud_task);

	while (true)
	{
		async_context_poll(&console_task.async_context.core);
		async_context_poll(&blinky_task.async_context.core);
		async_context_poll(&tud_task.async_context.core);
	}
	return 0;
}

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

#include "FreeRTOS.h"
#include "task.h"

#include "task_blinker.h"
#include "task_console.h"
#include "task_network.h"
#include "task_tud.h"

#include "global_signal.h"

// the magic global !
MainEnvironement_t MainEnvironement;

int gg(void)
{
	return (to_ms_since_boot(get_absolute_time()));
}

int main(void)
{
	printf("\nAt main entry %i\n", gg()); // no output !!!!

	stdio_init_all();

	printf("\nAfter init stdio %i\n", gg());

	MainEnvironement.mainEventGroup = xEventGroupCreate();

	blinker_init(&MainEnvironement);
	console_init(&MainEnvironement);
	network_init(&MainEnvironement);
	task_tud_init(&MainEnvironement);

	printf("\nBefore scheduler %i", gg());
	vTaskStartScheduler();

	puts("You should not be here!");

	return 0;
}

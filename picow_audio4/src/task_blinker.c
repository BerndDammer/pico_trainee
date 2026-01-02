/*
 * task_blinker.c
 *
 *  Created on: 12.06.2023
 *      Author: manni4
 */

#include "task_blinker.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "FreeRTOS.h"
#include "task.h"
#include "task_prio.h"

#include "global_signal.h"

static TaskHandle_t blinker_taskhandle;

#define PICO_DEFAULT_TOGGLE_PIN 16
#define LED_PIN 0
#define BLINK_TIMEOUT_MS 777

void blinker_thread(MainEnvironement_t *MainEnvironement)
{
	bool toggle = true;
	EventBits_t bits = 0;

	// wait for connection
	while (!(bits & EVENT_MASK_CYW43_INIT))
	{
		bits = xEventGroupWaitBits(MainEnvironement->mainEventGroup,
		EVENT_MASK_CONNECTED,
		pdFALSE,
		pdFALSE, 10000);
	}

	while (true)
	{
		cyw43_arch_gpio_put(LED_PIN, toggle);

		toggle = !toggle;

		vTaskDelay(BLINK_TIMEOUT_MS);
	}
}

void blinker_init(MainEnvironement_t *MainEnvironement)
{
	xTaskCreate((void (*)(void*)) blinker_thread, "BLINKER",
	configMINIMAL_STACK_SIZE, MainEnvironement,
	BLINKER_TASK_PRIO, &blinker_taskhandle);
}


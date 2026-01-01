/*
 * task_blinker.c
 *
 *  Created on: 12.06.2023
 *      Author: manni4
 */

#include "task_blinker.h"
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include "task_prio.h"

#include "global_signal.h"

static TaskHandle_t blinker_taskhandle;

#define PICO_DEFAULT_TOGGLE_PIN 16
#define BLINK_TIMEOUT_MS 888

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

static void port_blinker_init()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

void blinker_thread(MainEnvironement_t *MainEnvironement)
{
	port_blinker_init();
	bool toggle = true;

	while (true)
	{
		toggle = !toggle;
	    gpio_put(LED_PIN, toggle);
		vTaskDelay(BLINK_TIMEOUT_MS);
	}
}

void blinker_init(MainEnvironement_t *MainEnvironement)
{
	xTaskCreate((void (*)(void*)) blinker_thread, "BLINKER",
	configMINIMAL_STACK_SIZE, MainEnvironement,
	BLINKER_TASK_PRIO, &blinker_taskhandle);
}


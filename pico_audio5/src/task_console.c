/*
 * task_blinker.c
 *
 *  Created on: 12.06.2023
 *      Author: manni4
 */

#include "task_blinker.h"
#include "pico/stdlib.h"
#include "pico/types.h"

#include "hardware/structs/scb.h"

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "task_prio.h"

#include "global_signal.h"

#define STR_BUF_LEN 100

static TaskHandle_t console_taskhandle;

static int c;

static void console_menu()
{
	printf("\n\n");
	printf("------------------------------------\n");
	printf("AUDIO Test\n");
	printf("r reset processor\n");
	printf("e event bits\n");
	printf("press key to select\n");
	printf("------------------------------------\n");

}

void console_command(int c, MainEnvironement_t *MainEnvironement)
{
	switch (c)
	{
	case 'r':
	{
		scb_hw->aircr = 0x05FA << 16 | 4; // arm M0(+) Manual
	}
		break;
	case 'e':
	{
		puts("\n");

		EventBits_t bits = xEventGroupGetBits(MainEnvironement->mainEventGroup);
		for (unsigned int mask = 0X80000000; mask != 0; mask >>= 1)
		{
			putchar(mask & bits ? '1' : '0');
			if (mask & 0x01010100)
			{
				putchar('.');
			}
		}
		puts("\n");
	}
		break;
	case ' ':
	default:
		console_menu();
		break;
	}
}

void console_thread(MainEnvironement_t *MainEnvironement)
{
	console_menu();

	while (true)
	{
		vTaskDelay(100);
		xEventGroupWaitBits(MainEnvironement->mainEventGroup,
		EVENT_MASK_CONSOLE_CHAR,
		pdTRUE,
		pdFALSE, 100000);
		if (c != 0)
		{
			console_command(c, MainEnvironement);
			c = 0;
		}
		else
		{
			//
		}
	}
}

void chars_available_callback(MainEnvironement_t *MainEnvironement)
{
	c = getchar();
	xEventGroupSetBits(MainEnvironement->mainEventGroup,
	EVENT_MASK_CONSOLE_CHAR);
}

void console_init(MainEnvironement_t *MainEnvironement)
{
	xTaskCreate((CALLEE) console_thread, //
			"CONSOLE", configMINIMAL_STACK_SIZE, MainEnvironement,
			BLINKER_TASK_PRIO, &console_taskhandle);
	stdio_set_chars_available_callback((CALLEE)chars_available_callback, //
			MainEnvironement);
}


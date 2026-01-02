/*
 * async_cyw43.c
 *
 *  Created on: 28.04.2023
 *      Author: manni4
 */

#include "pico/cyw43_arch.h"

#include "pico/async_context_poll.h"

#include "async_cyw43.h"
#include "parameter.h"
#include "panic.h"

#define CONNECT_TIMEOUT_MS 10000

async_context_t* async_cyw43_init(void)
{
	if (cyw43_arch_init_with_country(CYW43_COUNTRY_GERMANY))
	{
		app_panic("cyw43 init failed");
		return NULL ;
	}

	cyw43_arch_enable_sta_mode();

	if (cyw43_arch_wifi_connect_timeout_ms(PARA_SSID, PARA_PWD,
	CYW43_AUTH_WPA2_AES_PSK, CONNECT_TIMEOUT_MS))
	{
		cyw43_arch_deinit();
		app_panic("failed to connect");
		return NULL ;
	}
	else
	{
		printf("Connected.\n");
	}

	// call it once to get a debug trigger
	//cyw43_arch_poll();

	return cyw43_arch_async_context();
}


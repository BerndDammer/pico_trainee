#include <stdio.h>
#include <stdbool.h>

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"

#include "blinker.h"

int blinker_counter = 0;

void blinker_main() {

	const uint LED_PIN = PICO_DEFAULT_LED_PIN;

	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	puts("\nBlinker init done");

	while (true) {
		gpio_put(LED_PIN, 1);
		sleep_ms(700);
		gpio_put(LED_PIN, 0);
		sleep_ms(1200);
		blinker_counter++;

		printf("\nBlinker loop %d", blinker_counter);
	}
}

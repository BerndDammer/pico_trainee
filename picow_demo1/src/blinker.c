#include <stdio.h>
#include <stdbool.h>

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"

#include "pico/cyw43_arch.h"

#include "blinker.h"

int blinker_counter = 0;

void blinker_main() {

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return;
    }

	puts("\nBlinker init done");

	while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
		sleep_ms(700);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
		sleep_ms(1200);
		blinker_counter++;

		printf("\nBlinker loop %d", blinker_counter);
	}
}

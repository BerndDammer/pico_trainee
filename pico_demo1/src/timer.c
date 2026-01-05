#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"

#include <stdint.h>

#include "timer.h"
#include "blinker.h"

int counter = 0;

int64_t timer_callback(alarm_id_t id, void *user_data)
{
    counter++;
    return 0;
}

alarm_callback_t p = &timer_callback;

void timer_init()
{
    // Timer example code - This example fires off the callback after 2000ms
    add_alarm_in_ms(2000, p, NULL, false);

    puts("\nLooper init done");

}

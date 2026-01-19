#include <stdio.h>
#include "pico/stdlib.h"

#include "cyw43.h"

struct
{
    bool toggle;
    int ret;
} d;

void entry(void)
{
    d.toggle = true;
    cyw43_t self;
    cyw43_init(&self);

    // d.nops = cyw43_arch_wifi_connect_blocking("flat", "wilhelmine", CYW43_AUTH_WPA2_MIXED_PSK);

    while (true)
    {
        // cyw43_arch_poll();
        // async_context_poll(&d.ac.core);
        sleep_ms(50);
        // cyw43_arch_gpio_put(0,d.toggle);
        d.toggle = !d.toggle;
    }
}

void main(void)
{
    stdio_init_all();
    entry();
}

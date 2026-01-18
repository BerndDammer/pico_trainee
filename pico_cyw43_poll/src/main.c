#include <stdio.h>
#include "pico/stdlib.h"

#include "pico/cyw43_arch.h"
#include "pico/async_context.h"
#include "pico/async_context_poll.h"

typedef struct _d_
{
    async_context_poll_t ac;
    int nops;
    bool toggle;
} d_t;

int ret;
void entry(void)
{
    d_t d;
    async_context_poll_init_with_defaults(&d.ac);
    cyw43_arch_set_async_context(&d.ac.core);
    cyw43_arch_init_with_country(CYW43_COUNTRY_GERMANY);
    cyw43_arch_enable_sta_mode();

    d.nops = cyw43_arch_wifi_connect_blocking("flat", "wilhelmine", CYW43_AUTH_WPA2_MIXED_PSK);

    while (true)
    {
        cyw43_arch_poll();
        async_context_poll(&d.ac.core);
        sleep_ms(50);
        cyw43_arch_gpio_put(0,d.toggle);
        d.toggle = !d.toggle;
    }
}

void main(void)
{
    stdio_init_all();
    entry();
}

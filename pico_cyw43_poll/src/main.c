#include <stdio.h>
#include "pico/stdlib.h"

#include "pico/cyw43_arch.h"
#include "pico/async_context.h"
#include "pico/async_context_poll.h"

#include "cyw43.h"

typedef struct _d_
{
    async_context_poll_t ac;
    int nops;
    bool toggle;
    cyw43_t *self;
} d_t;

void entry(void)
{
    d_t d;
    async_context_poll_init_with_defaults(&d.ac);
    cyw43_arch_set_async_context(&d.ac.core); // must be called before init

    // d.nops = cyw43_arch_init();
    d.nops = cyw43_arch_init_with_country(CYW43_COUNTRY_GERMANY); // only once or hard-fault
    //cyw43_arch_enable_sta_mode();

    //d.nops = cyw43_arch_wifi_connect_async("flat", "wilhelmine", CYW43_AUTH_WPA3_WPA2_AES_PSK);
    cyw43_arch_enable_ap_mode("MOTU","nevernever", CYW43_AUTH_WPA3_WPA2_AES_PSK);
    
    while (true)
    {
        cyw43_arch_poll();
        async_context_poll(&d.ac.core);
        //printf("cyw43_wifi_link_status %i\n", cyw43_wifi_link_status(d.self, CYW43_ITF_STA));
        sleep_ms(300);
        cyw43_arch_gpio_put(0, d.toggle);
        d.toggle = !d.toggle;
    }
}


void main(void)
{
    stdio_init_all();
    entry();
}

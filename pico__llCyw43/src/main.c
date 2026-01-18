#include <stdio.h>
#include "pico/stdlib.h"

// #include "pico/cyw43_driver.h"
#include "cyw43.h"
#include "pico/cyw43_arch.h"

typedef struct _cb_data_
{
    cyw43_ll_t cyw43_ll;
    int nops;
    uint8_t mac[6];
} cb_data_t;

int ret;
const uint8_t mac[6] = {1, 2, 3, 4, 5, 6};
void entry(void)
{
    cb_data_t cb_data =
        {
            .mac = {0x22, 0x33, 0x44, 0x55, 0x66, 0x77} //
        };
        cyw43_arch_init();

    cyw43_ll_init(&cb_data.cyw43_ll, &cb_data);
    ret = cyw43_ll_bus_init(&cb_data.cyw43_ll, mac); //????

    ret = cyw43_ll_wifi_on(&cb_data.cyw43_ll, 0);
    ret = cyw43_ll_wifi_join(&cb_data.cyw43_ll, 4, "flat", 10, "wilhelmine", CYW43_AUTH_WPA2_MIXED_PSK, NULL, CYW43_CHANNEL_NONE);

    while (true)
    {
        //cyw43_poll();
        sleep_ms(50);
    }
}

void main(void)
{
    stdio_init_all();
    entry();
}

#include "rp2040_scheduler.h"
#include "thread_list.h"

#include "pico/stdlib.h"

void init_pin(uint32_t pin)
{
    gpio_init(pin);
    gpio_set_pulls(pin, true, false);
    gpio_set_drive_strength(pin, GPIO_DRIVE_STRENGTH_2MA);
    gpio_set_dir(pin, true);
}

uint32_t uncooperative_thread(uint32_t base_pin)
{
    init_pin(base_pin);
    init_pin(base_pin + 1);
    //int sleep_time = 200 + 30 * base_pin;
    int sleep_time = 100 + 10 * base_pin;
    while (true)
    {
        gpio_put(base_pin, true);
        gpio_put(base_pin + 1, false);
        sleep_ms(sleep_time);
        gpio_put(base_pin, false);
        gpio_put(base_pin + 1, true);
        sleep_ms(sleep_time);
    }
    return 0;
}

uint32_t dummy_stack[7][256 / 4];

void project_core0_main(void)
{
    stdio_init_all();

    int i;
    for (i = 0; i < 7; i++)
    {
        thread_create(&uncooperative_thread, dummy_stack[i], 256, 2 * i + 2);
    }
}

void project_core1_main(void)
{
    // init hardware for core1 NVIC
    //thread_yield();
}

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

int64_t alarm_callback(alarm_id_t id, void *user_data)
{
    return 0;
}

void init_pin(uint32_t pin)
{
    gpio_init(pin);
    gpio_set_pulls(pin, true, false);
    gpio_set_drive_strength(pin, GPIO_DRIVE_STRENGTH_2MA);
    gpio_set_dir(pin, true);
}

int main()
{
    int loop_counter = 0;
    stdio_init_all();
    uint32_t i, j;
    for (i = 2; i < 16; i++)
    {
        init_pin(i);
    }
    while (true)
    {
        for (i = 0; i < 7; i++)
        {
            int pina = i * 2 + 2;
            int pinb = pina + 1;
            for (j = 0; j < 4; j++)
            {
                gpio_put(pina, true);
                gpio_put(pinb, false);
                sleep_ms(250);
                gpio_put(pina, false);
                gpio_put(pinb, true);
                sleep_ms(250);
            }
            gpio_put(pina, false);
            gpio_put(pinb, false);
        }
        printf("Chasing Lights %i\n", loop_counter++);
    }
}

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"

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

void measure_freqs(void)
{
    uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
    uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
    uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
    uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
    uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
    uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
#ifdef CLOCKS_FC0_SRC_VALUE_CLK_RTC
    uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);
#endif
    printf("pll_sys = %dkHz\n", f_pll_sys);
    printf("pll_usb = %dkHz\n", f_pll_usb);
    printf("rosc = %dkHz\n", f_rosc);
    printf("clk_sys = %dkHz\n", f_clk_sys);
    printf("clk_peri = %dkHz\n", f_clk_peri);
    printf("clk_usb = %dkHz\n", f_clk_usb);
    printf("clk_adc = %dkHz\n", f_clk_adc);
#ifdef CLOCKS_FC0_SRC_VALUE_CLK_RTC
    printf("clk_rtc = %dkHz\n", f_clk_rtc);
#endif
    // Can't measure clk_ref / xosc as it is the ref
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
        measure_freqs();
    }
}

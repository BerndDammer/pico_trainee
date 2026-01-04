#include "spi.h"

#include <stdbool.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/spi.h"

#define FSPI 20000
#define SPI_TX_PIN 15
#define FLAG_GPIO 14
#define SPI spi1

unsigned int crc_polynom;
unsigned int act_shift;
unsigned int int_counter = 0;
spi_hw_t *mySpiHW;

unsigned int incFlag;

void on_spi_out_ready()
{
    gpio_put(FLAG_GPIO, true);
    int_counter++;

    if (incFlag)
        act_shift++;

    int p = crc_polynom;
    int s = act_shift;

    for (int i = 0; i < 16; i++)
    {
        if (s & 0x8000)
        {
            s = (s << 1) ^ crc_polynom;
            //r = (r << 1) + 1;
        }
        else
        {
            s <<= 1;
            //r <<= 1;
        }
    }
    act_shift = s;

    // sdk function don't work in interrupt mode
    // spi_write16_blocking(SPI, &act_shift, 1);

    // interrupt operation requires direct write to data register
    // to have writing AND interrupt clear
    //mySpiHW->dr = s;
    //mySpiHW->dr = r;
    mySpiHW->dr = s >> 16;
    //mySpiHW->icr = 3;
    // deleting interrupt flag not necessary
    gpio_put(FLAG_GPIO, false);
}

void crcnoise_init(void)
{
    act_shift = 1;
    crc_polynom = 0x8005;

    gpio_init(FLAG_GPIO);
    gpio_set_function(FLAG_GPIO, GPIO_FUNC_SIO);
    gpio_set_dir(FLAG_GPIO, true);

    gpio_init(SPI_TX_PIN);
    gpio_set_function(SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_dir(SPI_TX_PIN, true);

    spi_init(SPI, FSPI);
    spi_set_format(SPI, 16, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);

    // spi hardware interrupt not part of sdk
    mySpiHW = spi_get_hw( SPI);
    mySpiHW->imsc = 8;

    //spi_write16_blocking(SPI, &act_shift, 1);

    irq_set_exclusive_handler(SPI1_IRQ, on_spi_out_ready);
    irq_set_enabled(SPI1_IRQ, true);

}

void crcnoise_set_shift(int shift)
{
    act_shift = shift;
}

void crcnoise_set_polynom(int p)
{
    //crc_polynom = p;
    crc_polynom = p & 0x7FFF;
}

void crcnoise_toggle_increment(void)
{
    incFlag = !incFlag;
}

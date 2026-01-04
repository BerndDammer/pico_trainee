/*
 * ds1631.c
 *
 *  Created on: 26.03.2023
 *      Author: manni4
 */

#include <stdbool.h>
#include <string.h>
#include "ds1631.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"

#define SCL 17
#define SDA 16

#define CLK (400 * 1000)
#define I2C i2c0

#define ADDR_FIX 9
#define ADDR_PIN 7
#define ADDR_BASE ((ADDR_FIX <<3)| ADDR_PIN)

#define CONTINUE true
#define FINISH false

enum DS1631_CMD
{
    START_CONVERT = 0X51,
    STOP_CONVERT = 0x22,
    READ_TEMP = 0XAA,
    ACC_TH = 0XA1,
    ACC_TL = 0XA2,
    ACC_CONF = 0XAC,
    SW_POR = 0X54,
};

static void init4tool(void)
{
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(SDA, SCL, GPIO_FUNC_I2C));

}

void ds1631_init(void)
{
    i2c_init(I2C, CLK);

    gpio_set_function(SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SDA);
    gpio_pull_up(SCL);

    init4tool();

}

int ds1631_check_addr(void)
{
    unsigned char rxdata[100];
    memset(rxdata, 0, 100);
    int ret;
    ret = i2c_read_blocking(I2C, ADDR_BASE, rxdata, 1, FINISH);

    //return rxdata[0];
    return ret;

}

void ds1631_get_regs(ds1631_regs *r)
{
    volatile int t; // for debugging
    unsigned char buffer[2];

    buffer[0] = ACC_CONF;
    t = i2c_write_blocking(I2C, ADDR_BASE, buffer, 1, CONTINUE);
    t = i2c_read_blocking(I2C, ADDR_BASE, buffer, 1, FINISH);
    r->config = buffer[0];

    buffer[0] = READ_TEMP;
    t = i2c_write_blocking(I2C, ADDR_BASE, buffer, 1, CONTINUE);
    t = i2c_read_blocking(I2C, ADDR_BASE, buffer, 2, FINISH);
    r->temp = (buffer[0] << 8) | buffer[1];

    buffer[0] = ACC_TL;
    t = i2c_write_blocking(I2C, ADDR_BASE, buffer, 1, CONTINUE);
    t = i2c_read_blocking(I2C, ADDR_BASE, buffer, 2, FINISH);
    r->tl = (buffer[0] << 8) | buffer[1];

    buffer[0] = ACC_TH;
    t = i2c_write_blocking(I2C, ADDR_BASE, buffer, 1, CONTINUE);
    t = i2c_read_blocking(I2C, ADDR_BASE, buffer, 2, FINISH);
    r->th = (buffer[0] << 8) | buffer[1];
}

void ds1631_start(void)
{
    volatile int t; // for debugging
    unsigned char buffer[1];

    buffer[0] = START_CONVERT;
    t = i2c_write_blocking(I2C, ADDR_BASE, buffer, 1, FINISH);
}

void ds1631_stop(void)
{
    volatile int t; // for debugging
    unsigned char buffer[1];

    buffer[0] = STOP_CONVERT;
    t = i2c_write_blocking(I2C, ADDR_BASE, buffer, 1, FINISH);
}

double ds1631_get_temp(void)
{
    volatile int t; // for debugging
    unsigned char buffer[2];
    double result;
    signed short i;
    buffer[0] = READ_TEMP;
    t = i2c_write_blocking(I2C, ADDR_BASE, buffer, 1, CONTINUE);
    t = i2c_read_blocking(I2C, ADDR_BASE, buffer, 2, FINISH);
    i = (signed short) (buffer[0] << 8) | buffer[1];
    result = (double) i / 256.0;
    return result;
}

void ds1631_write_borders(double dtl, double dth)
{
    volatile int t; // for debugging
    unsigned char buffer[6];
    signed short ti;

    ti = (signed short) (dtl * 256.0);
    buffer[0] = ACC_TL;
    buffer[1] = ti >> 8;
    buffer[2] = ti;
    t = i2c_write_blocking(I2C, ADDR_BASE, buffer, 3, FINISH);

    ti = (signed short) (dth * 256.0);
    buffer[0] = ACC_TH;
    buffer[1] = ti >> 8;
    buffer[2] = ti;
    t = i2c_write_blocking(I2C, ADDR_BASE, buffer, 3, FINISH);
}

void ds1631_reset(void)
{
    unsigned char buffer[2];
    buffer[0] = SW_POR;
    i2c_write_blocking(I2C, ADDR_BASE, buffer, 1, FINISH);

    i2c_deinit(I2C);
    i2c_init(I2C, CLK);
}

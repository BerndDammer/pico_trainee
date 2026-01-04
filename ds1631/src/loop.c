#include "loop.h"
#include "blinker.h"
#include "ds1631.h"

#include <stdio.h>
#include "pico/stdlib.h"

#define CONSOLE_TIMEOUT 2000000

void menu(void)
{
    printf("------------------------------------\n");
    printf("I2C Test\n");
    printf("a check addr\n");
    printf("1 Start\n");
    printf("0 Stop\n");
    printf("r Read All Regs\n");
    printf("p Power on reset\n");
    printf("8 Write Bordersy\n");
    printf("9 Write Bordersy\n");
    printf("press key to select\n");
    printf("------------------------------------\n");
}

void loop(void)
{
    volatile int c; // make visible in debugger; avoid optimize out
    int counter = 0;
    double f = 1.0;

    menu();
    ds1631_init();

    for (;;)
    {
        c = getchar_timeout_us(CONSOLE_TIMEOUT);
        blinker_toggle();

        if (c == PICO_ERROR_TIMEOUT)
        {
            printf("I2C Test Loop Counter %i   Temp %f\n", counter, ds1631_get_temp());
            counter++;
        }
        else
        {
            switch (c)
            {
            case 'a':
                ds1631_check_addr();
                break;
            case '0':
                ds1631_stop();
                break;
            case '1':
                ds1631_start();
                break;
            case 'r':
            {
                ds1631_regs r;
                ds1631_get_regs(&r);
                printf("Config  %04X\n", r.config);
                printf("Temp  %08X\n", r.temp);
                printf("Th  %08X\n", r.th);
                printf("Tl  %08X\n", r.tl);
            }
                break;
            case '8':
                ds1631_write_borders(20.0625, 32.95);
                break;
            case '9':
                ds1631_write_borders(64.5, 90.25);
                break;
            case 'p':
                ds1631_reset();
                break;
            case ' ':
            default:
                menu();
                break;
            }
        }
    }
}

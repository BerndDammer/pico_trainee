#include "loop.h"
#include "blinker.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"

#include <time.h>
#include <stdlib.h>
#include "spi.h"

#define CONSOLE_TIMEOUT 1000000

void menu(void)
{
    printf("------------------------------------\n");
    printf("1 set crc polynom 0X8001\n");
    printf("2 set crc polynom 0X8005\n");
    printf("3 set crc polynom 0X8007\n");
    printf("8 set crc polynom 0X0000\n");
    printf("i toggle increment\n");
    printf("r randomizing shift\n");
    printf("0 shift\n");
    printf("press key to select\n");
    printf("------------------------------------\n");
}

void loop(void)
{
    int counter = 0;
    volatile int c; // make visible in debugger; avoid optimize out
    srand(time(0));

    menu();
    crcnoise_init();
    for (;;)
    {
        c = getchar_timeout_us(CONSOLE_TIMEOUT);
        blinker_toggle();

        if (c == PICO_ERROR_TIMEOUT)
        {
            printf("Loop Counter %i\n", counter);
            counter++;
        }
        else
        {
            switch (c)
            {
            case '1':
                crcnoise_set_polynom( 0x8001 );
                break;
            case '2':
                crcnoise_set_polynom( 0x8005 );
                break;
            case '3':
                crcnoise_set_polynom( 0x8007 );
                break;
            case '4':
                break;
            case '5':
                break;
            case '6':
                break;
            case '7':
                break;
            case '8':
                crcnoise_set_polynom( 0x0000 );
                break;
            case 'r':
                crcnoise_set_shift(rand());
                break;
            case 'i':
                crcnoise_toggle_increment();
                break;
            case ' ':
            case '0':
                crcnoise_set_shift(0);
                break;
            default:
                menu();
                break;
            }
        }
    }
}

//#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/watchdog.h"

#include "reset_fix_mem.h"

#include "loop.h"
#include "jsontest.h"
#include "memtool.h"
#include "StartupCheck.h"

#include "pico/malloc.h"

#define CONSOLE_TIMEOUT 1000000

void menu(void)
{
    printf("------------------------------------\n");
    printf("Generated: __TIME__ %s  __DATE__ %s\n", __TIME__, __DATE__);
    printf("press key to restart\n");
    printf("1 read bad address\n");
    printf("m memtool dump\n");
    printf("f show fix mem\n");
    printf("p cpp test\n");
    printf("s show startup info\n");
    printf("v read forbidden ram area\n");
    printf("z division by zero\n");
    printf("w watchdog reboot after 3 sec\n");
    printf("a PendSV\n");
    printf("b Systick\n");
    printf("c cpp uncatched trow\n");
    printf("d svc supervisor call\n");
    printf("n nmi\n");
    printf("space show menu\n");
    printf("------------------------------------\n");
}

void doTheTest(void)
{
    test();
}

volatile int a, b, cc;
void loop(void)
{
    volatile int counter = 0;
    volatile int c;

    for (;;)
    {
        c = getchar_timeout_us(CONSOLE_TIMEOUT);

        if (c == PICO_ERROR_TIMEOUT)
        {
            counter++;
        }
        else
        {
            switch (c)
            {
            case '1':
                c = *(volatile int*) 0xbadcafe0;
                break;
            case 'a':
                force_pendsv();
                break;
            case 'b':
                force_systick();
                break;
            case 'c':
#ifdef MISSING            
                provocate_uncatched_throw();
#endif
                break;
            case 'd':
                __asm(" SVC 0X00 ");
                break;
            case 's':
                startup_check_dump();
                break;
            case 'f':
                printf("resetfix_counter %i\n", resetfix_counter);
                puts("********************************************************\n");
                puts(reset_buffer);
                puts("********************************************************\n");
                break;
            case 'm':
                dump_memory();
                break;
            case 'z':
                a = 0;
                b = 0;
                cc = a / b;
                break;
            case 'v':
                c = *(int*) 0x25042000;
                *(int*) 0x27042000 = 0xFFFFAAAA;
                break;
            case 'w':
                puts(" ... 3 2 1 ... baem ....");
                watchdog_reboot(0, 0, 3000); // don't like pointer NULL
                break;
            case 'n':
                force_nmi();
                break;
            case 'p':
#ifdef MISSING            
                doTheTest();
#endif                
                break;
            case ' ':
            case '0':
            default:
                menu();
                break;
            }
        }
    }
}
//////////////////////////////
/// TODO: how to provocate non init ISR ... where does it go

#include <stdio.h>
#include "pico/stdlib.h"

#include "pdp11_defs.h"

int main()
{
    stdio_init_all();
    
    cpu_set_boot(0);
    
    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}

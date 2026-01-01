#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"

#include "blinker.h"

int main()
{

    stdio_init_all();

    blinker_main();

    puts("Main Lost");

    return 0;
}

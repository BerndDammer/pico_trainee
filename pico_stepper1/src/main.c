#include "timer.h"
#include "loop_main.h"
#include "blinker.h"
#include "motor.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"


int main()
{
    stdio_init_all();

    timer_init();
    blinker_init();

    loop_main();

    return 0;
}

#include "loop.h"
#include "StartupCheck.h"

//#include <stdio.h>
#include "pico/stdlib.h"


int main()
{
    stdio_init_all();
    startup_check();
    loop();

    return 0;
}

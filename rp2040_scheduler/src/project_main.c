#include "project_main.h"
#include "rp2040_scheduler.h"
#include "thread_list.h"

#include "pico/stdlib.h"


void project_core0_main(void)
{
    stdio_init_all();
}

void project_core1_main(void)
{
    // init hardware for core1 NVIC
}

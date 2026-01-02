#include <stdio.h>
#include "pico/stdlib.h"
//#include "hardware/timer.h"
//#include "pico/cyw43_arch.h"

#include "async_main.h"

#pragma NORETURN
void main()
{
	stdio_init_all();

	puts("\nEntry");
	async_main_loop();

}

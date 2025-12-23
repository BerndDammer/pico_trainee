#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/exception.h"

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}

extern void dummy(void);
extern void SVC_Handler(void);
extern void SVC_Handler_TST2TST(void);
int main()
{
    stdio_init_all();

    // Timer example code - This example fires off the callback after 2000ms
    add_alarm_in_ms(2000, alarm_callback, NULL, false);
    // For more examples of timer use see https://github.com/raspberrypi/pico-examples/tree/master/timer

    dummy();

    exception_set_exclusive_handler(SVCALL_EXCEPTION, SVC_Handler_TST2TST);
    exception_set_exclusive_handler(SVCALL_EXCEPTION, SVC_Handler);

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}

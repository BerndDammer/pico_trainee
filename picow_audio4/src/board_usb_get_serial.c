#include "board_usb_get_serial.h"

#include <stdio.h>


uint8_t board_usb_get_serial(uint16_t *buffer, int max)
{
	printf("board_usb_get_serial\n");
	buffer[0] = 0X33;
	buffer[1] = 0X34;
	buffer[2] = 0X33;
	return 3;
}

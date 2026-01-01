/*
 * usb_device_cdc.c
 *
 *  callbacks 4 midi
 *
 *
 *  Created on: 09.10.2023
 */

#include "tusb.h"
#include "usb_device_midi.h"

// This include shows the necessary callbacks
// XXX cannot include
//#include "midi_device.h"

void tud_midi_rx_cb(uint8_t itf)
{
	uint8_t packet[4];

	while (tud_midi_available() > 0)
	{
		if (tud_midi_packet_read(packet))
		{
			printf("%02X %02X %02X %02X\n", //
					packet[0], packet[1], packet[2], packet[3]);
		}
		else
		{
			printf("false at read\n");
		}
	}
}


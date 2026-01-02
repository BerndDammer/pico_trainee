/*
 * usb_device_cdc.c
 *
 *  callbacks 4 cdc
 *
 *
 *  Created on: 09.10.2023
 */


#include "tusb.h"

// This include shows the necessary callbacks
// XXX cannot include
//#include "cdc_device.h"





// Invoked when received new data
void tud_cdc_rx_cb(uint8_t itf)
{
	uint8_t buf[64];
	uint32_t count;

	if (tud_cdc_n_available(itf) > 0) // data is available
	{
		count = tud_cdc_n_read(itf, buf, sizeof(buf));

		for (int i = 0; i < count; i++) {
			buf[i] ^= 0x40;
		}
		tud_cdc_n_write(itf, buf, count);
		tud_cdc_n_write_flush(itf);
	}
}

// Invoked when received `wanted_char`
void tud_cdc_rx_wanted_cb(uint8_t itf, char wanted_char)
{
	// nothing to do
}

// Invoked when a TX is complete and therefore space becomes available in TX buffer
void tud_cdc_tx_complete_cb(uint8_t itf)
{
	// nothing to do

}

// Invoked when line state DTR & RTS are changed via SET_CONTROL_LINE_STATE
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
	// nothing to do
}

// Invoked when line coding is change via SET_LINE_CODING
void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const* p_line_coding)
{
	(void)p_line_coding -> bit_rate;
}

// Invoked when received send break
void tud_cdc_send_break_cb(uint8_t itf, uint16_t duration_ms)
{
	// nothing to do
}


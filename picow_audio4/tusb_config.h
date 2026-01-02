// TODO What is necessary
#ifndef _TUSB_CONFIG_H
#define _TUSB_CONFIG_H

#define CFG_TUSB_MCU OPT_MCU_RP2040

// warning redifined
//#define CFG_TUSB_OS  OPT_OS_FREERTOS

#define BOARD_TUD_RHPORT      0

#define CFG_TUSB_DEBUG        0

#define CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_ALIGN          __attribute__ ((aligned(4)))

// 12 MHz
#define BOARD_DEVICE_RHPORT_SPEED   OPT_MODE_FULL_SPEED

#define CFG_TUSB_RHPORT0_MODE      (OPT_MODE_DEVICE | BOARD_DEVICE_RHPORT_SPEED)


#define CFG_TUSB_RHPORTx_MODE

// Enable Device stack
#define CFG_TUD_ENABLED       1

#define CFG_TUD_ENDPOINT0_SIZE    64

//------------- CLASS -------------//
#define CFG_TUD_CDC               2
#define CFG_TUD_MSC               0
#define CFG_TUD_HID               0
#define CFG_TUD_MIDI              0
#define CFG_TUD_VENDOR            0



// CDC FIFO size of TX and RX
#define CFG_TUD_CDC_RX_BUFSIZE   (64)
#define CFG_TUD_CDC_TX_BUFSIZE   (64)

// CDC Endpoint transfer buffer size, more is faster
#define CFG_TUD_CDC_EP_BUFSIZE   (64)

#endif //_TUSB_CONFIG_H

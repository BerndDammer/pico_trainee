/*
 * task_blinker.h
 *
 *  Created on: 12.06.2023
 *      Author: manni4
 */

#ifndef GLOBAL_SIGNAL_H_
#define GLOBAL_SIGNAL_H_

#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"


#define EVENT_MASK_CONSOLE_CHAR 0X400000
#define EVENT_MASK_FAIL 0X800000
#define EVENT_MASK_CYW43_INIT 0X000001
#define EVENT_MASK_CONNECTED 0X000002
#define EVENT_MASK_HEARTBEAT 0X000004

#define ITEMS_PER_QUEUE 4

typedef struct
{
	EventGroupHandle_t mainEventGroup;
}MainEnvironement_t;

typedef void (*CALLEE)(void *);

#define XDELAY 10000

#endif /* GLOBAL_SIGNAL_H_ */

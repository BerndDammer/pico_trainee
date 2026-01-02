/*
 * async_heartbeat.h
 *
 *  Created on: 03.05.2023
 *      Author: manni4
 */

#ifndef ASYNC_HEARTBEAT_H_
#define ASYNC_HEARTBEAT_H_

#include "can.h"
#include "pico/async_context.h"
#include "pico/async_context_poll.h"


async_context_t *async_heartbeat_init(void);
void can_incomming(can_msg_t *msg);

#endif /* ASYNC_HEARTBEAT_H_ */

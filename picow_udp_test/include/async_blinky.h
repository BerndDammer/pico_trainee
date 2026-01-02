/*
 * async_blinky.h
 *
 *  Created on: 28.04.2023
 *      Author: manni4
 */

#ifndef ASYNC_BLINKY_H_
#define ASYNC_BLINKY_H_

#include "pico/async_context.h"
#include "pico/async_context_poll.h"

async_context_t* async_blinky_init(void);


#endif /* ASYNC_BLINKY_H_ */

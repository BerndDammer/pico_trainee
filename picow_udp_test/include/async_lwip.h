/*
 * async_lwip.h
 *
 *  Created on: 25.04.2023
 *      Author: manni4
 */

#ifndef ASYNC_LWIP_H_
#define ASYNC_LWIP_H_

#include "can.h"

void async_lwip_init(async_context_t *asc);
void async_lwip_can_send(can_msg_t *msg);



#endif /* ASYNC_LWIP_H_ */

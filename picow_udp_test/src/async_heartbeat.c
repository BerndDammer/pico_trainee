/*
 * async_heartbeat.c
 *
 *  Created on: 03.05.2023
 *      Author: manni4
 */

#define HEARTBEAT_DELAY_MS 800

#include "panic.h"
#include "async_heartbeat.h"
#include "async_lwip.h"
#include "can.h"

#include "pico/async_context.h"
#include "pico/async_context_poll.h"

#include "pico/stdlib.h"
#include "pico/types.h"

async_context_poll_t async_context_heartbeat;
async_at_time_worker_t s_process_heartbeat;

absolute_time_t last_incomming_hertbeat_time;
short last_incomming_counter;
short last_echo;
short my_heartbeat_counter = 1;

void can_incomming(can_msg_t *msg) {
	last_incomming_counter = msg->data[0] | msg->data[1] << 8;
	last_echo = msg->data[2] | msg->data[3] << 8;
	last_incomming_hertbeat_time = get_absolute_time();
}

void process_heartbeat(async_context_t *context,
		struct async_work_on_timeout *worker) {

	can_msg_t msg;

	msg.id = CAN_ID_HEARTBEAT;
	msg.len = 8;
	msg.data[0] = my_heartbeat_counter;
	msg.data[1] = my_heartbeat_counter >> 8;
	msg.data[4] = last_echo;
	msg.data[5] = last_echo >> 8;
	msg.data[6] = 0XCA;
	msg.data[7] = 0XFE;

	if (absolute_time_diff_us( /* */
	get_absolute_time(), /**/
	delayed_by_us(last_incomming_hertbeat_time, HEARTBEAT_VALID_TIMEOUT_US) /**/
	) > 0l) /**/
	{
		msg.data[2] = last_incomming_counter;
		msg.data[3] = last_incomming_counter >> 8;
	} else {
		msg.data[2] = 0XFF;
		msg.data[3] = 0XFF;
	}

	async_lwip_can_send(&msg);
	my_heartbeat_counter++;

	async_context_add_at_time_worker_in_ms(&async_context_heartbeat.core,
			&s_process_heartbeat, HEARTBEAT_DELAY_MS);
}

async_context_t* async_heartbeat_init(void) {
	if (!async_context_poll_init_with_defaults(&async_context_heartbeat)) {
		app_panic("Async context heartbeat init fail");
		return NULL ;
	}

	s_process_heartbeat.do_work = process_heartbeat;
	async_context_add_at_time_worker_in_ms(&async_context_heartbeat.core,
			&s_process_heartbeat, HEARTBEAT_DELAY_MS);

	return &async_context_heartbeat.core;

}


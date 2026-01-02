/*
 * async_lwip.c
 *
 *  Created on: 25.04.2023
 *      Author: manni4
 */

/************************
 *
 *
 * TRAP QQQQ ?????
 *
 * lwip and cyw43 must be initialized with the same
 * async_context ???
 *
 * TRAP:
 *
 * multicast address 224.0.0.1 must NOT be joined
 *
 *
 *
 */

#include "pico/stdlib.h"
#include "pico/types.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/igmp.h"

#include "pico/async_context.h"
#include "pico/async_context_poll.h"
#include "pico/lwip_nosys.h"

#include "panic.h"
#include "async_lwip.h"
#include "async_heartbeat.h"

#define UDP_PORT_RECEIVE 50000
#define UDP_PORT_TRANS   50100
#define HEARTBEAT_TIMEOUT_MS 1000

async_context_t *async_context_lwip;

ip4_addr_t multicast_destination;
struct udp_pcb *multicast_receive_socket;

int reciece_counter = 0;

// thats magic
extern struct netif gnetif;

void recCallBack(void *arg, struct udp_pcb *pcb, struct pbuf *p,
		const ip_addr_t *addr, u16_t port)
{

	if (p->len == sizeof(can_msg_t))
	{
		can_incomming((can_msg_t*) (p->payload));
	}
	else
	{
		printf("\np ->len %i", p->len);
	}
	pbuf_free(p);
}

void async_lwip_can_send(can_msg_t *msg)
{
	struct pbuf *p;
	p = pbuf_alloc(PBUF_TRANSPORT, sizeof(can_msg_t), PBUF_RAM);
	memcpy(p->payload, msg, sizeof(can_msg_t));

	udp_sendto(multicast_receive_socket, p, &multicast_destination,
	UDP_PORT_TRANS); //send a multicast packet
	pbuf_free(p);
}

void async_lwip_init(async_context_t *asc)
{
	bool success;
	async_context_lwip = asc;

//	IP4_ADDR(&multicast_destination, 224, 0, 0, 1); //Multicast IP address.
	// TODO better logic
//	IP4_ADDR(&multicast_destination, 192, 168, 178, 255); //local broadcast
	IP4_ADDR(&multicast_destination, 255, 255, 255, 255); //non routing local broadcast

	success = lwip_nosys_init(async_context_lwip);
	if (!success)
	{
		app_panic("lwip init fail");
	}

	{
		multicast_receive_socket = udp_new();
		udp_bind(multicast_receive_socket, NULL, UDP_PORT_RECEIVE); //to allow receiving multicast
		udp_recv(multicast_receive_socket, recCallBack, NULL); //recCallBack is the callback function that will be called every time you    receive multicast
	}
	//--- add multicast receive
	//igmp_init();
}


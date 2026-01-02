/*
 * can.h
 *
 *  Created on: 04.05.2023
 *      Author: manni4
 */

#ifndef CAN_H_
#define CAN_H_

#define MASK_IDLEN 0X80000000
#define VAL_IDLEN_29 0X80000000
#define VAL_IDLEN_11 0X00000000

typedef struct
{
	unsigned int id;
	unsigned int len;
	unsigned char data[8];
}can_msg_t;

#define CAN_ID_HEARTBEAT 0X80045670
#define HEARTBEAT_VALID_TIMEOUT_US (long)(2500*1000)

#endif /* CAN_H_ */

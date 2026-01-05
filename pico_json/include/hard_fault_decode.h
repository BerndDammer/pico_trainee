/*
 * hard_fault_decode.h
 *
 *  Created on: 15.03.2023
 *      Author: manni4
 */

#ifndef HARD_FAULT_DECODE_H_
#define HARD_FAULT_DECODE_H_

//#include "pico/types.h"

void myHardfaultException( //
        unsigned int r0,  //
        unsigned int r1,  //
        unsigned int r2,  //
        unsigned int r3,  //
        unsigned int r12, //
        void *lr,         //
        void *pc,         //
        unsigned int xpsr //
        );

#endif /* HARD_FAULT_DECODE_H_ */

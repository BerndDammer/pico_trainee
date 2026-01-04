/*
 * spi_lights.h
 *
 *  Created on: 09.03.2023
 *      Author: manni4
 */

#ifndef SPI_H_
#define SPI_H_

void crcnoise_init(void);
void crcnoise_set_shift(int shift);
void crcnoise_set_polynom(int p);
void crcnoise_toggle_increment(void);


#endif /* SPI_H_ */

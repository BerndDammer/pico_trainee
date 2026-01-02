/*
 * parameter.h
 *
 *  Created on: 05.03.2023
 *      Author: manni4
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

#define FLASH_TOP 0x10200000
#define FLASH_PAGE 4096
//#define PARA_BASE 0x101FF000
#define PARA_BASE (FLASH_TOP - FLASH_PAGE)
#define PARA_SSID (unsigned char*)PARA_BASE
#define PARA_PWD (unsigned char*)(PARA_BASE+64)

#endif /* PARAMETER_H_ */

/*
 * ds1307.h
 *
 *  Created on: 02/09/2012
 *      Author: Luiz
 */

#ifndef DS1307_H_
#define DS1307_H_

#include <stdint.h>

#define DS1307_OUT_LOW		0x00
#define DS1307_OUT_HIGH		0x80

#define DS1307_SQWE_ENABLE	0x10
#define DS1307_SQWE_DISABLE	0x00

#define DS1307_RS_1HZ		0x00
#define DS1307_RS_4096HZ	0x01
#define DS1307_RS_8192HZ	0x02
#define DS1307_RS_32768HZ	0x03

typedef struct {
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t wkday;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} ds1307_datetime;

void DS1307_GetDateTime( ds1307_datetime* dt );

void DS1307_SetDateTime( ds1307_datetime* dt );

void DS1307_SetControl( uint8_t ctrl );

void DS1307_WriteRam( uint8_t* buf, uint8_t addr, uint8_t length );

void DS1307_ReadRam( uint8_t* dst, uint8_t addr, uint8_t length );

#endif /* DS1307_H_ */

/*
 * bcd.h
 *
 *  Created on: 29/08/2012
 *      Author: Luiz
 */

#ifndef BCD_H_
#define BCD_H_

#include <stdint.h>

#define BCD_TENS( x ) ( ( x ) >> 4 )
#define BCD_ONES( x ) ( ( x ) & 0x0F )

uint8_t bcd_incr( uint8_t x );
uint8_t bcd_decr( uint8_t x );

#endif /* BCD_H_ */

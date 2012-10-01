/*
 * bcd.c
 *
 *  Created on: 30/09/2012
 *      Author: Luiz
 */

#include "bcd.h"

uint8_t bcd_incr( uint8_t x ) {
	x += 0x01;
	// carry?
	if ( BCD_ONES( x ) == 0x0A ) {
		// overflow?
		if ( BCD_TENS( x ) == 0x09 ) {
			x = 0x00;
		} else {
			x &= 0xF0;
			x += 0x10;
		}
	}
	return x;
}

uint8_t bcd_decr( uint8_t x ) {
	// underflow?
	if ( x == 0x00 ) {
		x = 0x99;
	} else {
		// borrow?
		if ( BCD_ONES( x ) == 0x00 ) {
			x -= 0x10;
			x |= 0x09;
		} else {
			x -= 0x01;
		}
	}
	return x;
}

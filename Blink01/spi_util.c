/*
 * spi_util.c
 *
 *  Created on: 12/08/2012
 *      Author: Luiz
 */

#include <avr/io.h>
#include "spi_util.h"

void spi_wait() {
	while ( !( SPSR & _BV( SPIF ) ) ) ;
}

uint8_t spi_transfer( uint8_t out ) {
	SPDR = out;
	spi_wait();
	return SPDR;
}

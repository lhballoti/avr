/*
 * max7219.c
 *
 *  Created on: 12/08/2012
 *      Author: Luiz
 */

#include <avr/io.h>

#include "max7219.h"
#include "spi_util.h"

#ifndef MAX7219_SS_PIN
#error "MAX7219 slave select pin not defined"
#endif
#ifndef MAX7219_SS_PORT
#error "MAX7219 slave select port not defined"
#endif

void MAX7219_Send( uint8_t addr, uint8_t data ) {
	MAX7219_SS_PORT &= ~_BV( MAX7219_SS_PIN );
	spi_transfer( addr );
	spi_transfer( data );
	MAX7219_SS_PORT |= _BV( MAX7219_SS_PIN );
}

void MAX7219_SetScanLimit( uint8_t scan ) {
	MAX7219_Send( MAX7219_ADDR_SCLM, scan );
}

void MAX7219_SetDecodeMode( uint8_t dec ) {
	MAX7219_Send( MAX7219_ADDR_DECM, dec );
}

void MAX7219_SetIntensity( uint8_t inten ) {
	MAX7219_Send( MAX7219_ADDR_INTN, inten );
}

void MAX7219_SetDigit( uint8_t digit, uint8_t value ) {
	MAX7219_Send( MAX7219_ADDR_DIG0 + digit, value );
}

void MAX7219_Start() {
	MAX7219_Send( MAX7219_ADDR_SHDN, 1 );
}

void MAX7219_Shutdown() {
	MAX7219_Send( MAX7219_ADDR_SHDN, 0 );
}

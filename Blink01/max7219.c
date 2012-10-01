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

#define MAX7219_ADDR_NOOP 0x00
#define MAX7219_ADDR_DIG0 0x01
#define MAX7219_ADDR_DIG1 0x02
#define MAX7219_ADDR_DIG2 0x03
#define MAX7219_ADDR_DIG3 0x04
#define MAX7219_ADDR_DIG4 0x05
#define MAX7219_ADDR_DIG5 0x06
#define MAX7219_ADDR_DIG6 0x07
#define MAX7219_ADDR_DIG7 0x08
#define MAX7219_ADDR_DECM 0x09
#define MAX7219_ADDR_INTN 0x0A
#define MAX7219_ADDR_SCLM 0x0B
#define MAX7219_ADDR_SHDN 0x0C
#define MAX7219_ADDR_TEST 0x0F

static void MAX7219_Send( uint8_t addr, uint8_t data ) {
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

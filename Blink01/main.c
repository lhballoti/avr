/*
 * main.c
 *
 *  Created on: 07/08/2012
 *      Author: Luiz
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "spi_util.h"
#include "max7219.h"

#define LEDS_SS_DDB DDD5
#define LEDS_SS PORTD5

int main() {
	uint8_t leds = 1;
	uint16_t count = 0;
	uint8_t digits[4];

	// set up PB1, SCK and MOSI as outupts
	// PB2 is the SS pin, must be output when operating as master
	DDRB |= _BV( DDB1 ) | _BV( DDB5 ) | _BV( DDB3 ) | _BV( DDB2 );
	PORTB &= ~_BV( PORTB1 );

	// 595 and display slave selects
	DDRD |= _BV( LEDS_SS_DDB ) | _BV( DDD6 );
	PORTD |= _BV( LEDS_SS ) | _BV( PORTD6 );

	// init SPI
	SPCR = _BV( SPE ) | _BV( MSTR );
	uint8_t x = SPSR;
	x = SPDR;

	MAX7219_SetScanLimit( 0x3 );
	MAX7219_SetDecodeMode( 0xF );
	MAX7219_SetIntensity( 0xB );
	MAX7219_Start();

	while ( 1 ) {
//		PORTB ^= _BV( PINB1 );

		PORTD &= ~_BV( LEDS_SS );
		spi_transfer( leds );
		PORTD |= _BV( LEDS_SS );

		leds <<= 1;
		if ( !leds ) {
			leds = 1;
		}

		int temp = count;
		for ( int i = 3; i >= 0; i-- ) {
			digits[i] = temp % 10;
			temp /= 10;
		}
		for ( int i = 0; i < 4; i++ ) {
			MAX7219_SetDigit( i, digits[i] );
		}
		count++;
		if ( count == 10000 ) {
			count = 0;
		}

		_delay_ms( 1000 );
	}
}

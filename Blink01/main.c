/*
 * main.c
 *
 *  Created on: 07/08/2012
 *      Author: Luiz
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdint.h>
#include <util/twi.h>

#include "spi_util.h"
#include "i2c_util.h"
#include "max7219.h"
#include "ds1307.h"

#define LEDS_SS_DDB DDD5
#define LEDS_SS PORTD5

void sendLeds( uint8_t leds ) {
	PORTD &= ~_BV( LEDS_SS );
	spi_transfer( leds );
	PORTD |= _BV( LEDS_SS );
}

// empty timer interrupt
EMPTY_INTERRUPT( TIMER0_COMPA_vect );

int main() {
	uint8_t leds = 1;
//	uint16_t count = 0;
//	uint8_t digits[4];
	uint8_t oldHour = -1, oldMin = -1, oldSec = -1;
	ds1307_datetime dt;

	// set up PB1, SCK and MOSI as outupts
	// PB2 is the SS pin, must be output when operating as master
	DDRB |= _BV( DDB1 ) | _BV( DDB5 ) | _BV( DDB3 ) | _BV( DDB2 );
	PORTB &= ~_BV( PORTB1 );

	// 595 and display slave selects (outputs), timer clock (input)
	DDRD |= _BV( LEDS_SS_DDB ) | _BV( DDD6 );
	PORTD |= _BV( LEDS_SS ) | _BV( PORTD6 );
	DDRD &= ~_BV( DDD4 );

	// I2C pins
	DDRC &= ~( _BV( DDC4 ) | _BV( DDC5 ) );
	PORTC |= _BV( PORTC4 ) | _BV( PORTC5 );

	// init SPI
	SPCR = _BV( SPE ) | _BV( MSTR );
	uint8_t x = SPSR;
	x = SPDR;

	i2c_init();

	MAX7219_SetScanLimit( 0x3 );
	MAX7219_SetDecodeMode( 0xF );
	MAX7219_SetIntensity( 0xB );
	MAX7219_Start();

	MAX7219_SetDigit( 0, 9 );
	MAX7219_SetDigit( 1, 9 );
	MAX7219_SetDigit( 2, 9 );
	MAX7219_SetDigit( 3, 9 );

	DS1307_SetControl( DS1307_RS_4096HZ | DS1307_SQWE_ENABLE );

	sendLeds( 0x55 );

	// set up timer (fires @ 64 Hz)
	// OC0A and OC0B disconnected, CTC mode
	TCCR0A = 2;
	// TCCR0B will be written last, since it is used to enable the timer
	// 64 Hz frequency using external 4096 Hz clock
	OCR0A = 63;
	TCNT0 = 0;
	// enable output compare A interrupt
	TIMSK0 |= _BV( OCIE0A );
	sei();
	// start timer using external clock
	TCCR0B = 7;

	while ( 1 ) {

		sleep_enable();
		sleep_cpu();
		sleep_disable();

//		PORTB ^= _BV( PINB1 );

//		dt.hour = 0xFF;
//		dt.minute = 0xFF;

		DS1307_GetDateTime( &dt );
		if ( dt.hour != oldHour || dt.minute != oldMin ) {
			MAX7219_SetDigit( 0, ( dt.hour & 0x30 ) >> 4 );
			MAX7219_SetDigit( 1, dt.hour & 0x0F );
			MAX7219_SetDigit( 2, ( dt.minute & 0x70 ) >> 4 );
			MAX7219_SetDigit( 3, dt.minute & 0x0F );

			oldHour = dt.hour;
			oldMin = dt.minute;
		}

		if ( dt.second != oldSec ) {
			sendLeds( leds );

			leds <<= 1;
			if ( !leds ) {
				leds = 1;
			}

			oldSec = dt.second;
		}

//		int temp = count;
//		for ( int i = 3; i >= 0; i-- ) {
//			digits[i] = temp % 10;
//			temp /= 10;
//		}
//		for ( int i = 0; i < 4; i++ ) {
//			MAX7219_SetDigit( i, digits[i] );
//		}
//		count++;
//		if ( count == 10000 ) {
//			count = 0;
//		}

//		_delay_ms( 1000 );
	}
}

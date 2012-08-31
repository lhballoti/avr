/*
 * main.c
 *
 *  Created on: 07/08/2012
 *      Author: Luiz
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <util/twi.h>

#include "spi_util.h"
#include "max7219.h"
#include "i2c_util.h"

#define LEDS_SS_DDB DDD5
#define LEDS_SS PORTD5

void sendLeds( uint8_t leds ) {
	PORTD &= ~_BV( LEDS_SS );
	spi_transfer( leds );
	PORTD |= _BV( LEDS_SS );
}

int main() {
//	uint8_t leds = 1;
//	uint16_t count = 0;
//	uint8_t digits[4];
	uint8_t rtc_data[7];

	// set up PB1, SCK and MOSI as outupts
	// PB2 is the SS pin, must be output when operating as master
	DDRB |= _BV( DDB1 ) | _BV( DDB5 ) | _BV( DDB3 ) | _BV( DDB2 );
	PORTB &= ~_BV( PORTB1 );

	// 595 and display slave selects
	DDRD |= _BV( LEDS_SS_DDB ) | _BV( DDD6 );
	PORTD |= _BV( LEDS_SS ) | _BV( PORTD6 );

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

	while ( 1 ) {
//		PORTB ^= _BV( PINB1 );

//		sendLeds( leds );

//		leds <<= 1;
//		if ( !leds ) {
//			leds = 1;
//		}

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

		sendLeds( 0b00000000 );

		// write to RTC
		i2c_begin( 0b1101000, 0 );

		sendLeds( 0b10000000 );

		// set register pointer to 0
		i2c_write( 0, 1 );

//		sendLeds( 0b11000000 );

		// read from RTC
		i2c_begin( 0b1101000, 1 );

//		TWCR = _BV( TWINT ) | _BV( TWSTA ) | _BV( TWEN );
//		MAX7219_SetDigit( 3, 1 );
//		i2c_wait();
//		MAX7219_SetDigit( 3, 2 );
//		if ( TW_STATUS != TW_START ) {
//			MAX7219_SetDigit( 3, 3 );
//		} else {
//			MAX7219_SetDigit( 3, 4 );
//		}

		// transmit slave address and data direction
//		TWDR = ( 0b1101000 << 1 ) | 0x01;
//		MAX7219_SetDigit( 3, 5 );
//		TWCR = _BV( TWINT ) | _BV( TWEN );
//		MAX7219_SetDigit( 3, 6 );
//		i2c_wait();
//		MAX7219_SetDigit( 3, 7 );
//		if ( TW_STATUS != TW_MR_SLA_ACK ) {
//			i2c_stop();
//			MAX7219_SetDigit( 3, 8 );
////			return -1;
//		}


//		sendLeds( 0b11100000 );

		// read 6 bytes
		for ( int i = 0; i < 6; i++ ) {
//			rtc_data[i] = i2c_read( 0 );

			uint8_t twcr = _BV( TWINT ) | _BV( TWEN ) | _BV( TWEA );
//			if ( stop ) {
//				twcr = _BV( TWINT ) | _BV( TWEN );
//			}
//			MAX7219_SetDigit( 3, 1 );

			TWCR = twcr;
//			MAX7219_SetDigit( 3, 2 );

			i2c_wait();
//			MAX7219_SetDigit( 3, 3 );

//			uint8_t data = TWDR;
			rtc_data[i] = TWDR;
//			MAX7219_SetDigit( 3, 4 );

			if ( TW_STATUS == TW_MR_DATA_NACK ) {
				i2c_stop();
			}
//			MAX7219_SetDigit( 3, 5 );
		}

		sendLeds( 0b11110000 );

		// read 7th byte and stop
		rtc_data[6] = i2c_read( 1 );

		sendLeds( 0b11111000 );

		MAX7219_SetDigit( 0, ( rtc_data[2] & 0x30 ) >> 4 );
		MAX7219_SetDigit( 1, rtc_data[2] & 0x0F );
		MAX7219_SetDigit( 2, ( rtc_data[1] & 0x70 ) >> 4 );
		MAX7219_SetDigit( 3, rtc_data[1] & 0x0F );

		_delay_ms( 1000 );
	}
}

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
#include "alarm_clock.h"
#include "button_matrix.h"

#define BUTTON_ADJUST	8
#define BUTTON_NEXT		7
#define BUTTON_INCR		6
#define BUTTON_DECR		5

#define UI_STATE_IDLE	0
#define UI_STATE_WKDAY	1
#define UI_STATE_HOUR	2
#define UI_STATE_MINUTE	3

// empty timer interrupt
EMPTY_INTERRUPT( TIMER0_COMPA_vect );
//ISR( TIMER0_COMPA_vect ) {
//	PORTB ^= _BV( PORTB1 );
//}

//ISR( BADISR_vect ) {
//	PORTB |= _BV( PORTB1 );
//}

int main() {
	// set up PB1, SCK and MOSI as outupts
	// PB2 is the SS pin, must be output when operating as master
	DDRB |= _BV( DDB1 ) | _BV( DDB5 ) | _BV( DDB3 ) | _BV( DDB2 );
	PORTB &= ~_BV( PORTB1 );

	// 595 and display slave selects (outputs), timer clock (input)
	DDRD |= _BV( DDD5 ) | _BV( DDD6 );
	PORTD |= _BV( PORTD5 ) | _BV( PORTD6 );
	DDRD &= ~_BV( DDD4 );
	// enable internal pull-up for clock input (RTC output is open-drain)
	PORTD |= _BV( DDD4 );

	// I2C pins
	DDRC &= ~( _BV( DDC4 ) | _BV( DDC5 ) );
	PORTC |= _BV( PORTC4 ) | _BV( PORTC5 );

	// init SPI
	SPCR = _BV( SPE ) | _BV( MSTR );
	uint8_t x = SPSR;
//	x = SPDR;
	PORTD &= ~_BV( PORTD5 );
	spi_transfer( 0 );
	PORTD |= _BV( PORTD5 );

	i2c_init();

	ButtonMatrix_Init();

	MAX7219_SetScanLimit( 0x3 );
	MAX7219_SetDecodeMode( 0xF );
	MAX7219_SetIntensity( 0xB );
	MAX7219_Start();

	DS1307_SetControl( DS1307_RS_4096HZ | DS1307_SQWE_ENABLE );
	DS1307_Start();

	// set up timer 0 (fires @ 64 Hz)
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

		AlarmClock_Tick();

		if ( ButtonMatrix_stateChanged() ) {

			switch ( ui_state ) {
				case UI_STATE_IDLE:
					if ( ButtonMatrix_isPressed( BUTTON_ADJUST ) ) {
						ui_state = UI_STATE_WKDAY;
						AlarmClock_StartAdjust( ALARM_CLOCK_TIME );
					}
					break;
				case UI_STATE_WKDAY:
				case UI_STATE_HOUR:
				case UI_STATE_MINUTE:
					if ( ButtonMatrix_wasPressed( BUTTON_INCR ) ) {
						AlarmClock_IncrementField();
					} else if ( ButtonMatrix_wasPressed( BUTTON_DECR ) ) {
						AlarmClock_DecrementField();
					} else if ( ButtonMatrix_wasPressed( BUTTON_NEXT ) ) {
						if ( ui_state < UI_STATE_MINUTE ) {
							ui_state++;
						} else {
							ui_state = UI_STATE_WKDAY;
						}
						AlarmClock_NextField();
					} else if ( ButtonMatrix_wasReleased( BUTTON_ADJUST ) ) {
						AlarmClock_EndAdjust();
						ui_state = UI_STATE_IDLE;
					}
					break;
			}

		}
	}
}

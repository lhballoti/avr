/*
 * alarm_clock.c
 *
 *  Created on: 28/09/2012
 *      Author: Luiz
 */

#include <avr/io.h>
#include <stdint.h>

#include "alarm_clock.h"
#include "spi_util.h"
#include "max7219.h"
#include "ds1307.h"
#include "bcd.h"

#ifndef LEDS_SS_PORT
#error "LEDS slave select port not defined"
#endif
#ifndef LEDS_SS_PIN
#error "LEDS slave select pin not defined"
#endif

#define TICKS_PER_SECOND		64
#define TICKS_PER_HALF_SECOND	( TICKS_PER_SECOND >> 1 )
#define SHOW_FIELD_DELAY		( TICKS_PER_SECOND >> 1 )

#define FIELD_WEEKDAY	0x00
#define FIELD_HOUR		0x01
#define FIELD_MINUTE	0x02
#define FIELD_NONE		0xFF

#define WEEK_NONE		0
#define WEEK_MON		2
#define WEEK_TUE		4
#define WEEK_WED		8
#define WEEK_THU		16
#define WEEK_FRI		32
#define WEEK_SAT		64
#define WEEK_SUN		128
#define WEEK_WEEKDAYS	( WEEK_MON | WEEK_TUE | WEEK_WED | WEEK_THU | WEEK_FRI )
#define WEEK_WEEKENDS	( WEEK_SAT | WEEK_SUN )
#define WEEK_EVERYDAY	( WEEK_MON | WEEK_TUE | WEEK_WED | WEEK_THU | WEEK_FRI | WEEK_SAT | WEEK_SUN )

struct alarm_entry {
	uint8_t enabled;
	uint8_t wkday;
	uint8_t hours;
	uint8_t minutes;
};

const uint8_t weekdays_opts[] = {
	WEEK_MON,
	WEEK_TUE,
	WEEK_WED,
	WEEK_THU,
	WEEK_FRI,
	WEEK_SAT,
	WEEK_SUN,
	WEEK_WEEKDAYS,
	WEEK_WEEKENDS,
	WEEK_EVERYDAY
};

static uint8_t ticks = 0;
static uint8_t is_adjusting = 0;
static uint8_t curr_setting = -1;
static uint8_t show_field_timer = -1;
static uint8_t curr_field = FIELD_NONE;
static uint8_t curr_wkday = WEEK_NONE;
static uint8_t curr_hour = MAX7219_DIGIT_OFF;
static uint8_t curr_min = MAX7219_DIGIT_OFF;

static struct alarm_entry alarm_entries[ALARM_CLOCK_TIMER_2 + 1];

static void sendLeds( uint8_t leds ) {
	LEDS_SS_PORT &= ~_BV( LEDS_SS_PIN );
	spi_transfer( leds );
	LEDS_SS_PORT |= _BV( LEDS_SS_PIN );
}

static void showCurrentField() {
	switch ( curr_field ) {
		case FIELD_WEEKDAY:
			sendLeds( weekdays_opts[curr_wkday] );
			break;
		case FIELD_HOUR:
			MAX7219_SetDigit( 0, BCD_TENS( curr_hour ) );
			MAX7219_SetDigit( 1, BCD_ONES( curr_hour ) );
			break;
		case FIELD_MINUTE:
			MAX7219_SetDigit( 2, BCD_TENS( curr_min ) );
			MAX7219_SetDigit( 3, BCD_ONES( curr_min ) );
			break;
		case FIELD_NONE:
		default:
			break;
	}
}

static void hideCurrentField() {
	switch ( curr_field ) {
		case FIELD_WEEKDAY:
			sendLeds( WEEK_NONE );
			break;
		case FIELD_HOUR:
			MAX7219_SetDigit( 0, MAX7219_DIGIT_OFF );
			MAX7219_SetDigit( 1, MAX7219_DIGIT_OFF );
			break;
		case FIELD_MINUTE:
			MAX7219_SetDigit( 2, MAX7219_DIGIT_OFF );
			MAX7219_SetDigit( 3, MAX7219_DIGIT_OFF );
			break;
		case FIELD_NONE:
		default:
			break;
	}
}

static void loadSetting( int setting ) {
	struct alarm_entry ae = alarm_entries[curr_setting];
	curr_wkday = ae.wkday;
	curr_hour = ae.hours;
	curr_min = ae.minutes;
	sendLeds( weekdays_opts[curr_wkday] );
	MAX7219_SetDigit( 0, BCD_TENS( curr_hour ) );
	MAX7219_SetDigit( 1, BCD_ONES( curr_hour ) );
	MAX7219_SetDigit( 2, BCD_TENS( curr_min ) );
	MAX7219_SetDigit( 3, BCD_ONES( curr_min ) );
}

static void showTime() {
	ds1307_datetime dt;
	DS1307_GetDateTime( &dt );
	uint8_t leds = 1 << dt.wkday;
	leds += dt.second & 0x01;
	sendLeds( leds );
	MAX7219_SetDigit( 0, BCD_TENS( dt.hour & 0x30 ) );
	MAX7219_SetDigit( 1, BCD_ONES( dt.hour ) );
	MAX7219_SetDigit( 2, BCD_TENS( dt.minute & 0x70 ) );
	MAX7219_SetDigit( 3, BCD_ONES( dt.minute ) );
}

void AlarmClock_Tick() {
	ticks++;
	if ( is_adjusting ) {
		// blink current field every half second when adjusting, but not if
		// the timer is running
		if ( ticks == TICKS_PER_HALF_SECOND && !show_field_timer ) {
			hideCurrentField();
		// if the timer is running, decrement it
		} else if ( show_field_timer ) {
			show_field_timer--;
			// expired? hide field
			if ( !show_field_timer ) { hideCurrentField(); }
		}
	}
	if ( ticks == TICKS_PER_SECOND ) {
		ticks = 0;
		if ( is_adjusting ) {
			showCurrentField();
		} else {
			showTime();
		}
	}
}

void AlarmClock_Enable( int setting ) {

}

void AlarmClock_Disable( int setting ) {

}

void AlarmClock_StartAdjust( int setting ) {
	is_adjusting = 1;
	curr_setting = setting;
	curr_field = FIELD_WEEKDAY;
	loadSetting( curr_setting );
}

void AlarmClock_IncrementField() {
	show_field_timer = SHOW_FIELD_DELAY;
	switch ( curr_field ) {
		case FIELD_WEEKDAY:
			curr_wkday++;
			// alarms can be set to more than one day of week
			uint8_t wkmax = ( curr_setting == ALARM_CLOCK_TIME ) ? 6 : sizeof( weekdays_opts );
			if ( curr_wkday >= wkmax ) {
				curr_wkday = 0;
			}
			break;
		case FIELD_HOUR:
			if ( curr_hour == 0x23 ) {
				curr_hour = 0x00;
			} else {
				curr_hour = bcd_incr( curr_hour );
			}
			break;
		case FIELD_MINUTE:
			if ( curr_min == 0x59 ) {
				curr_min = 0x00;
			} else {
				curr_min = bcd_incr( curr_min );
			}
			break;
		default:
			break;
	}
	showCurrentField();
}

void AlarmClock_DecrementField() {
	show_field_timer = SHOW_FIELD_DELAY;
	switch ( curr_field ) {
		case FIELD_WEEKDAY:
			curr_wkday--;
			// alarms can be set to more than one day of week
			if ( curr_wkday == 0xFF ) {
				if ( curr_setting != ALARM_CLOCK_TIME ) {
					curr_wkday = sizeof( weekdays_opts ) - 1;
				} else {
					curr_wkday = 6;
				}
			}
			break;
		case FIELD_HOUR:
			if ( curr_hour == 0x00 ) {
				curr_hour = 0x23;
			} else {
				curr_hour = bcd_decr( curr_hour );
			}
			break;
		case FIELD_MINUTE:
			if ( curr_min == 0x00 ) {
				curr_min = 0x59;
			} else {
				curr_min = bcd_decr( curr_min );
			}
			break;
		default:
			break;
	}
	showCurrentField();
}

void AlarmClock_NextField() {
	showCurrentField();
	curr_field++;
	if ( curr_field > FIELD_MINUTE ) {
		curr_field = FIELD_WEEKDAY;
		showCurrentField();
	}
}

void AlarmClock_EndAdjust() {
	struct alarm_entry* ae = &( alarm_entries[curr_setting] );
	ae->wkday = curr_wkday;
	ae->hours = curr_hour;
	ae->minutes = curr_min;
	curr_setting = -1;
	is_adjusting = 0;
	curr_field = FIELD_NONE;
	showTime();
}

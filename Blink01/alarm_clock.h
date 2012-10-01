/*
 * alarm_clock.h
 *
 *  Created on: 28/09/2012
 *      Author: Luiz
 */

#ifndef __ALARM_CLOCK_H__
#define __ALARM_CLOCK_H__

#define ALARM_CLOCK_TIME		0
#define ALARM_CLOCK_ALARM_0		1
#define ALARM_CLOCK_ALARM_1		2
#define ALARM_CLOCK_ALARM_2		3
#define ALARM_CLOCK_TIMER_0		4
#define ALARM_CLOCK_TIMER_1		5
#define ALARM_CLOCK_TIMER_2		6

#define LEDS_SS_PORT	PORTD
#define LEDS_SS_PIN		PORTD5

void AlarmClock_Tick();

void AlarmClock_Enable( int setting );

void AlarmClock_Disable( int setting );

void AlarmClock_StartAdjust( int setting );

void AlarmClock_IncrementField();

void AlarmClock_DecrementField();

void AlarmClock_NextField();

void AlarmClock_EndAdjust();

#endif /* __ALARM_CLOCK_H__ */

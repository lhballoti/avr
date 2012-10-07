/*
 * max7219.h
 *
 *  Created on: 12/08/2012
 *      Author: Luiz
 */

#ifndef __MAX7219_H__
#define __MAX7219_H__

#define MAX7219_DIGIT_OFF	0x0F

#define MAX7219_SS_PORT PORTD
#define MAX7219_SS_PIN PORTD6

void MAX7219_SetScanLimit( uint8_t scan );

void MAX7219_SetDecodeMode( uint8_t dec );

void MAX7219_SetIntensity( uint8_t inten );

void MAX7219_SetDigit( uint8_t digit, uint8_t value );

void MAX7219_Start();

void MAX7219_Shutdown();

#endif /* __MAX7219_H__ */

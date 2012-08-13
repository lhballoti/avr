/*
 * max7219.h
 *
 *  Created on: 12/08/2012
 *      Author: Luiz
 */

#ifndef MAX7219_H_
#define MAX7219_H_

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

#define MAX7219_SS_PORT PORTD
#define MAX7219_SS_PIN PORTD6

void MAX7219_Send( uint8_t addr, uint8_t data );

void MAX7219_SetScanLimit( uint8_t scan );

void MAX7219_SetDecodeMode( uint8_t dec );

void MAX7219_SetIntensity( uint8_t inten );

void MAX7219_SetDigit( uint8_t digit, uint8_t value );

void MAX7219_Start();

void MAX7219_Shutdown();

#endif /* MAX7219_H_ */

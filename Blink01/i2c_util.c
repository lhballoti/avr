/*
 * i2c_util.c
 *
 *  Created on: 14/08/2012
 *      Author: Luiz
 */

#include <avr/io.h>
#include <util/twi.h>

/*static*/ void i2c_wait() {
	while ( !( TWCR & _BV( TWINT ) ) ) ;
}

/*static*/ void i2c_stop() {
	TWCR = _BV( TWINT ) | _BV( TWSTO ) | _BV( TWEN );
}

void i2c_init() {
	TWBR = 8;
	TWCR &= ~_BV( TWIE );
}

uint8_t i2c_begin( uint8_t addr, uint8_t rnw ) {

	// transmit start condition
	TWCR = _BV( TWINT ) | _BV( TWSTA ) | _BV( TWEN );
	i2c_wait();
	if ( TW_STATUS != TW_START ) {
		return -1;
	}

	// transmit slave address and data direction
	TWDR = ( addr << 1 ) | ( rnw & 0x01 );
	TWCR = _BV( TWINT ) | _BV( TWEN );
	i2c_wait();
	uint8_t expect = ( rnw ) ? TW_MR_SLA_ACK : TW_MT_SLA_ACK;
	if ( TW_STATUS != expect ) {
		i2c_stop();
		return -1;
	}

	return 0;
}

uint8_t i2c_write( uint8_t data, uint8_t stop ) {

	TWDR = data;
	TWCR = _BV( TWINT ) | _BV( TWEN );
	i2c_wait();
	if ( TW_STATUS != TW_MT_DATA_ACK ) {
		i2c_stop();
		return -1;
	}

	if ( stop ) {
		i2c_stop();
	}

	return 0;
}

uint8_t i2c_read( uint8_t stop ) {

	uint8_t twcr = _BV( TWINT ) | _BV( TWEN ) | _BV( TWEA );
	if ( stop ) {
		twcr = _BV( TWINT ) | _BV( TWEN );
	}
	TWCR = twcr;
	i2c_wait();
	uint8_t data = TWDR;
	if ( TW_STATUS == TW_MR_DATA_NACK ) {
		i2c_stop();
	}

	return data;
}

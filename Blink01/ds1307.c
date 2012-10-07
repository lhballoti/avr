/*
 * ds1307.c
 *
 *  Created on: 02/09/2012
 *      Author: Luiz
 */

#include "ds1307.h"
#include "i2c_util.h"

#define DS1307_BUS_ADDRESS	0x68
#define DS1307_SECONDS_REG	0x00
#define DS1307_MINUTES_REG	0x01
#define DS1307_HOURS_REG	0x02
#define DS1307_CONTROL_REG	0x07

void DS1307_Start() {
	// set register pointer
	i2c_begin( DS1307_BUS_ADDRESS, 0 );
	i2c_write( DS1307_SECONDS_REG, 1 );

	// read CH bit
	i2c_begin( DS1307_BUS_ADDRESS, 1 );
	uint8_t secs = i2c_read( 1 );

	if ( secs & 0x80 ) {
		i2c_begin( DS1307_BUS_ADDRESS, 0 );
		i2c_write( DS1307_SECONDS_REG, 0 );
		i2c_write( secs & 0x7F, 1 );
	}
}

void DS1307_GetDateTime( ds1307_datetime* dt ) {
	// set register pointer
	i2c_begin( DS1307_BUS_ADDRESS, 0 );
	i2c_write( DS1307_SECONDS_REG, 1 );

	// read data
	i2c_begin( DS1307_BUS_ADDRESS, 1 );
	dt->second = i2c_read( 0 ) & 0x7F;
	dt->minute = i2c_read( 0 ) & 0x7F;
	dt->hour = i2c_read( 0 ) & 0x3F;
	dt->wkday = i2c_read( 0 ) & 0x07;
	dt->day = i2c_read( 0 ) & 0x3F;
	dt->month = i2c_read( 0 ) & 0x1F;
	dt->year = 0x2000 | i2c_read( 1 );
}

void DS1307_SetDateTime( ds1307_datetime* dt ) {
	// set register pointer
	i2c_begin( DS1307_BUS_ADDRESS, 0 );
	i2c_write( DS1307_SECONDS_REG, 0 );
	// write data
	i2c_write( dt->second & 0x7F, 0 );
	i2c_write( dt->minute & 0x7F, 0 );
	i2c_write( dt->hour & 0x3F, 0 );
	i2c_write( dt->wkday & 0x07, 0 );
	i2c_write( dt->day & 0x3F, 0 );
	i2c_write( dt->month & 0x1F, 0 );
	i2c_write( dt->year, 1 );
}

void DS1307_SetControl( uint8_t ctrl ) {
	i2c_begin( DS1307_BUS_ADDRESS, 0 );
	i2c_write( DS1307_CONTROL_REG, 0 );
	i2c_write( ctrl, 1 );
}

void DS1307_WriteRam( void* buf, uint8_t addr, uint8_t length ) {
	uint8_t* ptr = buf;
	i2c_begin( DS1307_BUS_ADDRESS, 0 );
	i2c_write( addr, 0 );
	for ( int i = 0; i < length - 1; i++ ) {
		i2c_write( *( ptr + i ), 0 );
	}
	i2c_write( *( ptr + ( length - 1 ) ), 1 );
}

void DS1307_ReadRam( void* buf, uint8_t addr, uint8_t length ) {
	uint8_t* ptr = buf;
	i2c_begin( DS1307_BUS_ADDRESS, 0 );
	i2c_write( addr, 1 );
	i2c_begin( DS1307_BUS_ADDRESS, 1 );
	for ( int i = 0; i < length - 1; i++ ) {
		*( ptr + i ) = i2c_read( 0 );
	}
	*( ptr + ( length - 1 ) ) = i2c_read( 1 );
}

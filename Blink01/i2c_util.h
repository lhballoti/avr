/*
 * i2c_util.h
 *
 *  Created on: 14/08/2012
 *      Author: Luiz
 */

#ifndef I2C_UTIL_H_
#define I2C_UTIL_H_

void i2c_wait();
void i2c_stop();

void i2c_init();

uint8_t i2c_begin( uint8_t addr, uint8_t read );

uint8_t i2c_write( uint8_t data, uint8_t stop );

uint8_t i2c_read( uint8_t stop );

//void i2c_end();

#endif /* I2C_UTIL_H_ */

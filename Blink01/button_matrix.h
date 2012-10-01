/*
 * button_matrix.h
 *
 *  Created on: 14/09/2012
 *      Author: Luiz
 */

#ifndef __BUTTON_MATRIX_H__
#define __BUTTON_MATRIX_H__

#include <stdint.h>

#define BUTTON_MATRIX_ROW_PORT C
#define BUTTON_MATRIX_ROW_COUNT 3

#define BUTTON_MATRIX_COLUMN_PORT D
#define BUTTON_MATRIX_COLUMN_COUNT 3

void ButtonMatrix_Init();

char ButtonMatrix_stateChanged();

char ButtonMaxtrix_buttonStateChanged( uint8_t button );

char ButtonMatrix_isPressed( uint8_t button );

char ButtonMatrix_wasPressed( uint8_t button );

char ButtonMatrix_wasReleased( uint8_t button );

#endif /* __BUTTON_MATRIX_H__ */

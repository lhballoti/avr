/*
 * button_matrix.h
 *
 *  Created on: 14/09/2012
 *      Author: Luiz
 */

#ifndef BUTTON_MATRIX_H_
#define BUTTON_MATRIX_H_

#include <stdint.h>

#define BUTTON_MATRIX_ROW_PORT C
#define BUTTON_MATRIX_ROW_COUNT 3

#define BUTTON_MATRIX_COLUMN_PORT D
#define BUTTON_MATRIX_COLUMN_COUNT 3

void ButtonMatrix_Init();

char ButtonMatrix_stateChanged();

char ButtonMaxtrix_buttonStateChanged( uint8_t button );

char ButtonMatrix_isPressed( uint8_t button );

#endif /* BUTTON_MATRIX_H_ */

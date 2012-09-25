/*
 * button_matrix.c
 *
 *  Created on: 14/09/2012
 *      Author: Luiz
 */

#include "button_matrix.h"
#include <avr/io.h>
#include <string.h>
#include <util/delay_basic.h>

#if !defined( BUTTON_MATRIX_ROW_PORT ) || !defined( BUTTON_MATRIX_ROW_COUNT ) || !defined( BUTTON_MATRIX_COLUMN_PORT ) || !defined( BUTTON_MATRIX_COLUMN_COUNT )
#error "One or more BUTTON_MATRIX macros are not defined, edit button_matrix.h"
#endif

#define BUTTON_RESET 0xFF
#define BUTTON_PRESSED 0xf0

//#define CONCAT(a, b) __CONCAT(a, b)
//#define __CONCAT(a, b) a ## b

#define ROW_PORT __CONCAT( PORT, BUTTON_MATRIX_ROW_PORT )
#define ROW_DDR __CONCAT( DDR, BUTTON_MATRIX_ROW_PORT )
#define ROW_MASK ( ( 1 << BUTTON_MATRIX_ROW_COUNT ) - 1 )
#define COL_PORT __CONCAT( PORT, BUTTON_MATRIX_COLUMN_PORT )
#define COL_DDR __CONCAT( DDR, BUTTON_MATRIX_COLUMN_PORT )
#define COL_PIN __CONCAT( PIN, BUTTON_MATRIX_COLUMN_PORT )
#define COL_MASK ( ( 1 << BUTTON_MATRIX_COLUMN_COUNT ) - 1 )

// on each scan operation, we'll shift the button input
// values into each byte; when four consecutive readings
// return LOW, we'll consider the button was pressed
static uint8_t buttons[BUTTON_MATRIX_ROW_COUNT * BUTTON_MATRIX_COLUMN_COUNT];

static int oldButtonStates;
static int buttonStates;

void ButtonMatrix_Init() {
	memset( buttons, BUTTON_RESET, BUTTON_MATRIX_ROW_COUNT * BUTTON_MATRIX_COLUMN_COUNT );

	// rows as inputs
	ROW_DDR &= ~ROW_MASK;
	// enable internal pull-ups
	ROW_PORT |= ROW_MASK;

	// columns as inputs
	COL_DDR &= ~COL_MASK;
	// enable internal pull-ups
	COL_PORT |= COL_MASK;
}

static int scan() {
	int states = 0;
	int count = 0;
	uint8_t row, column;

	for ( row = 0x01; row & ROW_MASK; row <<= 1 ) {
		// select (pull down) row
		ROW_DDR ^= row;
		ROW_PORT &= ~row;

		_delay_loop_1( 10 );

		uint8_t colvalues = COL_PIN;
		for ( column = 0x01; column & COL_MASK; column <<= 1 ) {
			uint8_t but = buttons[count];
			but <<= 1;
			if ( colvalues & column ) {
				but++;
			}
			if ( ( but | BUTTON_PRESSED ) == BUTTON_PRESSED ) {
				states |= 1 << count;
			}
			buttons[count] = but;
			count++;
		}

		// unselect the row
		ROW_DDR ^= row;
		ROW_PORT |= row;

		_delay_loop_1( 10 );
	}

	return states;
}

char ButtonMatrix_stateChanged() {
	oldButtonStates = buttonStates;
	buttonStates = scan();
	return buttonStates != oldButtonStates;
}

char ButtonMatrix_isPressed( uint8_t button ) {
	return ( buttonStates & ( 1 << button )  ) != 0;
}

char ButtonMaxtrix_buttonStateChanged( uint8_t button ) {
	return ( ( buttonStates ^ oldButtonStates ) & ( 1 << button ) ) != 0;
}

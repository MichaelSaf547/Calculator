/*
 * Keypad_cfg.c
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */


#include "../../Lib/Std_types.h"
#include "../../Lib/Bit_utils.h"
#include "../../Mcal/Gpio/Gpio.h"
#include "Keypad.h"
#include "Keypad_cfg.h"



const Keypad_cfg_t Keypad_Pins[NUM_COLS + NUM_ROWS] =
	   {
			   [KEYPAD_ROW0] =
			   {
					   .Keypad_Port = GPIO_PORTB,
					   .Keypad_Pin = GPIO_PIN0
			   },

			   [KEYPAD_ROW1] =
			   {
					   .Keypad_Port = GPIO_PORTB,
					   .Keypad_Pin = GPIO_PIN1
			   },

			   [KEYPAD_ROW2] =
			   {
					   .Keypad_Port = GPIO_PORTB,
					   .Keypad_Pin = GPIO_PIN2
			   },

			   [KEYPAD_ROW3] =
			   {
					   .Keypad_Port = GPIO_PORTB,
					   .Keypad_Pin = GPIO_PIN3
			   },

			   [KEYPAD_COL0] =
			   {
					   .Keypad_Port = GPIO_PORTB,
					   .Keypad_Pin = GPIO_PIN4
			   },

			   [KEYPAD_COL1] =
			   {
					   .Keypad_Port = GPIO_PORTB,
					   .Keypad_Pin = GPIO_PIN5
			   },

			   [KEYPAD_COL2] =
			   {
					   .Keypad_Port = GPIO_PORTB,
					   .Keypad_Pin = GPIO_PIN6
			   },

			   [KEYPAD_COL3] =
			   {
					   .Keypad_Port = GPIO_PORTB,
					   .Keypad_Pin = GPIO_PIN7
			   },
	   };



/*
 * Button indications
 */
/*Fill the 2D array with the button indication, what each button indicates what.*/

const u8 Keypad_aau8ButtonIndicator[NUM_ROWS][NUM_COLS] = {{'1', '2', '3', '+'},
														   {'4', '5', '6', '-'},
														   {'7', '8', '9', '*'},
														   {'%', '0', '=', '/'}};



/*
 * Keypad_cfg.h
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */

#ifndef HAL_KEYPAD_KEYPAD_CFG_H_
#define HAL_KEYPAD_KEYPAD_CFG_H_


/*
 * Number of rows and columns of the keypad
 */
#define NUM_ROWS		4
#define NUM_COLS		4

/*
 * Rows numbering in regards to the pins configuration array
 */
#define KEYPAD_ROW0		0
#define KEYPAD_ROW1		1
#define KEYPAD_ROW2		2
#define KEYPAD_ROW3		3


/*
 * Columns numbering in regards to the pins configuration array
 */
#define KEYPAD_COL0		4
#define KEYPAD_COL1		5
#define KEYPAD_COL2		6
#define KEYPAD_COL3		7


/*KEYPADTASKPRIORITY is the priority of the Keypad task and it must be from 0 to the max number of the tasks*/
#define KEYPADTASKPRIORITY				ONE

/*KEYPADTASKPERIODICTIME is the periodic time for the Keypad task. Five is a recommended number*/
#define KEYPADTASKPERIODICTIME			5


#endif /* HAL_KEYPAD_KEYPAD_CFG_H_ */

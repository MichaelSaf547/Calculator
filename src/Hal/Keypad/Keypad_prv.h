/*
 * Keypad_prv.h
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */

#ifndef HAL_KEYPAD_KEYPAD_PRV_H_
#define HAL_KEYPAD_KEYPAD_PRV_H_

/*
 * Keypad_Task is used to check the keypad each cyclic time to check if a button is pressed and save its value.
 *
 * Inputs: it takes no input.
 *
 * Return: It returns nothing.
 *
 */
extern void Keypad_Task(void);


#endif /* HAL_KEYPAD_KEYPAD_PRV_H_ */

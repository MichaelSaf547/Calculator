/*
 * Lcd_cfg.h
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */

#ifndef HAL_LCD_LCD_CFG_H_
#define HAL_LCD_LCD_CFG_H_

#define LCD_NUM_ROWS        2
#define LCD_NUM_COLS        16


/*Choose INTERFACE_TYPE from two options
* 1- 0 for Four pins
* 2- 1 for Eight pins
*/
#define INTERFACE_TYPE      1



/*Choose interface pins from Gpio_Pin_0 -> PIN15D*/
/*Choose interface ports from Gpio_Port_A -> Gpio_Port_H*/


#define LCD_ENABLE_PIN          	0

#define LCD_READ_WRITE_PIN      	1

#define LCD_REGISTERSELECT_PIN      2


#if INTERFACE_TYPE == 1

#define LCD_PINS		11

#define LCD_D0_PIN                  3


#define LCD_D1_PIN                  4


#define LCD_D2_PIN                  5


#define LCD_D3_PIN                  6


#define LCD_D4_PIN                  7


#define LCD_D5_PIN                  8


#define LCD_D6_PIN                  9


#define LCD_D7_PIN                  10

#else

#define LCD_PINS		7

#define LCD_D4_PIN                  3


#define LCD_D5_PIN                  4


#define LCD_D6_PIN                  5


#define LCD_D7_PIN                  6


#endif



/*
 * LCD Control
*/
/*
	I/D (Increment/Decrement)
0 - Auto Increment the address counter. Cursor blink/move to left
1 - Auto Decrement the address counter. Cursor blink/move to right
*/

#define LCD_ID				1


/*
 * Shift
 * 0 – Shifting Disabled
 * 1 – During a DD RAM write operation, shift the entire display value in the direction
 * controlled by Bit DB1 (I/D). Appears as though the cursor position remains controlled by Bit DB1 (I/D). Appears as though the cursor position remains
 * constant and the display moves
 *
 */

#define LCD_SHIFT			0

/*
 * Display on/off
 * 0 – No character displayed. Data stored in DD RAM is retained
 * 1 – Display character stored in DD RAM
 */

#define LCD_DISPLAY			1


/*
 * Cursor on/off
 * 0 – No cursor
 * 1 – Display cursor
 */

#define LCD_CURSORON		0

/*
 * Cursor blink on/off
 * 0 – No cursor blinking
 * 1 – Cursor blinks on/off approx. every half second
 */

#define LCDCURSORBLINK		0


/*
*   3th bit is for two lines(1) or one line(0)				N
*   2th bit is for the number of Dots 11(1) or 8(0)			F
*/

#define LCDLINES			1
#define LCDNUMOFDOTS		0



/*
 * S/C	R/L
 * 0 	0 		Shift cursor left. Add. Counter decrement by 1
 * 0 	1 		Shift cursor right. Add. Counter increment by 1
 * 1 	0 		Shift entire display to left. Add. Counter remains unchanged
 * 1 	1 		Shift entire display to the right. Add. Counter remains unchanged
 */


#define LCDSC				0
#define LCDRL				1


/*LCDTASKPRIORITY is the priority of the LCD task and it must be from 0 to the max number of the tasks*/
#define LCDTASKPRIORITY				ZERO

/*LCDTASKPERIODICTIME is the periodic time for the LCD task*/
#define LCDTASKPERIODICTIME			2



#endif /* HAL_LCD_LCD_CFG_H_ */

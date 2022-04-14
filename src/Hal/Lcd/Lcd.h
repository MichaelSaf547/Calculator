/*
 * Lcd.h
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */

#ifndef HAL_LCD_LCD_H_
#define HAL_LCD_LCD_H_

/*
 * Typedefs
 */

typedef struct
{
	void * Lcd_Port;
	u16 Lcd_Pin;

}Lcd_cfg_t;


/*
*   Options for the address of the CGRAM
*   0b0100 0000
*   0b0100 1000
*   0b0101 0000
*   0b0101 1000
*   0b0110 0000
*   0b0110 1000
*   0b0111 0000
*   0b0111 1000

*/

#define LCDCGRAMZERO        0x40
#define LCDCGRAMONE         0x48
#define LCDCGRAMTWO         0x50
#define LCDCGRAMTHREE       0x58
#define LCDCGRAMFOUR        0x60
#define LCDCGRAMFIVE        0x68
#define LCDCGRAMSIX         0x70
#define LCDCGRAMSEVEN       0x78


/*
* Lcd_vidInit: is used to initialize the LCD
* Inputs: it takes no inputs (void)
* Return: it does not return anything(void)
*/
extern void Lcd_vidInit(void);

/*
* Lcd_vidSendCommand: is used to send command to the LCD
* Inputs: it takes one input the command to be sent.
* Return: it does not return anything(void)
*/
extern void Lcd_vidSendCommand(u8 Copy_u8Command);

/*
* Lcd_vidDisplayCharacter: is used to send a character to the LCD and display it.
* Inputs: it takes one input the character to be displayed on the LCD.
*       Options: LCDCGRAMZERO -> LCDCGRAMSEVEN
* Return: it does not return anything (void)
*/
extern void Lcd_vidDisplayCharacter(u8 Copyu8Character);

/*
 * Lcd_vidDisplayString: is used to display a string on the LCD in the defined position.
 *
 * Inputs: it takes four inputs, 1- Add_pu8DataBuffer is an address to the buffer.
 * 								 2- Copy_u8BufferLength is the length of the data in the buffer.
 * 								 3- Copy_u8RowIndex is the row location of the display.
 * 								 4- Copy_u8ColIndex is the column location of the display.
 *
 * 	Output: it returns nothing.
 */



extern void Lcd_vidDisplayString(pu8 Add_pu8DataBuffer, u8 Copy_u8BufferLength, u8 Copy_u8RowIndex, u8 Copy_u8ColIndex);

/*
* Lcd_vidCreateCustomCharacter: is used to create a custom character on the CGRAM
* Inputs: it takes one the Address where to save the character
* Return: it does not return anything (void)
*/
extern void Lcd_vidCreateCustomCharacter(u8 Copyu8Address, pu8 Add_pu8ACharData);

/*
* Lcd_vidGoTo: is used to jump to a custom position in the LCd
* Inputs: it takes two inputs the row and the col
* Return: it does not return anything (void)
*/
extern void Lcd_vidGoTo(u8 Copy_u8Row, u8 Copy_u8Col);

/*
 * Lcd_vidGetBusyState is used to get the busy flag of the LCD task
 *
 * Inputs: It takes one input the address to get the value in it.
 * 								if the address value is Null the function will ignore the operation
 * 								if not the function set the variable to two values
 * 											1- ZERO means it is not busy.
 * 											2- ONE means it is busy.
 *
 * Return: It returns nothing.
 */
extern void Lcd_vidGetBusyState(pu8 Add_pu8BusyState);



#endif /* HAL_LCD_LCD_H_ */

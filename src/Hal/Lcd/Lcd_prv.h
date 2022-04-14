/*
 * Lcd_prv.h
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */

#ifndef HAL_LCD_LCD_PRV_H_
#define HAL_LCD_LCD_PRV_H_


/*
 * Concatenate zeros and ones together
 */
#define CONCAT(B7, B6, B5, B4, B3, B2, B1, B0)        HELPER(B7, B6, B5, B4, B3, B2, B1, B0)
#define HELPER(B7, B6, B5, B4, B3, B2, B1, B0)        0b##B7##B6##B5##B4##B3##B2##B1##B0



/*
*   Types
*/

typedef union 
{
    u8 Lcd_InByte;
    
    struct bits
    {
        u8 b0 : 1;
        u8 b1 : 1;
        u8 b2 : 1;
        u8 b3 : 1;
        u8 b4 : 1;
        u8 b5 : 1;
        u8 b6 : 1;
        u8 b7 : 1;
        
    }Lcd_InBits;
    
}Lcd_tunUnion;




typedef enum
{
	Lcd_enuIdle,
	Lcd_enuInit,
	Lcd_enuSendCommand,
	Lcd_enuDisplayChar,
	Lcd_enuDisplayString,
	Lcd_enuCreateCustom,
	Lcd_enuGoTo,

}Lcd_tenuStates;

typedef enum
{
	Lcd_enuInitIdle,
	Lcd_enuInitState0,
	Lcd_enuInitState1,
	Lcd_enuInitState2,
	Lcd_enuInitState3,
	Lcd_enuInitState4,
	Lcd_enuInitState5,
	Lcd_enuInitState6,
	Lcd_enuInitState7,

}Lcd_tenuInitStates;


typedef enum
{
	Lcd_enuSendCommIdle,
	Lcd_enuSendComm0,
	Lcd_enuSendComm1,

}Lcd_tenuSendCommandStates;


typedef enum
{
	Lcd_enuDisplayCharIdle,
	Lcd_enuDisplayChar0,
	Lcd_enuDisplayChar1,

}Lcd_tenuDisplayCharStates;

typedef enum
{
	Lcd_enuDisplayStringIdle,
	Lcd_enuDisplayStringState0,

}Lcd_tenuSisplayStringStates;

typedef enum
{
	Lcd_enuCCCIdle,
	Lcd_enuCCCState0,

}Lcd_tenuCCCStates;




/*
*   Defines
*/

#define LCD_FIRST_ROW_ADD		0x80
#define LCD_SECOND__ROW_ADD		0xC0

#define LCD_OFF        0
#define LCD_ON         1


/*
*   0b001xxx00 is fixed
*   4th bit is for 8 bits(1) or 4 bits(0) of interface
*   3th bit is for two lines(1) or one line(0)
*   2th bit is for the number of Dots 11(1) or 8(0)
*/
#define LCDBITSLINES            CONCAT(0, 0, 1, INTERFACE_TYPE, LCDLINES, LCDNUMOFDOTS, 0, 0)
#define LCDCLEAR                0x01
#define LCDOFF                  0x02
#define LCDENTRYSETMODE         CONCAT(0, 0, 0, 0, 0, 1, LCD_ID, LCD_SHIFT)
#define LCDDIS_CURS_BLK         CONCAT(0, 0, 0, 0, 1, LCD_DISPLAY, LCD_CURSORON, LCDCURSORBLINK)
#define LCDCURSOR_DISPLAY_SH	CONCAT(0, 0, 0, 1, LCDSC, LCDRL, 0, 0)
#define LCDSETPOSITIONLY0X0     0x80


/*
 * Function Prototypes
 */

/*
 * Lcd_Task is used to check if a required task from the LCD and start doing in parts.
 *
 * Inputs: It takes no input.
 *
 * Return: It returns nothing.
 */
extern void Lcd_Task(void);

/*
 * Init_Process is used to initialize the LCD using Task on Ticks instead of one shot
 *
 * Inputs: It takes no input.
 *
 * Return: It returns nothing.
 */
extern void Init_Process(void);


/*
 * SendCommand_Process is used to send command to the LCD using Task on Ticks instead of one shot
 *
 * Inputs: It takes no input.
 *
 * Return: It returns nothing.
 */
extern void SendCommand_Process(void);


/*
 * DisplayChar_Process is used to Display Char on the LCD using Task on Ticks instead of one shot
 *
 * Inputs: It takes no input.
 *
 * Return: It returns nothing.
 */
extern void DisplayChar_Process(void);


 /*
  * DisplayString_Process is used to Display a sting on the LCD in a defined location
  *
  * Inputs: It takes no input.
  *
  * Return: It returns nothing.
  */
extern void DisplayString_Process(void);

/*
 * GoTo_Process is used to go to a position on the LCD using Task on Ticks instead of one shot
 *
 * Inputs: It takes no input.
 *
 * Return: It returns nothing.
 */
extern void GoTo_Process(void);


/*
 * CreateCustomChar_Process is used to save a new character on the LCD using Task on Ticks instead of one shot
 *
 * Inputs: It takes no input.
 *
 * Return: It returns nothing.
 */
extern void CreateCustomChar_Process(void);





#endif /* HAL_LCD_LCD_PRV_H_ */

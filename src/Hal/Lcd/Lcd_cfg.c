/*
 * Lcd_cfg.c
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */


#include "../../Lib/Std_types.h"
#include "../../Lib/Bit_utils.h"
#include "../../Mcal/Gpio/Gpio.h"
#include "Lcd.h"
#include "Lcd_cfg.h"

/*Choose interface pins from Gpio_Pin_0 -> PIN15D*/
/*Choose interface ports from GPIO_PORTA -> Gpio_Port_H*/

/*
 * Set the values of the pins and the ports and do not change the state of the two other values (Mode and speed).
 */

const Lcd_cfg_t Lcd_Pins[LCD_PINS] = {
		[LCD_ENABLE_PIN] =
		{
				.Lcd_Pin = GPIO_PIN8,
				.Lcd_Port = GPIO_PORTA
		},

		[LCD_READ_WRITE_PIN] =
		{
				.Lcd_Pin = GPIO_PIN9,
				.Lcd_Port = GPIO_PORTA
		},

		[LCD_REGISTERSELECT_PIN] =
		{
				.Lcd_Pin = GPIO_PIN10,
				.Lcd_Port = GPIO_PORTA
		},
#if INTERFACE_TYPE == 1
		[LCD_D0_PIN] =
		{
				.Lcd_Pin = GPIO_PIN0,
				.Lcd_Port = GPIO_PORTA
		},

		[LCD_D1_PIN] =
		{
				.Lcd_Pin = GPIO_PIN1,
				.Lcd_Port = GPIO_PORTA
		},

		[LCD_D2_PIN] =
		{
				.Lcd_Pin = GPIO_PIN2,
				.Lcd_Port = GPIO_PORTA
		},

		[LCD_D3_PIN] =
		{
				.Lcd_Pin = GPIO_PIN3,
				.Lcd_Port = GPIO_PORTA
		},
#endif
		[LCD_D4_PIN] =
		{
				.Lcd_Pin = GPIO_PIN4,
				.Lcd_Port = GPIO_PORTA
		},

		[LCD_D5_PIN] =
		{
				.Lcd_Pin = GPIO_PIN5,
				.Lcd_Port = GPIO_PORTA
		},

		[LCD_D6_PIN] =
		{
				.Lcd_Pin = GPIO_PIN6,
				.Lcd_Port = GPIO_PORTA
		},

		[LCD_D7_PIN] =
		{
				.Lcd_Pin = GPIO_PIN7,
				.Lcd_Port = GPIO_PORTA
		}
};






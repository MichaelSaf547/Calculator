/*
 * Lcd_cfg.c
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */


#include "../../Lib/Std_types.h"
#include "../../Lib/Bit_utils.h"
#include "../Gpio/Gpio.h"
#include "Lcd.h"
#include "Lcd_cfg.h"

/*Choose interface pins from Gpio_Pin_0 -> PIN15D*/
/*Choose interface ports from Gpio_Port_A -> Gpio_Port_H*/

/*
 * Set the values of the pins and the ports and do not change the state of the two other values (Mode and speed).
 */

const Lcd_cfg_t Lcd_Pins[LCD_PINS] = {
		[LCD_ENABLE_PIN] =
		{
				.Lcd_Pin = Gpio_Pin_8,
				.Lcd_Port = Gpio_Port_A
		},

		[LCD_READ_WRITE_PIN] =
		{
				.Lcd_Pin = Gpio_Pin_9,
				.Lcd_Port = Gpio_Port_A
		},

		[LCD_REGISTERSELECT_PIN] =
		{
				.Lcd_Pin = Gpio_Pin_10,
				.Lcd_Port = Gpio_Port_A
		},
#if INTERFACE_TYPE == 1
		[LCD_D0_PIN] =
		{
				.Lcd_Pin = Gpio_Pin_0,
				.Lcd_Port = Gpio_Port_A
		},

		[LCD_D1_PIN] =
		{
				.Lcd_Pin = Gpio_Pin_1,
				.Lcd_Port = Gpio_Port_A
		},

		[LCD_D2_PIN] =
		{
				.Lcd_Pin = Gpio_Pin_2,
				.Lcd_Port = Gpio_Port_A
		},

		[LCD_D3_PIN] =
		{
				.Lcd_Pin = Gpio_Pin_3,
				.Lcd_Port = Gpio_Port_A
		},
#endif
		[LCD_D4_PIN] =
		{
				.Lcd_Pin = Gpio_Pin_4,
				.Lcd_Port = Gpio_Port_A
		},

		[LCD_D5_PIN] =
		{
				.Lcd_Pin = Gpio_Pin_5,
				.Lcd_Port = Gpio_Port_A
		},

		[LCD_D6_PIN] =
		{
				.Lcd_Pin = Gpio_Pin_6,
				.Lcd_Port = Gpio_Port_A
		},

		[LCD_D7_PIN] =
		{
				.Lcd_Pin = Gpio_Pin_7,
				.Lcd_Port = Gpio_Port_A
		}
};






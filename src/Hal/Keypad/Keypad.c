/*
 * Keypad.c
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */

#include "../../Lib/Std_types.h"
#include "../../Lib/Bit_utils.h"
#include "../../Mcal/Gpio/Gpio.h"
#include "../../Services/Sched/Sched.h"
#include "Keypad.h"
#include "Keypad_prv.h"
#include "Keypad_cfg.h"


/*Extern the keypad pins*/
extern const Keypad_cfg_t Keypad_Pins[NUM_COLS + NUM_ROWS];

/*Keypad_u8PressedAvaliable is used as a flag for the Keypad_enuGetKey function*/
extern u8 Keypad_u8PressedAvaliable;

/*Keypad_u8PressedKey is used to hold the pressed key value*/
extern u8 Keypad_u8PressedKey;


/*
 * Function implementation
 */

extern Keypad_tenuErrorStatus Keypad_enuInit()
{
	/*Loc_enuReturnStatus is used to hold the error status of the function*/
	Keypad_tenuErrorStatus Loc_enuReturnStatus = Keypad_enuOk;

	/*Loc_u8ForCounter is used as a counter for the for loop.*/
	u8 Loc_u8ForCounter = ZERO;

	/*Loc_enuGpioConfig is used to configure the pins of the keypad*/
	gpioPinCfg_t Loc_enuGpioConfig;

	/*Loc_enuGpioErrorStatus is used to check the initialization of the pins.*/
	gpioErrorStatus_t Loc_enuGpioErrorStatus = gpio_enuOk;

	/*Loc_KeypadRunnable is used to set the Keypad task into the scheduler.*/
	Runnable_t Loc_KeypadRunnable;


	/*Initialize the row pins as output*/
	for(Loc_u8ForCounter = ZERO; Loc_u8ForCounter < NUM_ROWS; Loc_u8ForCounter++)
	{
		/*Set the row pins to be output with high speed*/
		Loc_enuGpioConfig.mode = GPIO_PIN_OUT_PP;
		Loc_enuGpioConfig.speed = GPIO_PIN_SPEED2;

		Loc_enuGpioConfig.port = Keypad_Pins[Loc_u8ForCounter].Keypad_Port;
		Loc_enuGpioConfig.pin = Keypad_Pins[Loc_u8ForCounter].Keypad_Pin;

		/*Initialize the pin*/
		Loc_enuGpioErrorStatus = gpio_initPin(&Loc_enuGpioConfig);

		/*Check the error status of the gpio_init function*/
		if(Loc_enuGpioErrorStatus != gpio_enuOk)
		{
			/*Set Keypad_enuNotOk to Loc_enuReturnStatus as the the function called initialize the pin.*/
			Loc_enuReturnStatus = Keypad_enuNotOk;
		}/*end of if(Loc_enuGpioErrorStatus != Gpio_enuOk)*/
		else
		{
			/*Do nothing*/
		}/*end of else of if(Loc_enuGpioErrorStatus != Gpio_enuOk)*/


	}/*end of for(Loc_u8ForCounter = ZERO; Loc_u8ForCounter < NUM_ROWS; Loc_u8ForCounter++)*/

	/*Initialize the column pins as input*/
	for(Loc_u8ForCounter = NUM_ROWS; Loc_u8ForCounter < (NUM_COLS + NUM_ROWS); Loc_u8ForCounter++)
	{
		/*Set the row pins to be input pull up with high speed*/
		Loc_enuGpioConfig.mode = GPIO_PIN_IN_PU;
		Loc_enuGpioConfig.speed = GPIO_PIN_SPEED2;

		Loc_enuGpioConfig.port = Keypad_Pins[Loc_u8ForCounter].Keypad_Port;
		Loc_enuGpioConfig.pin = Keypad_Pins[Loc_u8ForCounter].Keypad_Pin;

		/*Initialize the pin*/
		Loc_enuGpioErrorStatus = gpio_initPin(&Loc_enuGpioConfig);

		/*Check the error status of the gpio_init function*/
		if(Loc_enuGpioErrorStatus != gpio_enuOk)
		{
			/*Set Keypad_enuNotOk to Loc_enuReturnStatus as the the function called initialize the pin.*/
			Loc_enuReturnStatus = Keypad_enuNotOk;
		}/*end of if(Loc_enuGpioErrorStatus != Gpio_enuOk)*/
		else
		{
			/*Do nothing*/
		}/*end of else of if(Loc_enuGpioErrorStatus != Gpio_enuOk)*/


	}/*end of for(Loc_u8ForCounter = NUM_ROWS; Loc_u8ForCounter < (NUM_COLS + NUM_ROWS); Loc_u8ForCounter++)*/

	/*Add the Keypad task into the scheduler*/
	Loc_KeypadRunnable.CyclicTimems = KEYPADTASKPERIODICTIME;

	/*Set the Keypad_Task pointer into the runnable pointer.*/
	Loc_KeypadRunnable.cbfP = Keypad_Task;

	/*Add the Keypad task to scheduler with the configured priority value.*/
	Sched_RegisterRunnable(&Loc_KeypadRunnable, KEYPADTASKPRIORITY);

	/*Return the error status to the caller*/
	return Loc_enuReturnStatus;
}


extern Keypad_tenuErrorStatus Keypad_enuGetKey(pu8 Add_pu8PressedKey)
{
	/*Loc_enuReturnStatus is used to hold the error status of the function*/
	Keypad_tenuErrorStatus Loc_enuReturnStatus = Keypad_enuOk;

	if(Keypad_u8PressedAvaliable == ONE)
	{
		/*Check if the pointer is pointing to null*/
		if(Add_pu8PressedKey == NULL)
		{
			/*Set Keypad_enuNullPointer to Loc_enuReturnStatus as the pointer is pointing to null.*/
			Loc_enuReturnStatus = Keypad_enuNullPointer;

		}/*end of if(Add_pu8PressedKey == NULL)*/
		else
		{
			/*Save the value of the pressed key into the pointer.*/
			*Add_pu8PressedKey = Keypad_u8PressedKey;

			/*Clear the flag that indicates a pressed key.*/
			Keypad_u8PressedAvaliable = ZERO;

		}/*end of else of if(Add_pu8PressedKey == NULL)*/

	}/*end of if(Keypad_u8PressedAvaliable == ONE)*/
	else
	{
		/*Set s to Loc_enuReturnStatus as now key has been pressed.*/
		Loc_enuReturnStatus = Keypad_enuNotOk;

	}/*end of else of if(Keypad_u8PressedAvaliable == ONE)*/

	/*Return the error status to the caller*/
	return Loc_enuReturnStatus;
}


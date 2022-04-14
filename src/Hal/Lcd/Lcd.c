/*
 * Lcd.c
 *
 *  Created on: Apr 14, 2022
 *      Author: Michael Safwat
 */


#include "../Lib/Std_types.h"
#include "../Lib/Bit_utils.h"
#include "../Gpio/Gpio.h"
#include "../Sched/Sched.h"
#include "../Sched/Sched_cfg.h"
#include "../Lcd/Lcd.h"
#include "../Lcd/Lcd_prv.h"
#include "../Lcd/Lcd_cfg.h"


/*Extern the LCD Pins*/
extern const const Lcd_cfg_t Lcd_Pins[LCD_PINS];

/*Lcd_enuState is used to hold the LCD state.*/
extern Lcd_tenuStates Lcd_enuState;

/*
 * Lcd_enuPrvState is used to hold the previous LCD state as a
 * state may need to use another state so i need to save it.
 */
extern Lcd_tenuStates Lcd_enuPrvState;

/*Lcd_u8Command is used to buffer the command to be done*/
extern u8 Lcd_u8Command;

/*Lcd_u8Display is used to buffer the data to be displayed*/
extern u8 Lcd_u8Display;

/*Lcd_u8Row is used to buffer the row location for goto*/
extern u8 Lcd_u8Row;

/*Lcd_u8Col is used to buffer the column location for goto*/
extern u8 Lcd_u8Col;

/*Lcd_au8DataBuffer is used to buffer data for string or create custom character.*/
extern u8 Lcd_au8DataBuffer[16];

/*Lcd_u8BufferLength is used to save the length of the data in the buffer*/
extern u8 Lcd_u8BufferLength;

extern void Lcd_vidInit(void)
{
	/*Loc_run is used to configure the LCD task and set it into the scheduler*/
	Runnable_t Loc_run;

	/*Loc_u8ForCounter is used to loop over the LCD_Pins to initialize them*/
	u8 Loc_u8ForCounter = ZERO;

	/*Loc_Pincfg is used to initialize the LCD pins*/
	Gpio_Pincfg_t Loc_Pincfg;

	/*Set the periodic time of the LCD task to two*/
	Loc_run.CyclicTimems = LCDTASKPERIODICTIME;

	/*Set the call back function pointer to the LCD task*/
	Loc_run.cbfP = Lcd_Task;

	/*Add the task into the Scheduler*/
	Sched_RegisterRunnable(&Loc_run, LCDTASKPRIORITY);

	/*Set the LCD state to*/
	Lcd_enuState = Lcd_enuInit;

	/*Initialize the pins*/
	for(Loc_u8ForCounter = LCD_PINS; Loc_u8ForCounter > 0; Loc_u8ForCounter--)
	{
		/*Set the mode of the pin to general output push pull*/
		Loc_Pincfg.Gpio_Mode = Gpio_Mode_Out_GP_PP;

		/*Set the speed of the pin to high*/
		Loc_Pincfg.Gpio_Speed = Gpio_Speed_High;

		/*Set the pin number from the constant array*/
		Loc_Pincfg.Gpio_Pin = Lcd_Pins[Loc_u8ForCounter - ONE].Lcd_Pin;

		/*Set the port number from the constant array*/
		Loc_Pincfg.Gpio_Port = Lcd_Pins[Loc_u8ForCounter - ONE].Lcd_Port;

		Gpio_enuInit(&Loc_Pincfg);
	}
}


extern void Lcd_vidSendCommand(u8 Copy_u8Command)
{
	/*
	 * Check if the LCD state is not Lcd_enuSendCommand
	 * and notLcd_enuDisplayChar  make it send command and
	 * save the previous state
	 */
	if((Lcd_enuState != Lcd_enuSendCommand) && (Lcd_enuState != Lcd_enuDisplayChar))
	{
		/*Save the previous state*/
		Lcd_enuPrvState = Lcd_enuState;

		/*Set the state to Send command*/
		Lcd_enuState = Lcd_enuSendCommand;

		/*Buffer The command*/
		Lcd_u8Command = Copy_u8Command;
	}/*end of if(Lcd_enuState != Lcd_enuSendCommand)*/
	else
	{
		/*Ignore it*/
	}/*end of else of if(Lcd_enuState != Lcd_enuSendCommand)*/

}


extern void Lcd_vidDisplayCharacter(u8 Copy_u8Character)
{
	/*
	 * Check if the LCD state is not Display char
	 * and not Lcd_enuSendCommand make it Display character
	 * and save the state before change it
	 */
	if((Lcd_enuState != Lcd_enuDisplayChar) && (Lcd_enuState != Lcd_enuSendCommand))
	{
		/*Save the previous state before change it*/
		Lcd_enuPrvState = Lcd_enuState;

		/*Set the current state to Display char*/
		Lcd_enuState = Lcd_enuDisplayChar;

		/*Buffer The Char*/
		Lcd_u8Display = Copy_u8Character;
	}/*end of if(Lcd_enuState != Lcd_enuDisplayChar)*/
	else
	{
		/*Ignore it*/
	}/*end of else of if(Lcd_enuState != Lcd_enuDisplayChar)*/

}


extern void Lcd_vidDisplayString(pu8 Add_pu8DataBuffer, u8 Copy_u8BufferLength, u8 Copy_u8RowIndex, u8 Copy_u8ColIndex)
{
	/*Check if the LCD state is idle make it create custom character*/
	if(Lcd_enuState == Lcd_enuIdle)
	{
		/*Check if the pointer to the array is pointing to null or not*/
		if(Add_pu8DataBuffer == NULL)
		{
			/*Error Handling*/
		}/*end of if(Add_pu8ACharData == NULL)*/
		else
		{
			/*Check that the length of the buffer is more than zero and smaller than 16*/
			if(Copy_u8BufferLength > ZERO && Copy_u8BufferLength < 17)
			{
				/*Loc_u8ForCounter is used as a counter for the for loop*/
				u8 Loc_u8ForCounter;

				/*Change the LCD state to Lcd_enuCreateCustom for the next Tick*/
				Lcd_enuState = Lcd_enuDisplayString;

				/*Save the data in the buffer*/
				for(Loc_u8ForCounter = Copy_u8BufferLength; Loc_u8ForCounter > ZERO; Loc_u8ForCounter--)
				{
					/*Save each byte in the buffer*/
					Lcd_au8DataBuffer[Loc_u8ForCounter - ONE] = Add_pu8DataBuffer[Loc_u8ForCounter - ONE];
				}/*end of for(Loc_u8ForCounter = EIGHT; Loc_u8ForCounter < ZERO; Loc_u8ForCounter--)*/

				/*Save the row and the column location*/
				Lcd_u8Row = Copy_u8RowIndex;
				Lcd_u8Col = Copy_u8ColIndex;

				/*Save the data length*/
				Lcd_u8BufferLength = Copy_u8BufferLength;

			}/*end of if(Copy_u8BufferLength > ZERO && Copy_u8BufferLength < 17)*/
			else
			{
				/*Ignore it.*/
			}/*end of else of if(Copy_u8BufferLength > ZERO && Copy_u8BufferLength < 17)*/

		}/*end of else of if(Add_pu8ACharData == NULL)*/

	}/*end of if(Lcd_enuState == Lcd_enuIdle)*/
	else
	{
		/*Ignore it*/
	}/*end of else of if(Lcd_enuState == Lcd_enuIdle)*/
}


extern void Lcd_vidCreateCustomCharacter(u8 Copyu8Address, pu8 Add_pu8ACharData)
{
	/*Check if the LCD state is idle make it create custom character*/
	if(Lcd_enuState == Lcd_enuIdle)
	{
		/*Check if the pointer to the array is pointing to null or not*/
		if(Add_pu8ACharData == NULL)
		{
			/*Error Handling*/
		}/*end of if(Add_pu8ACharData == NULL)*/
		else
		{
			/*Loc_u8ForCounter is used as a counter for the for loop*/
			u8 Loc_u8ForCounter;

			/*Change the LCD state to Lcd_enuCreateCustom for the next Tick*/
			Lcd_enuState = Lcd_enuCreateCustom;

			/*Save the data in the buffer*/
			for(Loc_u8ForCounter = EIGHT; Loc_u8ForCounter > ZERO; Loc_u8ForCounter--)
			{
				/*Save each byte in the buffer*/
				Lcd_au8DataBuffer[Loc_u8ForCounter - ONE] = Add_pu8ACharData[Loc_u8ForCounter - ONE];
			}/*end of for(Loc_u8ForCounter = EIGHT; Loc_u8ForCounter < ZERO; Loc_u8ForCounter--)*/

			/*Save the address for the CreateCustomChar_Process*/
			Lcd_u8Command = Copyu8Address;
		}/*end of else of if(Add_pu8ACharData == NULL)*/

	}/*end of if(Lcd_enuState == Lcd_enuIdle)*/
	else
	{
		/*Ignore it*/
	}/*end of else of if(Lcd_enuState == Lcd_enuIdle)*/

}


extern void Lcd_vidGoTo(u8 Copy_u8Row, u8 Copy_u8Col)
{
	/*Check if the LCD state is idle make it go to*/
	if(Lcd_enuState == Lcd_enuIdle || Lcd_enuState == Lcd_enuDisplayString)
	{
		/*Save the location (row and column)*/
		Lcd_u8Row = Copy_u8Row;
		Lcd_u8Col = Copy_u8Col;

		/*Save the previous state*/
		Lcd_enuPrvState = Lcd_enuState;

		/*Set the Lcd_enu to Lcd_enuGoTo*/
		Lcd_enuState = Lcd_enuGoTo;

	}/*end of if(Lcd_enuState == Lcd_enuIdle || Lcd_enuState == Lcd_enuDisplayString)*/
	else
	{
		/*Ignore it*/
	}/*end of else of if(Lcd_enuState == Lcd_enuIdle || Lcd_enuState == Lcd_enuDisplayString)*/
}


extern void Lcd_vidGetBusyState(pu8  Add_pu8BusyState)
{
	/*Check if the pointer is pointing to null or not*/
	if(Add_pu8BusyState == NULL)
	{
		/*Ignore it*/
	}/*end of if(Add_enuBusyState == NULL)*/

	else
	{
		/*Check if the LCD is busy or not*/
		if(Lcd_enuState == Lcd_enuIdle)
		{
			/*Set the address to zero to indicate it is idle.*/
			*Add_pu8BusyState = ZERO;
		}/*end of if(Lcd_enuState == Lcd_enuIdle)*/
		else
		{
			/*Set the address to one to indicate it is busy.*/
			*Add_pu8BusyState = ONE;
		}/*end of else of if(Lcd_enuState == Lcd_enuIdle)*/

	}/*end of else of if(Add_enuBusyState == NULL)*/
}





/*
 * Lcd_prv.c
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */



#include "../../Lib/Std_types.h"
#include "../../Lib/Bit_utils.h"
#include "../../Mcal/Gpio/Gpio.h"
#include "Lcd.h"
#include "Lcd_prv.h"
#include "Lcd_cfg.h"



/*Extern the LCD Pins*/
extern const const Lcd_cfg_t Lcd_Pins[LCD_PINS];

/*Lcd_enuState is used to hold the LCD state.*/
Lcd_tenuStates Lcd_enuState = Lcd_enuIdle;

/*
 * Lcd_enuPrvState is used to hold the previous LCD state as a
 * state may need to use another state so i need to save it.
 */
Lcd_tenuStates Lcd_enuPrvState = Lcd_enuIdle;

/*Lcd_u8Command is used to buffer the command to be done*/
u8 Lcd_u8Command = ZERO;

/*Lcd_u8Display is used to buffer the data to be displayed*/
u8 Lcd_u8Display = ZERO;

/*Lcd_u8Row is used to buffer the row location for goto*/
u8 Lcd_u8Row = 0;

/*Lcd_u8Col is used to buffer the column location for goto*/
u8 Lcd_u8Col = 0;

/*Lcd_au8DataBuffer is used to buffer data for string or create custom character.*/
u8 Lcd_au8DataBuffer[16];

/*Lcd_u8BufferLength is used to save the length of the data in the buffer*/
u8 Lcd_u8BufferLength = ZERO;

extern void Lcd_Task(void)
{

	/*Switch on the LCD states*/
	switch(Lcd_enuState)
	{
		case Lcd_enuIdle:
		{
			/*Do Nothing. Just break.*/
			break;
		}/*end of case Lcd_enuIdle*/

		case Lcd_enuInit:
		{
			Init_Process();
			break;
		}/*end of case Lcd_enuInit*/

		case Lcd_enuSendCommand:
		{
			SendCommand_Process();
			break;
		}/*end of case Lcd_enuSendCommand*/

		case Lcd_enuDisplayChar:
		{
			DisplayChar_Process();
			break;
		}/*end of case Lcd_enuDisplayChar*/

		case Lcd_enuDisplayString:
		{
			DisplayString_Process();
			break;
		}/*end of case Lcd_enuDisplayString*/

		case Lcd_enuCreateCustom:
		{
			CreateCustomChar_Process();
			break;
		}/*end of case Lcd_enuCreateCustom*/

		case Lcd_enuGoTo:
		{
			GoTo_Process();
			break;
		}/*end of case Lcd_enuGoTo*/

		default:
		{
			/*Do Nothing. It will never happen.*/
			break;
		}/*end of default*/
	}/*end of switch(Lcd_enuState)*/
}


extern void Init_Process(void)
{
	/*
	 * Init_Process has 9 states each state needs two states as
	 * it either calls Lcd_vidSendCommand or it calls Lcd_vidDisplayCharacter and both have 2 states:
	 * 1- Define the number of lines and the number of rows to display a character.
	 * 2- Define the number of lines and the number of rows to display a character.
	 * 3- Define the number of lines and the number of rows to display a character.
	 * 4- Turn off the LCD.
	 * 5- Clear the LCD.
	 * 6- Cursor and shifting configuration.
	 * 7- Cursor increment or decrement.
	 * 8- Display and shifting configuration.
	 * 9- Set the Position on the row 0 and column0.
	 */

	/*Loc_enuInitState is used to save the initialization state*/
	static Lcd_tenuInitStates Loc_enuInitState = Lcd_enuInitIdle;

	/*Switch over the Initialization states to check which one should be done now.*/
	switch(Loc_enuInitState)
	{
		case Lcd_enuInitIdle:
		{
			/*Set Lcd_enuInitState0 to Loc_enuInitState for the next Tick*/
			Loc_enuInitState = Lcd_enuInitState0;

			/*Do the 1th part of the initialization process*/
			/*Send a command with the number of lines and the number of rows*/
			Lcd_vidSendCommand(LCDBITSLINES);
			break;
		}/*end of case Lcd_enuInitIdle*/

		case Lcd_enuInitState0:
		{
			/*Set Lcd_enuInitState1 to Loc_enuInitState for the next Tick*/
			Loc_enuInitState = Lcd_enuInitState1;

			/*Do the 2th part of the initialization process*/
			/*Send a command with the number of lines and the number of rows*/
			Lcd_vidSendCommand(LCDBITSLINES);
			break;
		}/*end of case Lcd_enuInitState0*/

		case Lcd_enuInitState1:
		{
			/*Set Lcd_enuInitState2 to Loc_enuInitState for the next Tick*/
			Loc_enuInitState = Lcd_enuInitState2;

			/*Do the 3th part of the initialization process*/
			/*Send a command with the number of lines and the number of rows*/
			Lcd_vidSendCommand(LCDBITSLINES);
			break;
		}/*end of case Lcd_enuInitState1*/

		case Lcd_enuInitState2:
		{
			/*Set Lcd_enuInitState3 to Loc_enuInitState for the next Tick*/
			Loc_enuInitState = Lcd_enuInitState3;

			/*Do the 4th part of the initialization process*/
			/*Turn the LCD Off*/
		    Lcd_vidSendCommand(LCDOFF);
			break;
		}/*end of case Lcd_enuInitState2*/

		case Lcd_enuInitState3:
		{
			/*Set Lcd_enuInitState4 to Loc_enuInitState for the next Tick*/
			Loc_enuInitState = Lcd_enuInitState4;

			/*Do the 5th part of the initialization process*/
		    /*Clear LCD*/
		    Lcd_vidSendCommand(LCDCLEAR);
			break;
		}/*end of case Lcd_enuInitState3*/

		case Lcd_enuInitState4:
		{
			/*Set Lcd_enuInitState5 to Loc_enuInitState for the next Tick*/
			Loc_enuInitState = Lcd_enuInitState5;

			/*Do the 6th part of the initialization process*/
		    /*Display, no cursor and no blinking*/
		    Lcd_vidSendCommand(LCDDIS_CURS_BLK);
			break;
		}/*end of case Lcd_enuInitState4*/

		case Lcd_enuInitState5:
		{
			/*Set Lcd_enuInitState6 to Loc_enuInitState for the next Tick*/
			Loc_enuInitState = Lcd_enuInitState6;

			/*Do the 7th part of the initialization process*/
		    /*Increment cursor by default*/
		    Lcd_vidSendCommand(LCDENTRYSETMODE);
			break;
		}/*end of case Lcd_enuInitState5*/

		case Lcd_enuInitState6:
		{
			/*Set Lcd_enuInitState7 to Loc_enuInitState for the next Tick*/
			Loc_enuInitState = Lcd_enuInitState7;

			/*Do the 8th part of the initialization process*/
		    /*Display and Shifting*/
			Lcd_vidSendCommand(LCDCURSOR_DISPLAY_SH);
			break;
		}/*end of case Lcd_enuInitState6*/

		case Lcd_enuInitState7:
		{
			/*Set Lcd_enuInitIdle to Loc_enuInitState for the next Tick*/
			Loc_enuInitState = Lcd_enuInitIdle;

		    /*Set Lcd_enuIdle to Lcd_enuState to indicate that the initialization is done*/
		    Lcd_enuState = Lcd_enuIdle;

			/*Do the 8th part of the initialization process*/
		    /*Set the position into the line 0 and column 0*/
		    Lcd_vidSendCommand(LCDSETPOSITIONLY0X0);

			break;
		}/*end of case Lcd_enuInitState7*/


		default:
		{
			/*Error Handling*/
			break;
		}/*end of default*/

	}/*end of switch(Loc_enuInitState)*/

}


extern void GoTo_Process(void)
{

	/*
	 * GoTo_Process has just one step but it needs two steps
	 * It calls the Lcd_vidSendCommand function which has 2 states.
	 */

	/*
	*   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
	*   40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F
	*   Using this table we can know how to move position
	*/

	/*Reload the state that called the GoTo Process state*/
	Lcd_enuState = Lcd_enuPrvState;

	/*Check if the row is more than 1 then go to the base address*/
	if(Lcd_u8Row > LCD_NUM_ROWS)
	{
		/*Set the position into the line 0 and column 0 as the input is not valid*/
		Lcd_vidSendCommand(LCDSETPOSITIONLY0X0);
	}/*end of if(Copy_u8Row > LCD_NUM_ROWS)*/

	else
	{
		/*Check if the row is more than 1 then go to the base address*/
	    if(Lcd_u8Col > LCD_NUM_COLS)
	    {
	        /*Set the position into the line 0 and col 0 as the input is not valid*/
	        Lcd_vidSendCommand(LCDSETPOSITIONLY0X0);
	        //_delay_ms(2);
	    }/*end of if(Copy_u8Col > LCD_NUM_COLS)*/

	    else
	    {
	        if(Lcd_u8Row == ZERO)
	        {
	            /*Use the address of the first line*/
	            Lcd_vidSendCommand(LCD_FIRST_ROW_ADD | Lcd_u8Col);
	        }/*end of (Copy_u8Row == ZERO)*/

	        else
	        {
	            /*Use the address of the second line*/
	            Lcd_vidSendCommand(LCD_SECOND__ROW_ADD | Lcd_u8Col);
	        }/*end of else of (Copy_u8Row == ZERO)*/
	        //_delay_ms(2);
	    }/*end of else of if(Copy_u8Col > LCD_NUM_COLS)*/


	}/*end of else of if(Copy_u8Row > LCD_NUM_ROWS)*/

}


extern void DisplayString_Process(void)
{
	/*
	 * CreateCustomChar_Process has 9 states each state needs two states as
	 * it either calls Lcd_vidSendCommand or it calls Lcd_vidDisplayCharacter and both have 2 states:
	 * 1- Go to the location on the LCD.
	 * 2- Send the ith byte of the string to display character.
	 */

	/*Loc_enuDisplayStringState is used to save the display string state*/
	static Lcd_tenuSisplayStringStates Loc_enuDisplayStringState = Lcd_enuDisplayStringIdle;

	/*Loc_u8DSByteCounter is used to send byte by byte to the LCD*/
	static u8 Loc_u8DSByteCounter = ZERO;

	/*Switch over the display string states to check which part should be done now.*/
	switch(Loc_enuDisplayStringState)
	{
		case Lcd_enuDisplayStringIdle:
		{
			/*Set zero to Loc_u8DSByteCounter to indicate that the first byte will be sent.*/
			Loc_u8DSByteCounter = ZERO;

			/*Set Lcd_enuDisplayStringState0 to Loc_enuDisplayStringState for the next Tick*/
			Loc_enuDisplayStringState = Lcd_enuDisplayStringState0;

			/*Do the 1th part of the display string process*/
			/*Go to the location on the LCD*/
		    Lcd_vidGoTo(Lcd_u8Row, Lcd_u8Col);

			break;
		}/*end of case Lcd_enuDisplayStringIdle*/

		case Lcd_enuDisplayStringState0:
		{

		    /*Check the number of the byte which has been sent*/
		    if(Loc_u8DSByteCounter == Lcd_u8BufferLength - ONE)
		    {
				/*Set Loc_enuDisplayStringState to Loc_enuDisplayStringState for the next Tick*/
		    	Loc_enuDisplayStringState = Lcd_enuDisplayStringIdle;

			    /*Set Lcd_enuIdle to Lcd_enuState to indicate that the display string is done*/
			    Lcd_enuState = Lcd_enuIdle;
		    }/*end of if(Loc_u8DSByteCounter == Lcd_u8BufferLength - ONE)*/
		    else
		    {
		    	/*Do Nothing*/
		    }/*end of else of if(Loc_u8DSByteCounter == Lcd_u8BufferLength - ONE)*/

			/*Do the 2th part of the display string process*/
			/*Send the ith byte to be displayed to the LCD*/
		    Lcd_vidDisplayCharacter(Lcd_au8DataBuffer[Loc_u8DSByteCounter]);

		    /*Increase the byte index by one for the next tick.*/
		    Loc_u8DSByteCounter++;

			break;
		}/*end of case Lcd_enuDisplayStringState0*/

		default:
		{
			/*Error Handling*/
		}/*end of default*/

	}/*end of switch(Loc_enuDisplayStringState)*/
}


extern void CreateCustomChar_Process()
{
	/*
	 * CreateCustomChar_Process has 9 states each state needs two states as
	 * it either calls Lcd_vidSendCommand or it calls Lcd_vidDisplayCharacter and both have 2 states:
	 * 1- Send the address of the CGRAM as a command to save the character in it.
	 * 2- Send the 1th byte of the custom character as a display character.
	 * 3- Send the 2th byte of the custom character as a display character.
	 * 4- Send the 3th byte of the custom character as a display character.
	 * 5- Send the 4th byte of the custom character as a display character.
	 * 6- Send the 5th byte of the custom character as a display character.
	 * 7- Send the 6th byte of the custom character as a display character.
	 * 8- Send the 7th byte of the custom character as a display character.
	 * 9- Send the 8th byte of the custom character as a display character.
	 */

	/*Loc_enuCCCState is used to save the Create custom character state*/
	static Lcd_tenuCCCStates Loc_enuCCCState = Lcd_enuCCCIdle;

	/*Loc_u8CCCByteCounter is used to send byte by byte to the LCD*/
	static u8 Loc_u8CCCByteCounter = ZERO;

	/*Switch over the create custom character states to check which part should be done now.*/
	switch(Loc_enuCCCState)
	{
		case Lcd_enuCCCIdle:
		{
			/*Set zero to Loc_u8CCCByteCounter to indicate that the first byte will be sent.*/
			Loc_u8CCCByteCounter = ZERO;

			/*Set Lcd_enuCCCState0 to Loc_enuCCCState for the next Tick*/
			Loc_enuCCCState = Lcd_enuCCCState0;

			/*Do the 1th part of the create custom character process*/
			/*Send the address as a command to the LCD*/
		    Lcd_vidSendCommand(Lcd_u8Command);

			break;
		}/*end of case Lcd_enuCCCIdle*/

		case Lcd_enuCCCState0:
		{

		    /*Check the number of the byte which has been sent*/
		    if(Loc_u8CCCByteCounter == SEVEN)
		    {
				/*Set Lcd_enuCCCIdle to Loc_enuCCCState for the next Tick*/
				Loc_enuCCCState = Lcd_enuCCCIdle;

			    /*Set Lcd_enuIdle to Lcd_enuState to indicate that the create custom character is done*/
			    Lcd_enuState = Lcd_enuIdle;
		    }/*end of if(Loc_u8CCCByteCounter == SEVEN)*/
		    else
		    {
		    	/*Do Nothing*/
		    }/*end of else of if(Loc_u8CCCByteCounter == SEVEN)*/

			/*Do the 2th part of the create custom character process*/
			/*Send the ith byte to be displayed to the LCD*/
		    Lcd_vidDisplayCharacter(Lcd_au8DataBuffer[Loc_u8CCCByteCounter]);

		    /*Increase the byte index by one for the next tick.*/
		    Loc_u8CCCByteCounter++;

			break;
		}/*end of case Lcd_enuCCCState0*/

		default:
		{
			/*Error Handling*/
		}/*end of default*/

	}/*end of switch(Loc_enuCCCState)*/
}


#if INTERFACE_TYPE == 1


extern void SendCommand_Process(void)
{
	/*
	 * SendCommand_Process it has tow states:
	 * 1- Send the command and send high on the enable pin.
	 * 2- Send low on the enable pin.
	 */

	/*Loc_enuSendCommState is used to save the Send command state*/
	static Lcd_tenuSendCommandStates Loc_enuSendCommState = Lcd_enuSendCommIdle;

	/*Switch over the send command states to check which part should be done now*/
	switch(Loc_enuSendCommState)
	{
		case Lcd_enuSendCommIdle:
		{
			/*Create a union to send the data bit by bit on the bus*/
			Lcd_tunUnion Loc_union;

			/*Assign the buffered command into the byte in union*/
			Loc_union.Lcd_InByte = Lcd_u8Command;

			/*
			 * Send three signals of:
			 * 						Low R/W
			 * 						Low En
			 * 						High RS
			 */
			gpio_setPinValue(Lcd_Pins[LCD_READ_WRITE_PIN].Lcd_Port, Lcd_Pins[LCD_READ_WRITE_PIN].Lcd_Pin, LCD_OFF);
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_OFF);
			gpio_setPinValue(Lcd_Pins[LCD_REGISTERSELECT_PIN].Lcd_Port, Lcd_Pins[LCD_REGISTERSELECT_PIN].Lcd_Pin, LCD_OFF);

			/*Send the data bit by bit on the bus*/
			gpio_setPinValue(Lcd_Pins[LCD_D0_PIN].Lcd_Port, Lcd_Pins[LCD_D0_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b0);
			gpio_setPinValue(Lcd_Pins[LCD_D1_PIN].Lcd_Port, Lcd_Pins[LCD_D1_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b1);
			gpio_setPinValue(Lcd_Pins[LCD_D2_PIN].Lcd_Port, Lcd_Pins[LCD_D2_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b2);
			gpio_setPinValue(Lcd_Pins[LCD_D3_PIN].Lcd_Port, Lcd_Pins[LCD_D3_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b3);
			gpio_setPinValue(Lcd_Pins[LCD_D4_PIN].Lcd_Port, Lcd_Pins[LCD_D4_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b4);
			gpio_setPinValue(Lcd_Pins[LCD_D5_PIN].Lcd_Port, Lcd_Pins[LCD_D5_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b5);
			gpio_setPinValue(Lcd_Pins[LCD_D6_PIN].Lcd_Port, Lcd_Pins[LCD_D6_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b6);
			gpio_setPinValue(Lcd_Pins[LCD_D7_PIN].Lcd_Port, Lcd_Pins[LCD_D7_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b7);

			/*Send High Enable signal*/
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_ON);

			/*Set Lcd_enuSendComm0 to Loc_enuSendCommState for the next Tick*/
			Loc_enuSendCommState = Lcd_enuSendComm0;

			break;
		}/*end of case Lcd_enuSendCommIdle*/

		case Lcd_enuSendComm0:
		{
		    /*Send a Low Enable signal to the LCD*/
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_OFF);

			/*Set Lcd_enuSendComm0 to Loc_enuSendCommState for the next Tick*/
			Loc_enuSendCommState = Lcd_enuSendCommIdle;

			/*Reload the state that called the send command state*/
			Lcd_enuState = Lcd_enuPrvState;

			break;
		}/*end of case Lcd_enuSendComm0*/

		default:
		{
			/*Error Handling*/
			break;
		}/*end of default*/

	}/*end of switch(Loc_enuSendCommState)*/
}


extern void DisplayChar_Process(void)
{
	/*
	 * DisplayChar_Process it has tow states:
	 * 1- Send the Data and send high on the enable pin.
	 * 2- Send low on the enable pin.
	 */

	/*Loc_enuDisplayCharState is used to save the Send command state*/
	static Lcd_tenuDisplayCharStates Loc_enuDisplayCharState = Lcd_enuDisplayCharIdle;

	/*Switch over the Display Char states to check which part should be done now*/
	switch(Loc_enuDisplayCharState)
	{
		case Lcd_enuDisplayCharIdle:
		{
			/*Create a union to send the data bit by bit on the bus*/
			Lcd_tunUnion Loc_union;

			/*Assign the data into the byte in union*/
			Loc_union.Lcd_InByte = Lcd_u8Display;

			/*
			 * Send three signals of:
			 * 						Low R/W
			 * 						Low En
			 * 						High RS
			 */
			gpio_setPinValue(Lcd_Pins[LCD_READ_WRITE_PIN].Lcd_Port, Lcd_Pins[LCD_READ_WRITE_PIN].Lcd_Pin, LCD_OFF);
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_OFF);
			gpio_setPinValue(Lcd_Pins[LCD_REGISTERSELECT_PIN].Lcd_Port, Lcd_Pins[LCD_REGISTERSELECT_PIN].Lcd_Pin, LCD_ON);


			/*Send the data bit by bit on the bus*/
			gpio_setPinValue(Lcd_Pins[LCD_D0_PIN].Lcd_Port, Lcd_Pins[LCD_D0_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b0);
			gpio_setPinValue(Lcd_Pins[LCD_D1_PIN].Lcd_Port, Lcd_Pins[LCD_D1_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b1);
			gpio_setPinValue(Lcd_Pins[LCD_D2_PIN].Lcd_Port, Lcd_Pins[LCD_D2_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b2);
			gpio_setPinValue(Lcd_Pins[LCD_D3_PIN].Lcd_Port, Lcd_Pins[LCD_D3_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b3);
			gpio_setPinValue(Lcd_Pins[LCD_D4_PIN].Lcd_Port, Lcd_Pins[LCD_D4_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b4);
			gpio_setPinValue(Lcd_Pins[LCD_D5_PIN].Lcd_Port, Lcd_Pins[LCD_D5_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b5);
			gpio_setPinValue(Lcd_Pins[LCD_D6_PIN].Lcd_Port, Lcd_Pins[LCD_D6_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b6);
			gpio_setPinValue(Lcd_Pins[LCD_D7_PIN].Lcd_Port, Lcd_Pins[LCD_D7_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b7);

			/*Send High Enable signal*/
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_ON);

			/*Set Lcd_enuDisplayChar0 to Loc_enuDisplayCharState for the next Tick*/
			Loc_enuDisplayCharState = Lcd_enuDisplayChar0;

			break;
		}/*end of case Lcd_enuDisplayCharIdle*/

		case Lcd_enuDisplayChar0:
		{
		    /*Send a Low Enable signal to the LCD*/
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_OFF);

			/*Set Lcd_enuDisplayCharIdle to Loc_enuDisplayCharState for the next Tick*/
			Loc_enuDisplayCharState = Lcd_enuDisplayCharIdle;

			/*Reload the state that called the display char state*/
			Lcd_enuState = Lcd_enuPrvState;

			break;
		}/*end of case Lcd_enuDisplayChar0*/

		default:
		{
			/*Error Handling*/
			break;
		}/*end of default*/


	}/*end of switch(Loc_enuDisplayCharState)*/
}


#elif INTERFACE_TYPE == 0


extern void SendCommand_Process(void)
{
	/*
	 * SendCommand_Process it has three states:
	 * 1- Send the most nibble of the command and send high on the enable pin.
	 * 2- low the enable pin then Send the least nibble of the command and send high on the enable pin.
	 * 3- Send low on the enable pin.
	 */

	/*Loc_enuSendCommState is used to save the Send command state*/
	static Lcd_tenuSendCommandStates Loc_enuSendCommState = Lcd_enuSendCommIdle;

	/*Create a union to send the data bit by bit on the bus*/
	Lcd_tunUnion Loc_union;

	/*Assign the buffered command into the byte in union*/
	Loc_union.Lcd_InByte = Lcd_u8Command;

	/*Switch over the send command states to check which part should be done now*/
	switch(Loc_enuSendCommState)
	{
		case Lcd_enuSendCommIdle:
		{
			/*
			 * Send three signals of:
			 * 						Low R/W
			 * 						Low En
			 * 						High RS
			 */
			gpio_setPinValue(Lcd_Pins[LCD_READ_WRITE_PIN].Lcd_Port, Lcd_Pins[LCD_READ_WRITE_PIN].Lcd_Pin, LCD_OFF);
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_OFF);
			gpio_setPinValue(Lcd_Pins[LCD_REGISTERSELECT_PIN].Lcd_Port, Lcd_Pins[LCD_REGISTERSELECT_PIN].Lcd_Pin, LCD_OFF);

			/*Send the data bit by bit on the bus most nibble*/
			gpio_setPinValue(Lcd_Pins[LCD_D4_PIN].Lcd_Port, Lcd_Pins[LCD_D4_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b4);
			gpio_setPinValue(Lcd_Pins[LCD_D5_PIN].Lcd_Port, Lcd_Pins[LCD_D5_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b5);
			gpio_setPinValue(Lcd_Pins[LCD_D6_PIN].Lcd_Port, Lcd_Pins[LCD_D6_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b6);
			gpio_setPinValue(Lcd_Pins[LCD_D7_PIN].Lcd_Port, Lcd_Pins[LCD_D7_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b7);

			/*Send High Enable signal*/
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_ON);

			/*Set Lcd_enuSendComm0 to Loc_enuSendCommState for the next Tick*/
			Loc_enuSendCommState = Lcd_enuSendComm0;

			break;
		}/*end of case Lcd_enuSendCommIdle*/

		case Lcd_enuSendComm0:
		{
			/*Send a Low Enable signal to the LCD*/
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_OFF);

			/*Send the data bit by bit on the bus least nibble*/
			gpio_setPinValue(Lcd_Pins[LCD_D4_PIN].Lcd_Port, Lcd_Pins[LCD_D4_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b0);
			gpio_setPinValue(Lcd_Pins[LCD_D5_PIN].Lcd_Port, Lcd_Pins[LCD_D5_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b1);
			gpio_setPinValue(Lcd_Pins[LCD_D6_PIN].Lcd_Port, Lcd_Pins[LCD_D6_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b2);
			gpio_setPinValue(Lcd_Pins[LCD_D7_PIN].Lcd_Port, Lcd_Pins[LCD_D7_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b3);

			/*Send High Enable signal*/
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_ON);

			/*Set Lcd_enuSendComm0 to Loc_enuSendCommState for the next Tick*/
			Loc_enuSendCommState = Lcd_enuSendComm1;

			break;
		}/*end of case Lcd_enuSendComm0*/

		case Lcd_enuSendComm1:
		{
		    /*Send a Low Enable signal to the LCD*/
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_OFF);

			/*Set Lcd_enuSendComm0 to Loc_enuSendCommState for the next Tick*/
			Loc_enuSendCommState = Lcd_enuSendCommIdle;

			/*Check the Previous State of the LCD*/
			if(Lcd_enuPrvState == Lcd_enuGoTo)
			{
				/*Set Lcd_enuIdle to Lcd_enuState as the go to is just one Tick*/
				Lcd_enuState = Lcd_enuIdle;
			}/*end of if(Lcd_enuPrvState == Lcd_enuGoTo)*/
			else
			{
				/*Reload the state that called the send command state*/
				Lcd_enuState = Lcd_enuPrvState;
			}/*end of else of if(Lcd_enuPrvState == Lcd_enuGoTo)*/

			break;
		}/*end of case Lcd_enuSendComm1*/

		default:
		{
			/*Error Handling*/
			break;
		}/*end of default*/

	}/*end of switch(Loc_enuSendCommState)*/
}


extern void DisplayChar_Process(void)
{
	/*
	 * DisplayChar_Process it has three states:
	 * 1- Send the most nibble of the Data and send high on the enable pin.
	 * 2- low the enable pin then Send the least nibble of the data and send high on the enable pin.
	 * 3- Send low on the enable pin.
	 */

	/*Loc_enuDisplayCharState is used to save the Send command state*/
	static Lcd_tenuDisplayCharStates Loc_enuDisplayCharState = Lcd_enuDisplayCharIdle;

	/*Create a union to send the data bit by bit on the bus*/
	Lcd_tunUnion Loc_union;

	/*Assign the data into the byte in union*/
	Loc_union.Lcd_InByte = Lcd_u8Display;

	/*Switch over the Display Char states to check which part should be done now*/
	switch(Loc_enuDisplayCharState)
	{
		case Lcd_enuDisplayCharIdle:
		{
			/*
			 * Send three signals of:
			 * 						Low R/W
			 * 						Low En
			 * 						High RS
			 */
			gpio_setPinValue(Lcd_Pins[LCD_READ_WRITE_PIN].Lcd_Port, Lcd_Pins[LCD_READ_WRITE_PIN].Lcd_Pin, LCD_OFF);
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_OFF);
			gpio_setPinValue(Lcd_Pins[LCD_REGISTERSELECT_PIN].Lcd_Port, Lcd_Pins[LCD_REGISTERSELECT_PIN].Lcd_Pin, LCD_ON);


			/*Send the data bit by bit on the bus most nibble*/
			gpio_setPinValue(Lcd_Pins[LCD_D4_PIN].Lcd_Port, Lcd_Pins[LCD_D4_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b4);
			gpio_setPinValue(Lcd_Pins[LCD_D5_PIN].Lcd_Port, Lcd_Pins[LCD_D5_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b5);
			gpio_setPinValue(Lcd_Pins[LCD_D6_PIN].Lcd_Port, Lcd_Pins[LCD_D6_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b6);
			gpio_setPinValue(Lcd_Pins[LCD_D7_PIN].Lcd_Port, Lcd_Pins[LCD_D7_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b7);

			/*Send High Enable signal*/
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_ON);

			/*Set Lcd_enuDisplayChar0 to Loc_enuDisplayCharState for the next Tick*/
			Loc_enuDisplayCharState = Lcd_enuDisplayChar0;

			break;
		}/*end of case Lcd_enuDisplayCharIdle*/

		case Lcd_enuDisplayChar0:
		{
			/*Send a Low Enable signal to the LCD*/
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_OFF);


			/*Send the data bit by bit on the bus least nibble*/
			gpio_setPinValue(Lcd_Pins[LCD_D4_PIN].Lcd_Port, Lcd_Pins[LCD_D4_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b0);
			gpio_setPinValue(Lcd_Pins[LCD_D5_PIN].Lcd_Port, Lcd_Pins[LCD_D5_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b1);
			gpio_setPinValue(Lcd_Pins[LCD_D6_PIN].Lcd_Port, Lcd_Pins[LCD_D6_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b2);
			gpio_setPinValue(Lcd_Pins[LCD_D7_PIN].Lcd_Port, Lcd_Pins[LCD_D7_PIN].Lcd_Pin, Loc_union.Lcd_InBits.b3);

			/*Send High Enable signal*/
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_ON);

			/*Set Lcd_enuDisplayChar0 to Loc_enuDisplayCharState for the next Tick*/
			Loc_enuDisplayCharState = Lcd_enuDisplayChar1;

			break;
		}/*end of case Lcd_enuDisplayChar0*/

		case Lcd_enuDisplayChar1:
		{
		    /*Send a Low Enable signal to the LCD*/
			gpio_setPinValue(Lcd_Pins[LCD_ENABLE_PIN].Lcd_Port, Lcd_Pins[LCD_ENABLE_PIN].Lcd_Pin, LCD_OFF);

			/*Set Lcd_enuDisplayCharIdle to Loc_enuDisplayCharState for the next Tick*/
			Loc_enuDisplayCharState = Lcd_enuDisplayCharIdle;

			/*Reload the state that called the display char state*/
			Lcd_enuState = Lcd_enuPrvState;

			break;
		}/*end of case Lcd_enuDisplayChar1*/

		default:
		{
			/*Error Handling*/
			break;
		}/*end of default*/


	}/*end of switch(Loc_enuDisplayCharState)*/
}


#else


#error Invalid definition of INTERFACE_TYPE


#endif

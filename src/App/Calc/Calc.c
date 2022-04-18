/*
 * Calc.c
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */

#include "../../Lib/Std_types.h"
#include "../../Lib/Bit_utils.h"
#include "../../Mcal/Rcc/Rcc.h"
#include "../../Services/Sched/Sched.h"
#include "../../Hal/Lcd/Lcd.h"
#include "../../Hal/Lcd/Lcd_cfg.h"
#include "../../Hal/Keypad/Keypad.h"
#include "../../Hal/Keypad/Keypad_cfg.h"
#include "Calc.h"
#include "Calc_prv.h"
#include "Calc_cfg.h"
#include "diag/Trace.h"


/* ==================================================================== */
/* ========================== Variables  ============================== */
/* ==================================================================== */
CalcApp_enuOperandState CalcApp_enuCurrentState = CalcApp_enuStartState;
CalcApp_strInputData    CalcApp_strCurrInputData;
CalcApp_strOutputData   CalcApp_strCurrOutputData;
CalcApp_enuErrorStatus  CalcApp_ErrorState = CalcApp_OK;

static u8 CalcApp_u8Sign = 0;

static u8 CalcApp_au8DivByZero[11] = {'D', 'i', 'v', ' ', 'B', 'y', ' ', 'Z', 'e', 'r', 'o'};

/* ==================================================================== */
/* =================== Function Implementation ======================== */
/* ==================================================================== */

static void CalcApp_vidStartProcess();

static void CalcApp_vidGetOperandOneProcess();

static void CalcApp_vidGetOperandTwoProcess();

CalcApp_enuErrorStatus CalcApp_CalcResult(void);

void CalcApp_vidDisplayResultProcess(void);

void CalcApp_vidDone(void);

void CalcApp_vidDisplayOperand1(void);

void CalcApp_vidTask(){

        
    switch(CalcApp_enuCurrentState){ 
    
        case CalcApp_enuStartState: 
             CalcApp_vidStartProcess();
        break;
        
        case CalcApp_enuGetOperandOneState:
             CalcApp_vidGetOperandOneProcess();
        break;

        case CalcApp_enuGetOperandTwoState:
             CalcApp_vidGetOperandTwoProcess();
        break;

		case CalcApp_enuCalculateResultState: /*calculate the result */
        	CalcApp_ErrorState = CalcApp_CalcResult();
        	break;

		case CalcApp_enuDisplayResultState:
			CalcApp_vidDisplayResultProcess();
        break;

		case CalcApp_enuDone:
		{
			CalcApp_vidDone();
			break;
		}

		case CalcApp_enuDisplayOperand1State:
		{
			CalcApp_vidDisplayOperand1();
			break;
		}

		default:
		{
			break;
		}
    }
    
}

void CalcApp_Init(void) {
	CalcApp_strCurrInputData.CalcApp_strOperand1 = 0;
	CalcApp_strCurrInputData.CalcApp_strOperand2 = 0;
	CalcApp_strCurrInputData.CalcApp_strOperation = 0;
	CalcApp_strCurrOutputData.CalcApp_strResult = 0;
            
    RCC_enuEnablePreipheral      (AHB1_BUS, GPIOA_RCC);
	RCC_enuEnablePreipheral      (AHB1_BUS, GPIOB_RCC);

	Keypad_enuInit();
	Lcd_vidInit();
	
    Runnable_t Loc_runnable;
	Loc_runnable.CyclicTimems = 100;
	Loc_runnable.cbfP = CalcApp_vidTask;
	Sched_RegisterRunnable(&Loc_runnable, 2);
    
    Sched_vidInit();	
	Sched_vidStart();

}


static void CalcApp_vidStartProcess(){
    
    Keypad_tenuErrorStatus Loc_enuErrorStatus = Keypad_enuNotOk;
    
    u8 Loc_u8PressedValue;

    Loc_enuErrorStatus = Keypad_enuGetKey(&Loc_u8PressedValue);
    
    if(Loc_enuErrorStatus == Keypad_enuOk ){ 
        
        /* GET A DIGIT */ 
        if(Loc_u8PressedValue >= '0' && Loc_u8PressedValue <= '9'){
            
            /* BUFFER THE OPERATION */
            CalcApp_strCurrInputData.CalcApp_strOperand1 = Loc_u8PressedValue  - '0';

            /*Display the digit*/
            Lcd_vidDisplayCharacter(Loc_u8PressedValue);

            /* CHANGE THE CURRENT STATE */
            CalcApp_enuCurrentState = CalcApp_enuGetOperandOneState;
        }


        /* GET MINUS */
        else if(Loc_u8PressedValue == '-'){
            
            /* ASSIGN THE SIGN */
            CalcApp_u8Sign = 1;

    		/* display negative sign in case of negative number in operand one */
    		Lcd_vidDisplayCharacter('-');	// '-' isn't the cute emoji

            /* CHANGE THE CURRENT STATE */
            CalcApp_enuCurrentState = CalcApp_enuGetOperandOneState;

        }
        
        else{
            /* DO NOTHING */ 
        }
    }                
}
static void CalcApp_vidGetOperandOneProcess(){
	static u8 Loc_u8FirstEnter = 0;

    Keypad_tenuErrorStatus Loc_enuErrorStatus = Keypad_enuNotOk;
    
    u8 Loc_u8PressedValue;

    Loc_enuErrorStatus = Keypad_enuGetKey(&Loc_u8PressedValue);
    
    if(GET_BIT(Loc_u8FirstEnter,0) == 0)
    {
    	SET_BIT(Loc_u8FirstEnter,0);
    	if(CalcApp_u8Sign == 1)
    	{
    		SET_BIT(Loc_u8FirstEnter,4);
    	}
    }

    if(Loc_enuErrorStatus == Keypad_enuOk ){ 
    
        if(Loc_u8PressedValue >= '0' && Loc_u8PressedValue <= '9'){

        	CLR_BIT(Loc_u8FirstEnter,4);

            /* BUFFER THE OPERATION */
            
            CalcApp_strCurrInputData.CalcApp_strOperand1 = ((10 * CalcApp_strCurrInputData.CalcApp_strOperand1) + Loc_u8PressedValue - '0');


            /*Display the digit*/
            Lcd_vidDisplayCharacter(Loc_u8PressedValue);
        }

        else if((GET_BIT(Loc_u8FirstEnter,4) == 0) && (Loc_u8PressedValue == '-' || Loc_u8PressedValue == '+' || Loc_u8PressedValue == '*' || Loc_u8PressedValue == '/')){
            CLR_BIT(Loc_u8FirstEnter,0);

			/* BUFFER THE OPERATION */
			CalcApp_strCurrInputData.CalcApp_strOperation = Loc_u8PressedValue;

			/* MINUS OPERATION */
			if(CalcApp_u8Sign == 1)
			{
				CalcApp_strCurrInputData.CalcApp_strOperand1 *= -1;
			}

			/*Display the operator*/
			Lcd_vidDisplayCharacter(Loc_u8PressedValue);

			/* CHANGE THE CURRENT STATE */
			CalcApp_enuCurrentState = CalcApp_enuGetOperandTwoState;

			CalcApp_u8Sign = 0;

        }

        else{
            /* DO NOTHING */ 
        }
    }
}



static void CalcApp_vidGetOperandTwoProcess(){
    static u8 Loc_u8GotDigit = 0;

    Keypad_tenuErrorStatus Loc_enuErrorStatus = Keypad_enuNotOk;
    
    u8 Loc_u8PressedValue;

    Loc_enuErrorStatus = Keypad_enuGetKey(&Loc_u8PressedValue);
    
    if(Loc_enuErrorStatus == Keypad_enuOk ){ 
        if(Loc_u8PressedValue >= '0' && Loc_u8PressedValue <= '9'){
            Loc_u8GotDigit = 1;

            /* BUFFER THE OPERATION */
            
            CalcApp_strCurrInputData.CalcApp_strOperand2 = ((10 * CalcApp_strCurrInputData.CalcApp_strOperand2) + Loc_u8PressedValue - '0');

            
            /*Display the digit*/
            Lcd_vidDisplayCharacter(Loc_u8PressedValue);

        }
        
        /*Check if operand two is zero and pressed key is - and the operator is also -*/
        else if(Loc_u8GotDigit == 0 && Loc_u8PressedValue == '-')
        {
        	/*Check if the operator equal '-' just ignore the input*/
        	if(CalcApp_strCurrInputData.CalcApp_strOperation == '-')
        	{
        		/*Do Nothing*/
        	}
        	else if(CalcApp_u8Sign == 1)
        	{
        		/*Do Nothing*/
        	}
        	/*raise the negative flag*/
        	else
        	{
        		/*Display '-'*/
        		Lcd_vidDisplayCharacter('-');

        		CalcApp_u8Sign = 1;
        	}
        }

        else if((Loc_u8PressedValue == '-' || Loc_u8PressedValue == '+' || Loc_u8PressedValue == '*' || Loc_u8PressedValue == '/') && (Loc_u8GotDigit == 1)){
            Loc_u8GotDigit = 0;

        	switch(CalcApp_strCurrInputData.CalcApp_strOperation)
        	{
				case '+':
				{
					CalcApp_strCurrInputData.CalcApp_strOperand1 = CalcApp_strCurrInputData.CalcApp_strOperand1 + CalcApp_strCurrInputData.CalcApp_strOperand2;

					CalcApp_strCurrInputData.CalcApp_strOperation = Loc_u8PressedValue;

	                /* CHANGE THE CURRENT STATE */
	                CalcApp_enuCurrentState = CalcApp_enuDisplayOperand1State;

	                /*Clear the LCD*/
	                Lcd_vidSendCommand(ONE);

	                CalcApp_strCurrInputData.CalcApp_strOperand2 = 0;

					break;
				}

				case '-':
				{
					CalcApp_strCurrInputData.CalcApp_strOperand1 = CalcApp_strCurrInputData.CalcApp_strOperand1 - CalcApp_strCurrInputData.CalcApp_strOperand2;

					CalcApp_strCurrInputData.CalcApp_strOperation = Loc_u8PressedValue;

	                /* CHANGE THE CURRENT STATE */
	                CalcApp_enuCurrentState = CalcApp_enuDisplayOperand1State;

	                /*Clear the LCD*/
	                Lcd_vidSendCommand(ONE);

	                CalcApp_strCurrInputData.CalcApp_strOperand2 = 0;

					break;
				}

				case '*':
				{
					CalcApp_strCurrInputData.CalcApp_strOperand1 = CalcApp_strCurrInputData.CalcApp_strOperand1 * CalcApp_strCurrInputData.CalcApp_strOperand2;

					CalcApp_strCurrInputData.CalcApp_strOperation = Loc_u8PressedValue;

	                /* CHANGE THE CURRENT STATE */
	                CalcApp_enuCurrentState = CalcApp_enuDisplayOperand1State;

	                /*Clear the LCD*/
	                Lcd_vidSendCommand(ONE);

	                CalcApp_strCurrInputData.CalcApp_strOperand2 = 0;

					break;
				}

				case '/':
				{
					if(CalcApp_strCurrInputData.CalcApp_strOperand2 != ZERO)
					{
						CalcApp_strCurrInputData.CalcApp_strOperand1 = CalcApp_strCurrInputData.CalcApp_strOperand1 / CalcApp_strCurrInputData.CalcApp_strOperand2;

						CalcApp_strCurrInputData.CalcApp_strOperation = Loc_u8PressedValue;

		                /* CHANGE THE CURRENT STATE */
		                CalcApp_enuCurrentState = CalcApp_enuDisplayOperand1State;

		                /*Clear the LCD*/
		                Lcd_vidSendCommand(ONE);

		                CalcApp_strCurrInputData.CalcApp_strOperand2 = 0;
					}
					else
					{
						Lcd_vidDisplayString(CalcApp_au8DivByZero, 11, 1, 0);

						/* update current state */
						CalcApp_enuCurrentState = CalcApp_enuDone;
					}

					break;
				}

				default:
				{
					break;
				}
        	}
        }

        else if(Loc_u8PressedValue == '=' && Loc_u8GotDigit == 1)
        {
        	Loc_u8GotDigit = 0;

        	CalcApp_enuCurrentState = CalcApp_enuCalculateResultState;

            /* MINUS OPERATION */
            if(CalcApp_u8Sign == 1)
            {
            	CalcApp_strCurrInputData.CalcApp_strOperand2 *= -1;

            	CalcApp_u8Sign = 0;
            }
        }

        else{

        }
    }
}

/*
 * Name:  CalcApp_CalcResult
 *
 * InputArguments: None
 *
 * Return Value: CalcApp_enuErrorStatus - return enumeration of the error status
 *
 * Description: calculate the result according to selected operation
 *
 */
CalcApp_enuErrorStatus CalcApp_CalcResult(void) {

	CalcApp_enuErrorStatus ErrorStatus = CalcApp_OK;
	/*
	 * check the operation type
	 */
	switch (CalcApp_strCurrInputData.CalcApp_strOperation) {
	case '+':
		CalcApp_strCurrOutputData.CalcApp_strResult =
				CalcApp_strCurrInputData.CalcApp_strOperand1
						+ CalcApp_strCurrInputData.CalcApp_strOperand2;


		break;
	case '-':
		CalcApp_strCurrOutputData.CalcApp_strResult =
				CalcApp_strCurrInputData.CalcApp_strOperand1
						- CalcApp_strCurrInputData.CalcApp_strOperand2;
		break;
	case '*':
		CalcApp_strCurrOutputData.CalcApp_strResult =
				CalcApp_strCurrInputData.CalcApp_strOperand1
						* CalcApp_strCurrInputData.CalcApp_strOperand2;

		break;

	case '/':

		if (CalcApp_strCurrInputData.CalcApp_strOperand2 != 0) {

			CalcApp_strCurrOutputData.CalcApp_strResult =
					(CalcApp_strCurrInputData.CalcApp_strOperand1 * 1000)
							/ CalcApp_strCurrInputData.CalcApp_strOperand2;

		} else {
			Lcd_vidDisplayString(CalcApp_au8DivByZero, 11, 1, 0);

			/* update current state */
			CalcApp_enuCurrentState = CalcApp_enuDone;

			ErrorStatus = CalcApp_NOK_DivByZero;
		}
		break;
	default:

		ErrorStatus = CalcApp_NOK;
		break;
	}

	/*
	 * set state to the next state to display results
	 */
		if(CalcApp_enuCurrentState != CalcApp_enuDone)
			CalcApp_enuCurrentState = CalcApp_enuDisplayResultState;

	return ErrorStatus;
}


void CalcApp_vidDisplayResultProcess(void)
{
	u8 Loc_u8Temp;
	s32 Loc_s32Temp = (CalcApp_strCurrOutputData.CalcApp_strResult);
	u8 Loc_arrTemp [16] = {0};
	u8 index = 1;
	u8 i = 0;

	/* negativeFlag for result */
	u8 negativeFlag = (Loc_s32Temp < 0);

	/* check for the sign of the result */
	if ((negativeFlag)){

		/*Add '-' in the first element in the array*/
		Loc_arrTemp[ZERO] = '-';

		/* get rid off minus sign */
		Loc_s32Temp *= (-1);
	}
	if(Loc_s32Temp == 0)
	{
		Loc_arrTemp[0] = '0';
		Lcd_vidDisplayString(Loc_arrTemp,1,1,2);
	}
	else{
		/* store digits of result */
		while (Loc_s32Temp) {
			Loc_u8Temp = Loc_s32Temp % 10;
			Loc_arrTemp[index++] = Loc_u8Temp;
			Loc_s32Temp /= 10;
		}

		/* display digits after decimal point */
		if((index & ONE) == ONE)
		{
			for (i = 1; i <= (index >> 1); i++) {
				Loc_u8Temp = Loc_arrTemp[i];
				Loc_arrTemp[i] = Loc_arrTemp[index - i];
				Loc_arrTemp[index - i] = Loc_u8Temp;
			}
		}
		else
		{
			for (i = 1; i < (index >> 1); i++) {
				Loc_u8Temp = Loc_arrTemp[i];
				Loc_arrTemp[i] = Loc_arrTemp[index - i];
				Loc_arrTemp[index - i] = Loc_u8Temp;
			}
		}


		for(i = 1; i < index; i++)
		{
			Loc_arrTemp[i] += '0';
		}

		if(CalcApp_strCurrInputData.CalcApp_strOperation == '/')
		{
			if (index <= FOUR) {
				for(i = index; i > ONE; i--)
				{
					Loc_arrTemp[i + 1] = Loc_arrTemp[i - 1];
				}
				Loc_arrTemp[ONE] = '0';
				Loc_arrTemp[TWO] = '.';

				if(negativeFlag)
				{
					Lcd_vidDisplayString(Loc_arrTemp, index + 2, 1, 2);
				}
				else
				{
					Lcd_vidDisplayString(&(Loc_arrTemp[1]), index + 1, 1, 2);
				}

			}
			else
			{
				for(i = index; i > (index - THREE); i--)
				{
					Loc_arrTemp[i] = Loc_arrTemp[i - 1];
				}

				Loc_arrTemp[i] = '.';

				if(negativeFlag)
				{
					Lcd_vidDisplayString(Loc_arrTemp, index + 1, 1, 2);
				}
				else
				{
					Lcd_vidDisplayString(&(Loc_arrTemp[1]), index, 1, 2);
				}
			}



		}
		else
		{
			if(negativeFlag)
			{
				Lcd_vidDisplayString(Loc_arrTemp, index, 1, 2);
			}
			else
			{
				Lcd_vidDisplayString(&(Loc_arrTemp[1]), index - ONE, 1, 2);
			}
		}
	}
		/* update current state */
		CalcApp_enuCurrentState = CalcApp_enuDone;

}

void CalcApp_vidDisplayOperand1(void)
{
	u8 Loc_u8Temp;
	s32 Loc_s32Temp = (CalcApp_strCurrInputData.CalcApp_strOperand1);
	u8 Loc_arrTemp [16] = {0};
	u8 index = 1;
	u8 i = 0;

	/* negativeFlag for result */
	u8 negativeFlag = (Loc_s32Temp < 0);

	/* check for the sign of the result */
	if ((negativeFlag)){

		/*Add '-' in the first element in the array*/
		Loc_arrTemp[ZERO] = '-';

		/* get rid off minus sign */
		Loc_s32Temp *= (-1);
	}

	if(Loc_s32Temp == 0)
	{
		Loc_arrTemp[0] = '0';
		Loc_arrTemp[1] = CalcApp_strCurrInputData.CalcApp_strOperation;
		Lcd_vidDisplayString(Loc_arrTemp,2,0,0);
	}
	else{
		/* store digits of result */
		while (Loc_s32Temp) {
			Loc_u8Temp = Loc_s32Temp % 10;
			Loc_arrTemp[index++] = Loc_u8Temp;
			Loc_s32Temp /= 10;
		}

		/* display digits after decimal point */
		if((index & ONE) == ONE)
		{
			for (i = 1; i <= (index >> 1); i++) {
				Loc_u8Temp = Loc_arrTemp[i];
				Loc_arrTemp[i] = Loc_arrTemp[index - i];
				Loc_arrTemp[index - i] = Loc_u8Temp;
			}
		}
		else
		{
			for (i = 1; i < (index >> 1); i++) {
				Loc_u8Temp = Loc_arrTemp[i];
				Loc_arrTemp[i] = Loc_arrTemp[index - i];
				Loc_arrTemp[index - i] = Loc_u8Temp;
			}
		}


		for(i = 1; i < index; i++)
		{
			Loc_arrTemp[i] += '0';
		}

		Loc_arrTemp[index++] = CalcApp_strCurrInputData.CalcApp_strOperation;

		if(negativeFlag)
		{
			Lcd_vidDisplayString(Loc_arrTemp, index, 0, 0);
		}
		else
		{
			Lcd_vidDisplayString(&(Loc_arrTemp[1]), index - ONE, 0, 0);
		}
	}
	/* update current state */
	CalcApp_enuCurrentState = CalcApp_enuGetOperandTwoState;
}

void CalcApp_vidDone(void)
{
    Keypad_tenuErrorStatus Loc_enuErrorStatus = Keypad_enuNotOk;

    u8 Loc_u8PressedValue;

    Loc_enuErrorStatus = Keypad_enuGetKey(&Loc_u8PressedValue);

    if(Loc_enuErrorStatus == Keypad_enuOk )
	{
    	if(Loc_u8PressedValue == '%')
    	{
    		Lcd_vidSendCommand(1);

    		/* update current state */
    		CalcApp_enuCurrentState = CalcApp_enuStartState;

    		CalcApp_strCurrInputData.CalcApp_strOperand1 = 0;
    		CalcApp_strCurrInputData.CalcApp_strOperand2 = 0;
    		CalcApp_strCurrInputData.CalcApp_strOperation = 0;
    		CalcApp_strCurrOutputData.CalcApp_strResult = 0;

    		CalcApp_u8Sign = 0;
    	}
	}
}

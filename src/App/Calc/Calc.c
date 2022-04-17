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


/* ==================================================================== */
/* ========================== Variables  ============================== */
/* ==================================================================== */
CalcApp_enuOperandState CalcApp_enuCurrentState = CalcApp_enuStartState;
CalcApp_strInputData    CalcApp_strCurrInputData;
CalcApp_strOutputData   CalcApp_strCurrOutputData;
CalcApp_enuErrorStatus  CalcApp_ErrorState = CalcApp_OK;

static u8 CalcApp_u8Sign = 0;

/* ==================================================================== */
/* =================== Function Implementation ======================== */
/* ==================================================================== */

static void CalcApp_vidStartProcess();

static void CalcApp_vidGetOperandOneProcess();

static void CalcApp_vidDisplayOperandOneProcess();

static void CalcApp_vidDisplayOperationProcess();

static void CalcApp_vidGetOperandTwoProcess();

static void CalcApp_vidDisplayOperandTwoProcess();

CalcApp_enuErrorStatus CalcApp_CalcResult(void);

void CalcApp_vidTask(){

        
    switch(CalcApp_enuCurrentState){ 
    
        case CalcApp_enuStartState: 
             CalcApp_vidStartProcess();
        break;
        
        case CalcApp_enuGetOperandOneState:
             CalcApp_vidGetOperandOneProcess();
        break;
        
        case CalcApp_enuDisplayOperandOneState:
        	CalcApp_vidDisplayOperandOneProcess();
        	break;

        case CalcApp_enuDisplayOperationState:
        	CalcApp_vidDisplayOperationProcess();
        	break;

        case CalcApp_enuGetOperandTwoState:
             CalcApp_vidGetOperandTwoProcess();
        break;

        case CalcApp_enuDisplayOperandTwoState:
			CalcApp_vidDisplayOperandTwoProcess();
			break;

		case CalcApp_enuCalculateResultState: /*calculate the result */
        	CalcApp_ErrorState = CalcApp_CalcResult();
        break;

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
	CalcApp_strCurrOutputData.CalcApp_strFloatNumFlag = CALC_APP_FLOAT_NUM_FLAG_NEXSIT;\
            
    RCC_enuEnablePreipheral      (AHB1_BUS, GPIOA_RCC);
	RCC_enuEnablePreipheral      (AHB1_BUS, GPIOB_RCC);
	RCC_enuEnablePreipheral      (AHB1_BUS, GPIOC_RCC);
	RCC_enuEnablePreipheral      (AHB1_BUS, GPIOD_RCC);

	Keypad_enuInit();
	Lcd_vidInit();
	
    Runnable_t Loc_runnable;
	Loc_runnable.CyclicTimems = 100;
	Loc_run.cbfP = CalcApp_vidTask;
	Sched_RegisterRunnable(&Loc_run, 2);
    
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
            CalcApp_strCurrInputData.CalcApp_strOperand1 = Loc_u8PressedValue; 
            
            CalcApp_enuCurrentState = CalcApp_enuDisplayOperandOneState;
        }
        
        /* GET MINUS */
        else if(Loc_u8PressedValue == '-'){
            
            /* ASSIGN THE SIGN */
            CalcApp_u8Sign = 1;
        }
        
        else{
            /* DO NOTHING */ 
        }
    }                
}
static void CalcApp_vidGetOperandOneProcess(){
    
    Keypad_tenuErrorStatus Loc_enuErrorStatus = Keypad_enuNotOk;
    
    u8 Loc_u8PressedValue;

    Loc_enuErrorStatus = Keypad_enuGetKey(&Loc_u8PressedValue);
    
    if(Loc_enuErrorStatus == Keypad_enuOk ){ 
    
        if(Loc_u8PressedValue >= '0' && Loc_u8PressedValue <= '9'){
                        
            /* BUFFER THE OPERATION */
            
            /* MINUS OPERATION */ 
            if(CalcApp_u8Sign == 1){
                CalcApp_strCurrInputData.CalcApp_strOperand1 = ((10 * CalcApp_strCurrInputData.CalcApp_strOperand1) + Loc_u8PressedValue) * -1;  
                CalcApp_u8Sign = 0;
            }
            
            else{
                CalcApp_strCurrInputData.CalcApp_strOperand1 = ((10 * CalcApp_strCurrInputData.CalcApp_strOperand1) + Loc_u8PressedValue);  
            }
            
            /* CHANGE THE CURRENT STATE */ 
            CalcApp_enuCurrentState = CalcApp_enuDisplayOperandOneState;

        }

        else if(Loc_u8PressedValue == '-' || Loc_u8PressedValue == '+' || Loc_u8PressedValue == '*' || Loc_u8PressedValue == '/'){
            
            /* BUFFER THE OPERATION */
            CalcApp_strCurrInputData.CalcApp_strOperation = Loc_u8PressedValue; 
            
            /* CHANGE THE CURRENT STATE */ 
            CalcApp_enuCurrentState = CalcApp_enuDisplayOperationState;
        }

        else{
            /* DO NOTHING */ 
        }
    }
}

static void CalcApp_vidDisplayOperandOneProcess()
{
	u8 Loc_u8Temp;

	/* firstEntrant flag for the process */
	static u8 firstEntrant = 1;

	/* negativeFlag for operand one */
	u8 negativeFlag = ((CalcApp_strCurrInputData.CalcApp_strOperand1) < 0);

	/* check for the sign of operand one */
	if ((negativeFlag) && (firstEntrant)){

		/* invalidate flag of firstEntrant for the process */
		firstEntrant = 0;

		/* display negative sign in case of negative number in operand one */
		Lcd_vidDisplayCharacter('-');	// '-' isn't the cute emoji
	}
	/* Extract one digit of operand one to be displayed */
	Loc_u8Temp = (negativeFlag) ?
			/* get rid off the minus sign in case of negative number */
			(u8)(((CalcApp_strCurrInputData.CalcApp_strOperand1) % 10) * -1)
			: (u8)((CalcApp_strCurrInputData.CalcApp_strOperand1) % 10);

	/* display one digit of operand one */
	Lcd_vidDisplayCharacter(Loc_u8Temp);

	/* update current state */
	CalcApp_enuCurrentState = CalcApp_enuGetOperandOneState;
}

static void CalcApp_vidDisplayOperationProcess()
{
	/* display operation symbol */
	Lcd_vidDisplayCharacter(CalcApp_strCurrInputData.CalcApp_strOperation);

	/* update current state */
	CalcApp_enuCurrentState = CalcApp_enuGetOperandTwoState;
}

static void CalcApp_vidGetOperandTwoProcess(){
    
    Keypad_tenuErrorStatus Loc_enuErrorStatus = Keypad_enuNotOk;
    
    u8 Loc_u8PressedValue;

    Loc_enuErrorStatus = Keypad_enuGetKey(&Loc_u8PressedValue);
    
    if(Loc_enuErrorStatus == Keypad_enuOk ){ 
        if(Loc_u8PressedValue >= '0' && Loc_u8PressedValue <= '9'){
                        
            /* BUFFER THE OPERATION */
            
            /* MINUS OPERATION */ 
            if(CalcApp_u8Sign == 1){
                CalcApp_strCurrInputData.CalcApp_strOperand2 = ((10 * CalcApp_strCurrInputData.CalcApp_strOperand2) + Loc_u8PressedValue) * -1;  
                CalcApp_u8Sign = 0;
            }
            
            else{
                CalcApp_strCurrInputData.CalcApp_strOperand2 = ((10 * CalcApp_strCurrInputData.CalcApp_strOperand2) + Loc_u8PressedValue);  
            }
            
            /* CHANGE THE CURRENT STATE */ 
            CalcApp_enuCurrentState = CalcApp_enuDisplayOperandTwoState;

        }
        

        else if(Loc_u8PressedValue == '-' || Loc_u8PressedValue == '+' || Loc_u8PressedValue == '*' || Loc_u8PressedValue == '/'){
            
            /* BUFFER THE OPERATION */
            CalcApp_strCurrInputData.CalcApp_strOperation = Loc_u8PressedValue; 
            
            /* CHANGE THE CURRENT STATE */ 
            CalcApp_enuCurrentState = CalcApp_enuDisplayOperationState;
        }
        
        /* OPERATOR IS ALREADY ASSIGNED */
        else if(CalcApp_strCurrInputData.CalcApp_strOperation == '-' || CalcApp_strCurrInputData.CalcApp_strOperation == '+' || CalcApp_strCurrInputData.CalcApp_strOperation == '*' ||CalcApp_strCurrInputData.CalcApp_strOperation == '/' ){
            
            /* CURRENT OPERATION IS A MINUS */ 
            if(Loc_u8PressedValue == '-'){
                CalcApp_u8Sign = 1;
            }
            
        }

        else{
            /* DO NOTHING */ 
        }
    }
}

static void CalcApp_vidDisplayOperandTwoProcess()
{
	u8 Loc_u8Temp;

	/* firstEntrant flag for the process */
	static u8 firstEntrant = 1;

	/* negativeFlag for operand two */
	u8 negativeFlag = ((CalcApp_strCurrInputData.CalcApp_strOperand2) < 0);

	/* check for the sign of operand two */
	if ((negativeFlag) && (firstEntrant)){

		/* invalidate flag of firstEntrant for the process */
		firstEntrant = 0;

		/* display negative sign in case of negative number in operand two */
		Lcd_vidDisplayCharacter('-');	// '-' isn't the cute emoji
	}
	/* Extract one digit of operand two to be displayed */
	Loc_u8Temp = (negativeFlag) ?
			/* get rid off the minus sign in case of negative number */
			(u8)(((CalcApp_strCurrInputData.CalcApp_strOperand2) % 10) * -1)
			: (u8)((CalcApp_strCurrInputData.CalcApp_strOperand2) % 10);

	/* display one digit of operand two */
	Lcd_vidDisplayCharacter(Loc_u8Temp);

	/* update current state */
	CalcApp_enuCurrentState = CalcApp_enuDone;
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
					(CalcApp_strCurrInputData.CalcApp_strOperand1 * 10000)
							/ CalcApp_strCurrInputData.CalcApp_strOperand2;
			CalcApp_strCurrOutputData.CalcApp_strFloatNumFlag =
					CALC_APP_FLOAT_NUM_FLAG_EXIST;
		} else {
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

		CalcApp_enuCurrentState = CalcApp_enuDisplayResultState;

	return ErrorStatus;
}

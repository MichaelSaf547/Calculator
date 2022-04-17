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

static void CalcApp_vidGetOperandTwoProcess();

CalcApp_enuErrorStatus CalcApp_CalcResult(void);

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
	CalcApp_strCurrOutputData.CalcApp_strFloatNumFlag =
			CALC_APP_FLOAT_NUM_FLAG_NEXSIT;
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

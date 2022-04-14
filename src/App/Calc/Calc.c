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

static u8 CalcApp_u8Sign = 0;

/* ==================================================================== */
/* =================== Function Implementation ======================== */
/* ==================================================================== */

void CalcApp_vidTask(){

    static Keypad_tenuErrorStatus Loc_enuErrorStatus = Keypad_enuNotOk;
    
    Loc_enuErrorStatus = Keypad_enuGetKey(&Loc_u8PressedValue);
    
    if(Loc_enuErrorStatus == Keypad_enuOk ){        
        
        switch(CalcApp_enuCurrentState){ 
        
            case CalcApp_enuStartState: 
                 CalcApp_vidStartProcess(Loc_enuErrorStatus);
            break;
            
            case CalcApp_enuGetOperandOneState:
                 CalcApp_vidGetOperandOneProcess(Loc_enuErrorStatus);
            break;
            
            case CalcApp_enuGetOperandTwoState:
                 CalcApp_vidGetOperandTWOProcess(Loc_enuErrorStatus);
            break;
        }
    }    
        
}

static void CalcApp_vidStartProcess(u8 Copy_u8KeyValue){
    
    /* GET A DIGIT */ 
    if(Copy_u8KeyValue >= '0' && Copy_u8KeyValue <= '9'){
        
        /* BUFFER THE OPERATION */
        Loc_s16CurrOperand = Copy_u8KeyValue;
        CalcApp_strCurrInputData.CalcApp_strOperand1 = Copy_u8KeyValue; 
        
        CalcApp_enuCurrentState = CalcApp_enuDisplayOperandOneState;
    }
    
    /* GET MINUS */
    else if(Copy_u8KeyValue == '-'){
        
        /* ASSIGN THE SIGN */
        CalcApp_u8Sign = 1;
    }
    
    else{
        /* DO NOTHING */ 
    }
                
}
static void CalcApp_vidGetOperandOneProcess(u8 Copy_u8KeyValue){
    
    if(Copy_u8KeyValue >= '0' && Copy_u8KeyValue <= '9'){
                    
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

    else if(Copy_u8KeyValue == '-' || Copy_u8KeyValue == '+' || Copy_u8KeyValue == '*' || Copy_u8KeyValue == '/'){
        
        /* BUFFER THE OPERATION */
        CalcApp_strCurrInputData.CalcApp_strOperation = Copy_u8KeyValue; 
        
        /* CHANGE THE CURRENT STATE */ 
        CalcApp_enuCurrentState = CalcApp_enuDisplayOperationState;
    }

    else{
        /* DO NOTHING */ 
    }
}
static void CalcApp_vidGetOperandTwoProcess(u8 Copy_u8KeyValue){
    
    if(Copy_u8KeyValue >= '0' && Copy_u8KeyValue <= '9'){
                    
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
        CalcApp_enuCurrentState = CalcApp_enuDisplayOperandOneState;

    }
    

    else if(Copy_u8KeyValue == '-' || Copy_u8KeyValue == '+' || Copy_u8KeyValue == '*' || Copy_u8KeyValue == '/'){
        
        /* BUFFER THE OPERATION */
        CalcApp_strCurrInputData.CalcApp_strOperation = Copy_u8KeyValue; 
        
        /* CHANGE THE CURRENT STATE */ 
        CalcApp_enuCurrentState = CalcApp_enuDisplayOperationState;
    }
    
    /* OPERATOR IS ALREADY ASSIGNED */
    else if(CalcApp_strCurrInputData.CalcApp_strOperation == '-' || CalcApp_strCurrInputData.CalcApp_strOperation == '+' || CalcApp_strCurrInputData.CalcApp_strOperation == '*' ||CalcApp_strCurrInputData.CalcApp_strOperation == '/' ){
        
        /* CURRENT OPERATION IS A MINUS */ 
        if(Copy_u8KeyValue == '-'){
            CalcApp_u8Sign = 1;
        }
        
    }

    else{
        /* DO NOTHING */ 
    }
}




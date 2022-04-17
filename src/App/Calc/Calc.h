/*
 * Calc.h
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */

#ifndef APP_CALC_CALC_H_
#define APP_CALC_CALC_H_

/* ==================================================================== */
/* ========================== New Types =============================== */
/* ==================================================================== */
typedef struct{
    
    s64 CalcApp_strOperand1;
    s64 CalcApp_strOperand2;
    u8  CalcApp_strOperation;
    
}CalcApp_strInputData;

typedef struct{

    u8 CalcApp_strFloatNumFlag; /*flag set to 1 to detect if result is float*/
    s64 CalcApp_strResult;
    
}CalcApp_strOutputData;

typedef enum{
    
    CalcApp_enuStartState, 
    
    CalcApp_enuGetOperandOneState,
    CalcApp_enuDisplayOperandOneState,
    CalcApp_enuDisplayOperationState,
    CalcApp_enuGetOperandTwoState,
    CalcApp_enuDisplayOperandTwoState,
    CalcApp_enuCalculateResultState,
    
    CalcApp_enuDisplayResultState,
	CalcApp_enuDone
    
}CalcApp_enuOperandState;

/*
 *enumeration to define error status of each function
 */
typedef enum {
	CalcApp_OK, CalcApp_NOK, CalcApp_NOK_DivByZero, CalcApp_NOK_ResultOverflow
} CalcApp_enuErrorStatus;

/* ==================================================================== */
/* ===================== Function Prototypes ========================== */
/* ==================================================================== */
void CalcApp_vidTask();




#endif /* APP_CALC_CALC_H_ */

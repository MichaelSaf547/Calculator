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
    
}CalcApp_enuOperandState;


/* ==================================================================== */
/* ===================== Function Prototypes ========================== */
/* ==================================================================== */
void CalcApp_vidTask();




#endif /* APP_CALC_CALC_H_ */

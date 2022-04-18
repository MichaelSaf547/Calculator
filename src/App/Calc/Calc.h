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
    
    s32 CalcApp_strOperand1;
    s32 CalcApp_strOperand2;
    u8  CalcApp_strOperation;
    
}CalcApp_strInputData;

typedef struct{

    s32 CalcApp_strResult;
    
}CalcApp_strOutputData;

typedef enum{
    
    CalcApp_enuStartState, 
    
    CalcApp_enuGetOperandOneState,
    CalcApp_enuGetOperandTwoState,
    CalcApp_enuCalculateResultState,
	CalcApp_enuDisplayOperand1State,
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


extern void CalcApp_Init(void);

#endif /* APP_CALC_CALC_H_ */

/*
 * Module: Scheduler
 *
 * File Name: Sched.h
 *
 * Description:	header file for scheduler module for Arm Cortex M4 stm32f401cc
 *
 * Author: Sara Adel
 */

#ifndef SCHED_H_
#define SCHED_H_


/**************************************** Type Definitions **********************************/

/**
 * Type defined pointer to function with no arguments and no return
 **/
typedef  void (* SchedPointer_to_Function_t)(void);


/**
 * Type defined structure used to configure the PLL
 * with elements:  Callback - pointer to function (takes the address of the task function)
 * 				   CyclicTimeMs - function repeats each CyclicTime
 *
 * used in the function: Sched_RegisterRunnable
 *
 **/
typedef struct
{
	SchedPointer_to_Function_t Callback;
	u32 CyclicTimeMs;

}Runnable_t;

/**
 * Type defined enumeration used to indicate the error status of a function
 * with elements:  Sched_Ok - when function job done correctly
 *				   Sched_IncompatibleCyclicTime - the cyclic time is not a divisor of the Tick
 *	   			   Sched_RepeatedPriority - a runnable is already assigned to this priority
 *				   Sched_NullPtr- incorrect address is passed to callback in Runnable
 *
 * used in all peripheral  function
 *
 **/
typedef enum
{
	Sched_Ok,
	Sched_IncompatibleCyclicTime,
	Sched_RepeatedPriority,
	Sched_NullPtr

}Sched_ErrorStatus;

/******************************** Function Prototypes ***************************************/

/*
 * Name: Sched_vidInit
 *
 * InputArguments: None
 *
 * Return Value: None
 *
 * Description: initialize scheduler
 *
 */
 void Sched_vidInit(void);


 /*
  * Name: Sched_vidStart
  *
  * InputArguments: None
  *
  * Return Value: None
  *
  * Description: start scheduler
  *
  */
 void Sched_vidStart(void);


 /*
  * Name: Sched_RegisterRunnable
  *
  * InputArguments:  Runnable - pointer to structure Runnable_t
  * 				Priority - priority of task ( 0 highest, 255 lowest)
  *
  * Return Value: Sched_ErrorStatus - return enumeration of the error status :
  *									Sched_Ok - when function job done correctly
  *									Sched_IncompatibleCyclicTime - the cyclic time is not a divisor of the Tick
  *									Sched_RepeatedPriority - a runnable is already assigned to this priority
  *									Sched_NullPtr- incorrect address is passed to callback in Runnable
  *
  * Description: register a task to the scheduler
  *
  */
 Sched_ErrorStatus Sched_RegisterRunnable(Runnable_t * Runnable, u8 Priority);


#endif /* SCHED_H_ */

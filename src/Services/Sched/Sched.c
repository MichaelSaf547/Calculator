/*
 * Module: Scheduler
 *
 * File Name: Sched.c
 *
 * Description:	source file for scheduler module for Arm Cortex M4 stm32f401cc
 *
 * Author: Sara Adel
 */


/****************************************** Includes  *****************************************/
#include "../../Lib/Std_types.h"
#include "../../Lib/Bit_utils.h"
#include "../../Mcal/Systick/Systick.h"
#include "Sched.h"
#include "Sched_prv.h"
#include "Sched_cfg.h"


/***********************************Private section*******************************************/

/*
 * array of structure Runnable_t represents the
 */
Runnable_t SchedTasks[SCHED_MAX_TASKS_NUMBER_CFG];

static void Scheduler(void);

static void Scheduler(void)
{

	static u32 TimeMs = SCHED_TICK_MS;

	u32 Priority ;

	for(Priority = 0;Priority <  SCHED_MAX_TASKS_NUMBER_CFG; Priority++)
	{
		if (TimeMs  ==  SchedTasks[Priority].CyclicTimeMs){

			if(SchedTasks[Priority].Callback != NULL){
				SchedTasks[Priority].Callback();
			}

		}
	}

	if (TimeMs  == SCHED_MAX_CYCLIC_TIME_MS_CFG){
		TimeMs = SCHED_TICK_MS;
	}else{
		TimeMs += SCHED_TICK_MS;
	}

}

/***********************************Function Implementation*******************************************/

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
void Sched_vidInit(void)
{
	u8 i;

	Systick_Init();

	Systick_SetTickTime_ms(SCHED_TICK_MS);

	 Systick_Register_cbf(Scheduler);

	for(i=0;i<SCHED_MAX_TASKS_NUMBER_CFG;i++){

		SchedTasks[i].Callback = NULL;
		SchedTasks[i].CyclicTimeMs = 0;
	}
}

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
 void Sched_vidStart()
{

	 Systick_Start();

	while(1){
		 asm("NOP");
	}
}
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
 Sched_ErrorStatus Sched_RegisterRunnable(Runnable_t * Runnable, u8 Priority)
{
	 Sched_ErrorStatus ErrorStatus = Sched_Ok;
	 if(SchedTasks[Priority].Callback != NULL && SchedTasks[Priority].CyclicTimeMs !=0 ){
		 ErrorStatus = Sched_RepeatedPriority;
	 }else if (Runnable -> Callback == NULL){
		 ErrorStatus = Sched_NullPtr;
	 }else if((Runnable->CyclicTimeMs % SCHED_TICK_MS) != 0){
		 ErrorStatus = Sched_IncompatibleCyclicTime;
	 }else{
		 SchedTasks[Priority].Callback = Runnable -> Callback;
		 SchedTasks[Priority].CyclicTimeMs = Runnable -> CyclicTimeMs;
	 }

	 return ErrorStatus;

}


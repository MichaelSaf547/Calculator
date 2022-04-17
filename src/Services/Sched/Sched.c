/*
 * Sched.c
 *
 *  Created on: Apr 14, 2022
 *      Author: Michael_Safwat
 */


#include "../../Lib/Std_types.h"
#include "../../Lib/Bit_utils.h"
#include "../../Mcal/Systick/Systick.h"
#include "Sched.h"
#include "Sched_prv.h"
#include "Sched_cfg.h"


/*
 * Sched is used to loop over the tasks every time the system timer interrupt happens
 *
 * Inputs: It takes no input.
 *
 * Return: It returns nothing.
 */
static void Sched(void);

/*
 * RunnableList is an array to hold the tasks in it with the defined length in the Sched_cfg.h file
 */
Runnable_t RunnableList[RUNNABLELISTLENGTH];

extern void Sched_vidInit()
{
	/*Initialize the system timer*/
	Systick_Init();

	/*Set the time for the timer in ms*/
	Systick_SetTickTime_ms(TICK_MS);

	/*Set the Sched function as the call back function to be called every time the timer finish*/
	Systick_Register_cbf(Sched);
}


extern void Sched_vidStart()
{
	/*Start the System timer*/
	Systick_Start();

	/*While to hold the processor waiting for the next interrupt of system timer*/
	while(1);
}

extern void Sched_RegisterRunnable(Runnable_t * Runnable, u32 Priority)
{
	/*Check if the priority place in the array is empty or not*/
	if(RunnableList[Priority].cbfP == NULL)
	{
		/*Save the given time of the task into the array in the priority location*/
		RunnableList[Priority].CyclicTimems = Runnable->CyclicTimems;

		/*Save the given function pointer in the array in the priority location*/
		RunnableList[Priority].cbfP = Runnable->cbfP;
	}/*end of if(RunnableList[Priority].cbfP == NULL)*/

	else
	{
		/*Raise Error*/
	}/*end of else of if(RunnableList[Priority].cbfP == NULL)*/

}

static void Sched(void)
{
	/*Sched_Timems is used to save the value of the time*/
	static u32 Sched_Timems = 0;

	/*Loc_u32ForCounter is used to loop over the tasks to find which one should run now*/
	u32 Loc_u32ForCounter;

	/*Loop over the tasks array to run the one that its time is now*/
	for(Loc_u32ForCounter = 0; Loc_u32ForCounter < RUNNABLELISTLENGTH; Loc_u32ForCounter++)
	{
		/*Check first that the periodic time of the task is not zero*/
		if(RunnableList[Loc_u32ForCounter].CyclicTimems != 0)
		{
			/*Check if the task time is now*/
			if(Sched_Timems % (RunnableList[Loc_u32ForCounter].CyclicTimems) == ZERO)
			{
				/*Check if the task function pointer is not null*/
				if(RunnableList[Loc_u32ForCounter].cbfP != NULL)
				{
					/*Call the Task*/
					(RunnableList[Loc_u32ForCounter].cbfP)();
				}/*end of if(RunnableList[Loc_u32ForCounter].cbfP != NULL)*/
				else
				{
					/*Error Handling*/
				}/*end of else of if(RunnableList[Loc_u32ForCounter].cbfP != NULL)*/
			}
		}/*end of if(RunnableList[Loc_u32ForCounter].CyclicTimems != 0)*/
		else
		{
			/*Error Handling*/
		}/*end of else of if(RunnableList[Loc_u32ForCounter].CyclicTimems != 0)*/
	}/*end of for(Loc_u32ForCounter = 0; Loc_u32ForCounter < RUNNABLELISTLENGTH; Loc_u32ForCounter++)*/

	/*Add the timer value to the time variable*/
	Sched_Timems = Sched_Timems + TICK_MS;

	/*Check if the time of all the tasks is equal to the max make the time variable equals zero*/
	if(SCHEDLOWESTCOMMON == Sched_Timems)
	{
		/*Make the time equals zero*/
		Sched_Timems = ZERO;
	}/*end of if(Sched_u32TimerMAx == Sched_Timems)*/
	else
	{
		/*Do Nothing*/
	}/*end of else of if(Sched_u32TimerMAx == Sched_Timems)*/
}



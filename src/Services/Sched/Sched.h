/*
 * Sched.h
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */

#ifndef SERVICES_SCHED_SCHED_H_
#define SERVICES_SCHED_SCHED_H_



typedef struct
{
	void (* cbfP)(void);
	u32 CyclicTimems;

}Runnable_t;


/*
 * Sched_vidInit is used to initialize the scheduler
 *
 * Inputs: It takes no inputs.
 *
 * Return: It returns nothing.
 */
extern void Sched_vidInit();


/*
 * Sched_vidStart is used to start the scheduler
 *
 * Inputs: It takes no inputs.
 *
 * Return: It returns nothing.
 */
extern void Sched_vidStart();


/*
 * Sched_RegisterRunnable is used to set a runnable task into the scheduler
 *
 * Inputs: It takes two inputs,	1- Runnable is a structure that contains
 * 													1- The function to call
 * 													2- The periodic time for this function
 * 								2- Priority is the priority of this task and two tasks can not have the same number.
 *
 * Return: It returns nothing.
 */
extern void Sched_RegisterRunnable(Runnable_t * Runnable, u32 Priority);



#endif /* SERVICES_SCHED_SCHED_H_ */

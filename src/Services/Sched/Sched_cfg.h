/*
 * Sched_cfg.h
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */

#ifndef SERVICES_SCHED_SCHED_CFG_H_
#define SERVICES_SCHED_SCHED_CFG_H_


/*
 * The number of tasks in the scheduler
 */
#define RUNNABLELISTLENGTH			3

/*Adjust the tick time of the system timer it should be the greatest common divisor of all the tasks*/
#define TICK_MS				2

/*SCHEDLOWESTCOMMON is the smallest number which modulus equals zero on all the tasks*/
#define SCHEDLOWESTCOMMON 	100


#endif /* SERVICES_SCHED_SCHED_CFG_H_ */

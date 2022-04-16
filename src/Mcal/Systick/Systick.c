/*
 * Systick.c
 *
 *  Created on: Apr 14, 2022
 *      Author:	Fady Nashat
 */


#include "../../Lib/Std_types.h"
#include "../../Lib/Bit_utils.h"
#include "Systick.h"
#include "Systick_prv.h"
#include "Systick_cfg.h"


#define STK_CTRL	*((pu32) 0xE000E010)
#define STK_LOAD	*((pu32) 0xE000E014)
#define STK_VAL		*((pu32) 0xE000E018)


static Systick_cbf_t Appfunc;


/*
 * Purpose : Initialize the Systick timer , this function takes no arguments and return void
 *
 * Arguments : void
 *
 * Return : void
 */
void Systick_Init()
{
	STK_CTRL = Systick_TICKINT | Systick_CLKSOURCE ;
}


/*
 * Purpose : it set the time needed to be counted in ms , this fucntion takes u32 value and return error status
 *
 * Argument : u32 value range from 1ms --> 2ms
 *
 * Return : Error status
 */
Systick_t_ErrorStatus Systick_SetTickTime_ms(u32 time)
{
	Systick_t_ErrorStatus Error = systick_ok;
	u32 value ;
	switch (Systick_CLKSOURCE)
	{
	case Systick_CLKSOURCE_AHB:
		value = time*Systick_SystemClock/1000;
		break;

	case Systick_CLKSOURCE_AHB_8:
		value = time*(Systick_SystemClock/8)/1000;
		break;
	}

	if(value > 0x00FFFFFF)
	{
		Error = systick_nok;
	}
	else
	{
		STK_LOAD = value;
	}

	return Error;
}


/*
 * Purpose : it enables the timer to count down , this function takes no arguments and return no arguments
 *
 * Arguments : void
 *
 * Return : void
 */
void Systick_Start()
{
	STK_CTRL |= Systick_Enable;
}


/*
 * Purpose : set the call back function to call when interrupt happen
 *
 * Argument : pointer to function that will be called
 *
 * Return : void
 */
void Systick_Register_cbf(Systick_cbf_t cbf)
{
	Appfunc = cbf;
}

/*
 * Purpose : when interrupt happen call the function which given
 */
void SysTick_Handler (void)
{
	if(Appfunc)
	{
		Appfunc();
	}
}



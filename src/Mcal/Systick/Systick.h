/*
 * Systick.h
 *
 *  Created on: Apr 14, 2022
 *      Author: Fady Nashat
 */

#ifndef MCAL_SYSTICK_SYSTICK_H_
#define MCAL_SYSTICK_SYSTICK_H_


//********************New Types*******************//
typedef enum{
	systick_ok,
	systick_nok
}Systick_t_ErrorStatus;

typedef void (*Systick_cbf_t) (void);
//*********************Defines********************//
#define Systick_Enable			0x00000001
#define Systick_TICKINT_ON		0x00000002
#define Systick_CLKSOURCE_AHB_8	0x00000000
#define Systick_CLKSOURCE_AHB	0x00000004



//*****************Function Prototypes************//
/*
 * Purpose : Initialize the Systick timer , this function takes no arguments and return void
 *
 * Arguments : void
 *
 * Return : void
 */
void Systick_Init();


/*
 * Purpose : it set the time needed to be counted in ms , this fucntion takes u32 value and return error status
 *
 * Argument : u32 value range from 1ms --> 2ms
 *
 * Return : Error status
 */
Systick_t_ErrorStatus Systick_SetTickTime_ms(u32 time);


/*
 * Purpose : it enables the timer to count down , this function takes no arguments and return no arguments
 *
 * Arguments : void
 *
 * Return : void
 */
void Systick_Start();


/*
 * Purpose : set the call back function to call when interrupt happen
 *
 * Argument : pointer to function that will be called
 *
 * Return : void
 */
void Systick_Register_cbf(Systick_cbf_t cbf);




#endif /* MCAL_SYSTICK_SYSTICK_H_ */

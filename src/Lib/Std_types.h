/******************************************************************************
 * @file    Std_types.h
 * @author  Joseph Girgis
 * @version V1.3
 * @date    20-March-2022
 * @brief   Standard Types header file in Library Layer.
 ******************************************************************************/

#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_

typedef unsigned char       	u8;
typedef unsigned short int  	u16;
typedef unsigned int        	u32;
typedef unsigned long long int	u64;
typedef signed char         	s8;
typedef signed short int    	s16;
typedef signed int          	s32;
typedef signed long long int	s64;
typedef float               	f32;
typedef double              	f64;

/* Pointers */
typedef unsigned char*      	pu8;
typedef unsigned short int* 	pu16;
typedef unsigned int*       	pu32;
typedef signed char*        	ps8;
typedef signed short int*   	ps16;
typedef signed int*         	ps32;
typedef float*              	pf32;
typedef double*             	pf64;

/* NULL Pointer */
#define NULL					((void*)0x00)

#endif /* _STD_TYPES_H_ */

/*** end of file ***/

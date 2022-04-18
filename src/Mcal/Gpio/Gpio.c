/******************************************************************************
 * @file    gpio.c
 * @author  Joseph Girgis
 * @version V1.1
 * @date    17-April-2022
 * @brief   Source file of GPIO MCAL module.
 ******************************************************************************/
 
/* ============================================================ */
/* ======================== Inclusions ======================== */
/* ============================================================ */
 
#include "../../Lib/Std_types.h"
#include "../../Lib/Bit_utils.h"

#include "gpio.h"

/* ================================================================== */
/* ======================== Type Definitions ======================== */
/* ================================================================== */

/* GPIOx Struct */
typedef struct{
	u32 MODER;			// GPIO port mode register
	u32 OTYPER;			// GPIO port output type register
	u32 OSPEEDR;		// GPIO port output speed register
	u32 PUPDR;			// GPIO port pull-up/pull-down register
	u32 IDR;			// GPIO port input data register
	u32 ODR;			// GPIO port output data register
	u32 BSRR;			// GPIO port bit set/reset register
	u32 LCKR;			// GPIO port configuration lock register
	u32 AFRL;			// GPIO alternate function low register
	u32 AFRH;			// GPIO alternate function high register
}GPIO_t;

/* ============================================================= */
/* ======================== Definitions ======================== */
/* ============================================================= */

/* Configuration Masks */
#define MASK_MODER			((u8)0b11000)
#define MASK_OTYPER			((u8)0b00100)
#define MASK_PUPDR			((u8)0b00011)
#define MASK_OSPEEDR		((u8)0b00011)

/* ================================================================================== */
/* ======================== Public Functions Implementations ======================== */
/* ================================================================================== */

gpioErrorStatus_t gpio_initPin(gpioPinCfg_t *PinCfg)
{
	gpioErrorStatus_t errorStatus = gpio_enuOk;
	u32 tmp;
	u8 i;
	for (i = 0; i < 16; i++) {
		if (GET_BIT((PinCfg->pin),i)) {
			
			/* Configuring MODER */
			tmp = (((GPIO_t*)(PinCfg->port))->MODER);
			tmp &= ~((MASK_MODER >> 3) << (i * 2));
			tmp |= (((PinCfg->mode) & (MASK_MODER)) >> 3) << (i * 2);
			(((GPIO_t*)(PinCfg->port))->MODER) = tmp;
			
			/* Configuring OTYPER */
			tmp = (((GPIO_t*)(PinCfg->port))->OTYPER);
			tmp &= ~((MASK_OTYPER >> 2) << i);
			tmp |= (((PinCfg->mode) & (MASK_OTYPER)) >> 2) << i;
			(((GPIO_t*)(PinCfg->port))->OTYPER) = tmp;
			
			/* Configuring PUPDR */
			tmp = (((GPIO_t*)(PinCfg->port))->PUPDR);
			tmp &= ~((MASK_PUPDR) << (i * 2));
			tmp |= ((PinCfg->mode) & (MASK_PUPDR)) << (i * 2);
			(((GPIO_t*)(PinCfg->port))->PUPDR) = tmp;
			
			/* Configuring OSPEEDR */
			tmp = (((GPIO_t*)(PinCfg->port))->OSPEEDR);
			tmp &= ~((MASK_OSPEEDR) << (i * 2));
			tmp |= (PinCfg->speed) << (i * 2);
			(((GPIO_t*)(PinCfg->port))->OSPEEDR) = tmp;
		}
	}
	return errorStatus;
}

/******************************************************************************/

gpioErrorStatus_t gpio_setPinValue(void *Port, u16 Pin, u8 Value)
{
	gpioErrorStatus_t errorStatus = gpio_enuOk;
	/* Inputs Validation */
	if ((Port != GPIO_PORTA) && (Port != GPIO_PORTB) && (Port != GPIO_PORTC)) {
		errorStatus = gpio_enuInvalidPort;
	} else if ((Value != GPIO_LOW) && (Value != GPIO_HIGH)) {
		errorStatus = gpio_enuInvalidValue;
	} else {	/* Setting Pin Value */
		switch (Value) {
		case GPIO_LOW:
			(((GPIO_t*)Port)->BSRR) = Pin << 16;
			break;
		case GPIO_HIGH:
			(((GPIO_t*)Port)->BSRR) = Pin;
			break;
		}
	}
	return errorStatus;
}

/******************************************************************************/

gpioErrorStatus_t gpio_getPinValue(void *Port, u16 Pin, pu8 Value)
{
	gpioErrorStatus_t errorStatus = gpio_enuOk;
	/* Inputs Validation */
	if ((Port != GPIO_PORTA) && (Port != GPIO_PORTB) && (Port != GPIO_PORTC)) {
		errorStatus = gpio_enuInvalidPort;
	} else {	/* Getting Pin Value */
		if ((((GPIO_t*)Port)->IDR) & (Pin))
			*Value = GPIO_HIGH;
		else
			*Value = GPIO_LOW;
	}
	return errorStatus;
}

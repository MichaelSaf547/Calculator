/******************************************************************************
 * @file    gpio.h
 * @author  Joseph Girgis
 * @version V1.0
 * @date    14-April-2022
 * @brief   Header file of GPIO MCAL module.
 ******************************************************************************/
 
#ifndef _GPIO_H_
#define _GPIO_H_

/* ================================================================== */
/* ======================== Type Definitions ======================== */
/* ================================================================== */
 
/* Pin Configuration */
typedef struct{
    u8 mode;
    u8 speed;
    u16 pin;
    void* port;
}gpioPinCfg_t;

/* Error Status */
typedef enum{
    gpio_enuOk,
    gpio_enuInvalidOperation,
    gpio_enuInvalidPort,
    gpio_enuInvalidPin,
    gpio_enuInvalidValue,
    gpio_enuNumOfErrors,
}gpioErrorStatus_t;

/* ============================================================= */
/* ======================== Definitions ======================== */
/* ============================================================= */

/* Ports */
#define GPIO_PORTA              ((void*)0x40020000)
#define GPIO_PORTB              ((void*)0x40020400)
#define GPIO_PORTC              ((void*)0x40020800)
    
/* Pins */  
#define GPIO_PIN0               ((u16)0x0001)
#define GPIO_PIN1               ((u16)0x0002)
#define GPIO_PIN2               ((u16)0x0004)
#define GPIO_PIN3               ((u16)0x0008)
#define GPIO_PIN4               ((u16)0x0010)
#define GPIO_PIN5               ((u16)0x0020)
#define GPIO_PIN6               ((u16)0x0040)
#define GPIO_PIN7               ((u16)0x0080)
#define GPIO_PIN8               ((u16)0x0100)
#define GPIO_PIN9               ((u16)0x0200)
#define GPIO_PIN10              ((u16)0x0400)
#define GPIO_PIN11              ((u16)0x0800)
#define GPIO_PIN12              ((u16)0x1000)
#define GPIO_PIN13              ((u16)0x2000)
#define GPIO_PIN14              ((u16)0x4000)
#define GPIO_PIN15              ((u16)0x8000)
    
/* Values */    
#define GPIO_LOW                ((u8)0x00)
#define GPIO_HIGH               ((u8)0x01)
    
/* Pin Modes */ 
#define GPIO_PIN_IN             ((u8)0b00000)
#define GPIO_PIN_IN_PU          ((u8)0b00001)
#define GPIO_PIN_IN_PD          ((u8)0b00010)

#define GPIO_PIN_OUT_PP         ((u8)0b01000)
#define GPIO_PIN_OUT_PP_PU      ((u8)0b01001)
#define GPIO_PIN_OUT_PP_PD      ((u8)0b01010)
#define GPIO_PIN_OUT_OD         ((u8)0b01100)
#define GPIO_PIN_OUT_OD_PU      ((u8)0b01101)
#define GPIO_PIN_OUT_OD_PD      ((u8)0b01110)

#define GPIO_PIN_AF_PP          ((u8)0b10000)
#define GPIO_PIN_AF_PP_PU       ((u8)0b10001)
#define GPIO_PIN_AF_PP_PD       ((u8)0b10010)
#define GPIO_PIN_AF_OD          ((u8)0b10100)
#define GPIO_PIN_AF_OD_PU       ((u8)0b10101)
#define GPIO_PIN_AF_OD_PD       ((u8)0b10110)

#define GPIO_PIN_ANALOG         ((u8)0b11000)

/* Pin Speeds */
#define GPIO_PIN_SPEED0         ((u8)0b00)
#define GPIO_PIN_SPEED1         ((u8)0b01)
#define GPIO_PIN_SPEED2         ((u8)0b10)
#define GPIO_PIN_SPEED3         ((u8)0b11)

/* ====================================================================== */
/* ======================== Functions Prototypes ======================== */
/* ====================================================================== */

/******************************************************************************
 * @name gpio_initPin
 * @public function
 * @brief initializes configuration for a specific pin
 * @param PinCfg
 *        pointer to gpioPinCfg_t structure that contains
 *        the configuration information for the a specific pin
 * @retval None
 ******************************************************************************/
gpioErrorStatus_t gpio_initPin(gpioPinCfg_t *PinCfg);

/******************************************************************************
 * @name gpio_setPinValue
 * @public function
 * @brief sets value for a specific pin
 * @param Port
 *        pointer to void to specify the port of the pin
 *        @arg GPIO_PORTA
 *        @arg GPIO_PORTB
 *        @arg GPIO_PORTC
 * @param Pin
 *        u8 variable to specify the pin number in the port
 *        @arg GPIO_PINx ; x = [ 0 -> 15 ]
 * @param Value
 *        u16 variable to hold a value of specific pin
 *        @arg GPIO_LOW
 *        @arg GPIO_HIGH
 * @retval None
 ******************************************************************************/
gpioErrorStatus_t gpio_setPinValue(void *Port, u16 Pin, u8 Value);

/******************************************************************************
 * @name gpio_getPinValue
 * @public function
 * @brief gets value from a specific pin
 * @param Port
 *        pointer to void to specify the port of the pin
 *        @arg GPIO_PORTA
 *        @arg GPIO_PORTB
 *        @arg GPIO_PORTC
 * @param Pin
 *        u16 variable to specify the pin number in the port
 * @param Value
 *        pointer to u8 variable to hold a value of specific pin
 *        @arg GPIO_LOW
 *        @arg GPIO_HIGH
 * @retval None
 ******************************************************************************/
gpioErrorStatus_t gpio_getPinValue(void *Port, u16 Pin, pu8 Value);

#endif /* _GPIO_H_ */

/*** end of file ***/
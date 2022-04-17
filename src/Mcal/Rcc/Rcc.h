/*
 * Rcc.h
 *
 *  Created on: Apr 14, 2022
 *      Author: Romario
 */

#ifndef MCAL_RCC_RCC_H_
#define MCAL_RCC_RCC_H_


/* ==================================================================== */
/* ========================== New Types =============================== */
/* ==================================================================== */
typedef enum{
  HSI,
  HSE
}RCC_enuPLLSRC;

typedef struct{
  RCC_enuPLLSRC   RCC_enuPLLSCR;
  u32             RCC_enuPLL_M;
  u32             RCC_enuPLL_N;
  u32             RCC_enuPLL_P;
  u32             RCC_enuPLL_Q;
}RCC_structCLKPLL;

typedef enum {
  RCC_enuOK,
  RCC_enuNotOK
}RCC_enuErrorStatus;

typedef enum{
  OFF,
  ON
}RCC_enuClkStatus;

typedef enum{
  Not_Ready,
  Ready
}RCC_enuReadyStatus;

typedef enum {
  RCC_ClockStatus_enuHSI,
  RCC_ClockStatus_enuHSE,
  RCC_ClockStatus_enuPLL,
  RCC_ClockStatus_enuNotApplicable
}RCC_enuClockStatus;


/* ==================================================================== */
/* ========================== Clock Types ============================= */
/* ==================================================================== */
#define RCC_HSI                 0
#define RCC_HSE                 1
#define RCC_PLL                 2



/* ========================== AHB Prescalers ========================== */
#define AHB_NO_CLOCK_FACTOR                           0b0000
#define AHB_CLOCK_DIVIDED_BY_2                        0b1000
#define AHB_CLOCK_DIVIDED_BY_4                        0b1001
#define AHB_CLOCK_DIVIDED_BY_8                        0b1010
#define AHB_CLOCK_DIVIDED_BY_16                       0b1011
#define AHB_CLOCK_DIVIDED_BY_64                       0b1100
#define AHB_CLOCK_DIVIDED_BY_128                      0b1101
#define AHB_CLOCK_DIVIDED_BY_256                      0b1110
#define AHB_CLOCK_DIVIDED_BY_512                      0b1111

/* ========================== AHB Prescalers ========================== */
#define APB1_NO_CLOCK_FACTOR                           0b000
#define APB1_CLOCK_DIVIDED_BY_2                        0b100
#define APB1_CLOCK_DIVIDED_BY_4                        0b101
#define APB1_CLOCK_DIVIDED_BY_8                        0b110
#define APB1_CLOCK_DIVIDED_BY_16                       0b111

/* ========================== APB2 Prescaler ========================== */
#define APB2_NO_CLOCK_FACTOR                           0b000
#define APB2_CLOCK_DIVIDED_BY_2                        0b100
#define APB2_CLOCK_DIVIDED_BY_4                        0b101
#define APB2_CLOCK_DIVIDED_BY_8                        0b110
#define APB2_CLOCK_DIVIDED_BY_16                       0b111



/* ==================================================================== */
/* =========================== Peripherals ============================ */
/* ==================================================================== */

/* ================================ AHB1 ============================== */
#define GPIOA_RCC                       0
#define GPIOB_RCC                       1
#define GPIOC_RCC                       2
#define GPIOD_RCC                       3
#define GPIOE_RCC                       4
#define GPIOH_RCC                       7
#define CRC_RCC                         12
#define DMA1_RCC                        21
#define DMA2_RCC                        22

/* ================================ AHB2 ============================== */
#define OTGFS_RCC 7

/* ================================ APB1 ============================== */
#define TIM2_RCC                        0
#define TIM3_RCC                        1
#define TIM4_RCC                        2
#define TIM5_RCC                        3
#define WWDG_RCC                        11
#define SPI2_RCC                        14
#define SPI3_RCC                        15
#define USART2_RCC                      17
#define I2C1_RCC                        21
#define I2C2_RCC                        22
#define I2C3_RCC                        23
#define PWR_RCC                         28

/* ================================ APB2 ============================== */
#define TIM1_RCC                        0
#define USART1_RCC                      4
#define USART6_RCC                      5
#define ADC1_RCC                        8
#define SDIO_RCC                        11
#define SPI1_RCC                        12
#define SPI4_RCC                        13
#define SYSCFG_RCC                      14
#define TIM9_RCC                        16
#define TIM10_RCC                       17
#define TIM11_RCC                       18


#define AHB1_BUS                        0
#define AHB2_BUS                        1
#define APB1_BUS                        2
#define APB2_BUS                        3

/* ==================================================================== */
/* ===================== Function Prototypes ========================== */
/* ==================================================================== */
RCC_enuErrorStatus  RCC_enuSelectClk             (u8 Copy_u8ClockSystem);
RCC_enuErrorStatus  RCC_enuControlClk            (u8 Copy_u8ClockSystem,RCC_enuClkStatus Copy_enuClkStatus);
RCC_enuErrorStatus  RCC_enuCheckReady            (u8 Copy_u8ClockSystem, u8* Add_pu8ReadyStatus);
RCC_enuErrorStatus  RCC_enuConfigPLL             (RCC_structCLKPLL Copy_structPLLCongif);
RCC_enuErrorStatus  RCC_enuControlPrescalerAHB   (u32 Copy_u32AHBPrescaler);
RCC_enuErrorStatus  RCC_enuControlPrescalerAPB1  (u32 Copy_u32APB1Prescaler);
RCC_enuErrorStatus  RCC_enuControlPrescalerAPB2  (u32 Copy_u32APB2Prescaler);
RCC_enuErrorStatus  RCC_enuEnablePreipheral      (u32 Copy_u32PeripheralBus, u32 Copy_u32Peripheral);
RCC_enuErrorStatus  RCC_enuDisablePreipheral     (u32 Copy_u32PeripheralBus, u32 Copy_u32Peripheral);



#endif /* MCAL_RCC_RCC_H_ */

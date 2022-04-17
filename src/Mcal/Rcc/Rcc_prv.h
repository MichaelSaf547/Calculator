/*
 * Rcc_prv.h
 *
 *  Created on: Apr 14, 2022
 *      Author: Romario
 */

#ifndef MCAL_RCC_RCC_PRV_H_
#define MCAL_RCC_RCC_PRV_H_


/* ==================================================================== */
/* ======================== Registers Defination ====================== */
/* ==================================================================== */
#define RCC_CR                  *((volatile u32*)0x40023800)
#define RCC_PLLCFGR             *((volatile u32*)0x40023804)
#define RCC_CFGR                *((volatile u32*)0x40023808)
#define RCC_CIR                 *((volatile u32*)0x4002380C)
#define RCC_AHB1RSTR            *((volatile u32*)0x40023810)
#define RCC_AHB2RSTR            *((volatile u32*)0x40023814)
#define RCC_APB1RSTR            *((volatile u32*)0x40023820)
#define RCC_APB2RSTR            *((volatile u32*)0x40023824)
#define RCC_AHB1ENR             *((volatile u32*)0x40023830)
#define RCC_AHB2ENR             *((volatile u32*)0x40023834)
#define RCC_APB1ENR             *((volatile u32*)0x40023840)
#define RCC_APB2ENR             *((volatile u32*)0x40023844)
#define RCC_AHB1LPENR           *((volatile u32*)0x40023850)
#define RCC_AHB2LPENR           *((volatile u32*)0x40023854)
#define RCC_APB1LPENR           *((volatile u32*)0x40023860)
#define RCC_APB2LPENR           *((volatile u32*)0x40023864)
#define RCC_BDCR                *((volatile u32*)0x40023870)
#define RCC_CSR                 *((volatile u32*)0x40023874)
#define RCC_SSCGR               *((volatile u32*)0x40023880)
#define RCC_PLLI2SCFGR          *((volatile u32*)0x40023884)
#define RCC_DCKCFGR             *((volatile u32*)0x4002388C)

/* ==================================================================== */
/* ============================= Masks ================================ */
/* ==================================================================== */
#define RCC_SET_CFGR_HSI         0x00000003
#define RCC_SET_CR_HSI           0x00000001

#define RCC_GET_HSI_RDY          0x00000002
#define RCC_GET_HSE_RDY          0x00020000
#define RCC_GET_PLL_RDY          0x02000000

#define MASK_GET_SYSCLK          0x0000000C
#define MASK_SET_HSI             0x00000001
#define MASK_SET_HSE             0x00010000
#define MASK_SET_PLL             0x01000000

#define MASK_GET_RDY_HSI         0x00000002
#define MASK_GET_RDY_HSE         0x00020000
#define MASK_GET_RDY_PLL         0x02000000
#define MASK_GET_ON_PLL          0x01000000

#define MASK_SET_PPL_M           0x0000003F
#define MASK_SET_PPL_N           0x00007FC0
#define MASK_SET_PPL_P           0x00030000
#define MASK_SET_PPL_Q           0x0F000000
#define MASK_SET_PPL_SRC         0x00400000

#define SHIFT_PLL_M              0
#define SHIFT_PLL_N              6
#define SHIFT_PLL_P              16
#define SHIFT_PLL_Q              24
#define SHIFT_PLL_SRC            22

#define SHIFT_AHB                4
#define SHIFT_APB1               10
#define SHIFT_APB2               13

#define MASK_SET_PRE_AHB         0x000000F0
#define MASK_SET_PRE_APB1        0x00001C00
#define MASK_SET_PRE_APB2        0x0000E000

#define  MASK_HSI_ON             0x00000001
#define  MASK_HSI_RDY            0x00000002
#define  MASK_HSE_ON             0x00010000
#define  MASK_HSE_RDY            0x00020000
#define  MASK_PLL_ON             0x01000000
#define  MASK_PLL_RDY            0x02000000

#define  MASK_SW                 0x00000003
#define  MASK_GET_SWS            0x0000000C

#define MASK_SWS_HSI 			 0x00000000
#define MASK_SWS_HSE 			 0x00000004
#define MASK_SWS_PLL 			 0x00000008

#define RCC_NOT_RDY_STATUS 		 0
#define RCC_RDY_STATUS 			 1
/* ============================ Pins Masks ============================= */
#define MASK_PIN_0                      0x00000001
#define MASK_PIN_1                      0x00000002
#define MASK_PIN_2                      0x00000004
#define MASK_PIN_3                      0x00000008

#define MASK_PIN_4                      0x00000010
#define MASK_PIN_5                      0x00000020
#define MASK_PIN_6                      0x00000040
#define MASK_PIN_7                      0x00000080

#define MASK_PIN_8                      0x00000100
#define MASK_PIN_9                      0x00000200
#define MASK_PIN_10                     0x00000400
#define MASK_PIN_11                     0x00000800

#define MASK_PIN_12                     0x00001000
#define MASK_PIN_13                     0x00002000
#define MASK_PIN_14                     0x00004000
#define MASK_PIN_15                     0x00008000

#define MASK_PIN_16                     0x00010000
#define MASK_PIN_17                     0x00020000
#define MASK_PIN_18                     0x00040000
#define MASK_PIN_19                     0x00080000

#define MASK_PIN_20                     0x00100000
#define MASK_PIN_21                     0x00200000
#define MASK_PIN_22                     0x00400000
#define MASK_PIN_23                     0x00800000

#define MASK_PIN_24                     0x01000000
#define MASK_PIN_25                     0x02000000
#define MASK_PIN_26                     0x04000000
#define MASK_PIN_27                     0x08000000

#define MASK_PIN_28                     0x10000000
#define MASK_PIN_29                     0x20000000
#define MASK_PIN_30                     0x40000000
#define MASK_PIN_31                     0x80000000

#endif /* MCAL_RCC_RCC_PRV_H_ */

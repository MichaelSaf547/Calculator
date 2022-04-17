/******************************************************************************
 * @file    Bit_utils.h
 * @author  Joseph Girgis
 * @version V1.0
 * @date    14-April-2022
 * @brief   Bit Utilities header file in Library Layer.
 ******************************************************************************/

#ifndef _BIT_UTILS_H_
#define _BIT_UTILS_H_

/* **********************************
 * MAGIC NUMBERS
 * **********************************/
#define ZERO            0
#define ONE             1
#define TWO             2
#define THREE			3
#define FOUR			4
#define FIVE			5
#define SIX				6
#define SEVEN			7
#define EIGHT			8
#define NINE			9
#define TEN 			10
#define HIGH_8BIT       0xFF
#define HIGH_16BIT      0xFFFF
#define HIGH_32BIT      0xFFFFFFFF
#define HALF_REG_SIZE   REG_SIZE/TWO

/* **********************************
 * REGISTER SIZE
 * options:
 * _8BIT_
 * _16BIT_
 * _32BIT_
 * **********************************/
#define _8BIT_      8
#define _16BIT_     16
#define _32BIT_     32
#define REG_SIZE    _32BIT_

/* **********************************
 * BIT UTILITIES FOR 8-bit Registes
 * **********************************/
#if REG_SIZE == _8BIT_

/* Bit Manipulation */
#define SET_BIT(Reg,Bit)        (Reg|=(ONE<<Bit))                                           // Tested
#define CLR_BIT(Reg,Bit)        (Reg&=(~(ONE<<Bit)))                                        // Tested
#define TGL_BIT(Reg,Bit)        (Reg^=(ONE<<Bit))                                           // Tested
#define GET_BIT(Reg,Bit)        ((Reg>>Bit)&ONE)                                            // Tested

/* Nibble Manipulation */
#define SET_H_NIB(Reg)          (Reg|=(HIGH_8BIT<<HALF_REG_SIZE))                           // Tested
#define SET_L_NIB(Reg)          (Reg|=(HIGH_8BIT>>HALF_REG_SIZE))                           // Tested
#define CLR_H_NIB(Reg)          (Reg&=(HIGH_8BIT>>HALF_REG_SIZE))                           // Tested
#define CLR_L_NIB(Reg)          (Reg&=(HIGH_8BIT<<HALF_REG_SIZE))                           // Tested
#define TGL_H_NIB(Reg)          (Reg^=(HIGH_8BIT<<HALF_REG_SIZE))                           // Tested
#define TGL_L_NIB(Reg)          (Reg^=(HIGH_8BIT>>HALF_REG_SIZE))                           // Tested
#define GET_H_NIB(Reg)          (Reg>>HALF_REG_SIZE)                                        // Tested
#define GET_L_NIB(Reg)          (Reg&(HIGH_8BIT>>HALF_REG_SIZE))                            // Tested
#define ASSIGN_H_NIB(Reg,Value) (Reg=(Reg&(HIGH_8BIT>>HALF_REG_SIZE))|(Value<<HALF_REG_SIZE)) // Tested
#define ASSIGN_L_NIB(Reg,Value) (Reg=(Reg&(HIGH_8BIT<<HALF_REG_SIZE))|Value)                  // Tested
#define SWAP_NIB(Reg)           (Reg=(Reg>>HALF_REG_SIZE)|(Reg<<(REG_SIZE-HALF_REG_SIZE)))  // Tested

/* Register Manipuation */
#define SET_REG(Reg)            (Reg|=HIGH_8BIT)                                            // Tested
#define CLR_REG(Reg)            (Reg&=ZERO)                                                 // Tested
#define TGL_REG(Reg)            (Reg^=~(ZERO<<REG_SIZE))                                    // Tested
#define GET_REG(Reg)            (Reg)                                                       // Tested
#define ASSIGN_REG(Reg,Value)   (Reg=Value)                                                 // Tested
#define RSHFT_REG(Reg,No)       (Reg>>=No)                                                  // Tested
#define LSHFT_REG(Reg,No)       (Reg<<=No)                                                  // Tested
#define CRSHFT_REG(Reg,No)      (Reg=(Reg>>No)|(Reg<<(REG_SIZE-No)))                        // Tested
#define CLSHFT_REG(Reg,No)      (Reg=(Reg<<No)|(Reg>>(REG_SIZE-No)))                        // Tested

#endif /* REG_SIZE == _8BIT_ */

/* **********************************
 * BIT UTILITIES FOR 16-bit Registes
 * **********************************/
#if REG_SIZE == _16BIT_

/* Bit Manipulation */
#define SET_BIT(Reg,Bit)        (Reg|=(ONE<<Bit))                                           // Tested
#define CLR_BIT(Reg,Bit)        (Reg&=(~(ONE<<Bit)))                                        // Tested
#define TGL_BIT(Reg,Bit)        (Reg^=(ONE<<Bit))                                           // Tested
#define GET_BIT(Reg,Bit)        ((Reg>>Bit)&ONE)                                            // Tested

/* Nibble Manipulation */
#define SET_H_NIB(Reg)          (Reg|=(HIGH_16BIT<<HALF_REG_SIZE))                          // Tested
#define SET_L_NIB(Reg)          (Reg|=(HIGH_16BIT>>HALF_REG_SIZE))                          // Tested
#define CLR_H_NIB(Reg)          (Reg&=(HIGH_16BIT>>HALF_REG_SIZE))                          // Tested
#define CLR_L_NIB(Reg)          (Reg&=(HIGH_16BIT<<HALF_REG_SIZE))                          // Tested
#define TGL_H_NIB(Reg)          (Reg^=(HIGH_16BIT<<HALF_REG_SIZE))                          // Tested
#define TGL_L_NIB(Reg)          (Reg^=(HIGH_16BIT>>HALF_REG_SIZE))                          // Tested
#define GET_H_NIB(Reg)          (Reg>>HALF_REG_SIZE)                                        // Tested
#define GET_L_NIB(Reg)          (Reg&(HIGH_16BIT>>HALF_REG_SIZE))                           // Tested
#define ASSIGN_H_NIB(Reg,Value) (Reg=Reg&(HIGH_16BIT>>HALF_REG_SIZE)|(Value<<HALF_REG_SIZE))// Tested
#define ASSIGN_L_NIB(Reg,Value) (Reg=Reg&(HIGH_16BIT<<HALF_REG_SIZE)|Value)                 // Tested
#define SWAP_NIB(Reg)           (Reg=(Reg>>HALF_REG_SIZE)|(Reg<<(REG_SIZE-HALF_REG_SIZE)))  // Tested

/* Register Manipuation */
#define SET_REG(Reg)            (Reg|=HIGH_16BIT)                                           // Tested
#define CLR_REG(Reg)            (Reg&=ZERO)                                                 // Tested
#define TGL_REG(Reg)            (Reg^=~(ZERO<<REG_SIZE))                                    // Tested
#define GET_REG(Reg)            (Reg)                                                       // Tested
#define ASSIGN_REG(Reg,Value)   (Reg=Value)                                                 // Tested
#define RSHFT_REG(Reg,No)       (Reg>>=No)                                                  // Tested
#define LSHFT_REG(Reg,No)       (Reg<<=No)                                                  // Tested
#define CRSHFT_REG(Reg,No)      (Reg=(Reg>>No)|(Reg<<(REG_SIZE-No)))                        // Tested
#define CLSHFT_REG(Reg,No)      (Reg=(Reg<<No)|(Reg>>(REG_SIZE-No)))                        // Tested

#endif /* REG_SIZE == _16BIT_ */

/* **********************************
 * BIT UTILITIES FOR 32-bit Registes
 * **********************************/
#if REG_SIZE == _32BIT_

/* Bit Manipulation */
#define SET_BIT(Reg,Bit)        (Reg|=(ONE<<Bit))                                           // Tested
#define CLR_BIT(Reg,Bit)        (Reg&=(~(ONE<<Bit)))                                        // Tested
#define TGL_BIT(Reg,Bit)        (Reg^=(ONE<<Bit))                                           // Tested
#define GET_BIT(Reg,Bit)        ((Reg>>Bit)&ONE)                                            // Tested

/* Nibble Manipulation */
#define SET_H_NIB(Reg)          (Reg|=(HIGH_32BIT<<HALF_REG_SIZE))                          // Tested
#define SET_L_NIB(Reg)          (Reg|=(HIGH_32BIT>>HALF_REG_SIZE))                          // Tested
#define CLR_H_NIB(Reg)          (Reg&=(HIGH_32BIT>>HALF_REG_SIZE))                          // Tested
#define CLR_L_NIB(Reg)          (Reg&=(HIGH_32BIT<<HALF_REG_SIZE))                          // Tested
#define TGL_H_NIB(Reg)          (Reg^=(HIGH_32BIT<<HALF_REG_SIZE))                          // Tested
#define TGL_L_NIB(Reg)          (Reg^=(HIGH_32BIT>>HALF_REG_SIZE))                          // Tested
#define GET_H_NIB(Reg)          (Reg>>HALF_REG_SIZE)                                        // Tested
#define GET_L_NIB(Reg)          (Reg&(HIGH_32BIT>>HALF_REG_SIZE))                           // Tested
#define ASSIGN_H_NIB(Reg,Value) (Reg=Reg&(HIGH_32BIT>>HALF_REG_SIZE)|(Value<<HALF_REG_SIZE))// Tested
#define ASSIGN_L_NIB(Reg,Value) (Reg=Reg&(HIGH_32BIT<<HALF_REG_SIZE)|Value)                 // Tested
#define SWAP_NIB(Reg)           (Reg=(Reg>>HALF_REG_SIZE)|(Reg<<(REG_SIZE-HALF_REG_SIZE)))  // Tested

/* Register Manipulation */
#define SET_REG(Reg)            (Reg|=HIGH_32BIT)                                           // Tested
#define CLR_REG(Reg)            (Reg&=ZERO)                                                 // Tested
#define TGL_REG(Reg)            (Reg^=~(ZERO<<REG_SIZE))                                    // Tested
#define GET_REG(Reg)            (Reg)                                                       // Tested
#define ASSIGN_REG(Reg,Value)   (Reg=Value)                                                 // Tested
#define RSHFT_REG(Reg,No)       (Reg>>=No)                                                  // Tested
#define LSHFT_REG(Reg,No)       (Reg<<=No)                                                  // Tested
#define CRSHFT_REG(Reg,No)      (Reg=(Reg>>No)|(Reg<<(REG_SIZE-No)))                        // Tested
#define CLSHFT_REG(Reg,No)      (Reg=(Reg<<No)|(Reg>>(REG_SIZE-No)))                        // Tested

#endif /* REG_SIZE == _32BIT_ */

#endif /* _BIT_UTILS_H_ */

/*** end of file ***/

/*
 * Rcc.c
 *
 *  Created on: Apr 14, 2022
 *      Author:
 */


#include "../../Lib/Std_types.h"
#include "../../Lib/Bit_utils.h"
#include "Rcc.h"
#include "Rcc_prv.h"


/****************************** Select CLOCK *******************************************
* @description  :  select the clock of the system -------------------------------------*
* @param       :        Copy_enuClk (RCC_CLK)             : RCC_HSI, RCC_HSE, RCC_PLL--*
* @return      :  error, choosing a wrong clock, or the clock isn't ready or on ,    --*
*                 or failed to set the clock as a system clock ------------------------*                                                                             *
****************************************************************************************/
RCC_enuErrorStatus  RCC_enuSelectClk(u8 Copy_u8ClockSystem){
  RCC_enuErrorStatus Ret_enuSystemCLOCK = RCC_enuOK;
  u32 Loc_u32CFGR_Register;
  u32 Loc_u32TimeOut = 0;

  /* CHECKING ERROR : RIGHT CLOCK */
  if(Copy_u8ClockSystem <= RCC_PLL){

    switch(Copy_u8ClockSystem){

       case RCC_HSI:{

    	   /* CHECK ON */
           if( ((RCC_CR & MASK_HSI_ON) == MASK_HSI_ON) ){

        	    /* CHECK READY FLAG */
                if((RCC_CR & MASK_HSI_RDY) == MASK_HSI_RDY){

					/* SET SW */
					Loc_u32CFGR_Register  = RCC_CFGR;
					Loc_u32CFGR_Register &= ~MASK_SW;
					Loc_u32CFGR_Register |= (RCC_HSI<<0);
					RCC_CFGR = Loc_u32CFGR_Register;

					while( ((RCC_CFGR & MASK_GET_SWS) != MASK_SWS_HSI) && (Loc_u32TimeOut<100000)){
						Loc_u32TimeOut++;
					}
					if( Loc_u32TimeOut >= 100000 ){
						/* TIMEOUT */
						Ret_enuSystemCLOCK = RCC_enuNotOK;
					}
                }

                /* ERROR : NOT READY */
                else{
                	Ret_enuSystemCLOCK = RCC_enuNotOK;
                }
            }
            /* CLOCK OFF */
            else{
                Ret_enuSystemCLOCK = RCC_enuNotOK;
            }

            break;
        }
        case RCC_HSE:
        /* CHECK ON */
        if((RCC_CR & MASK_HSE_ON) == MASK_HSE_ON){

          /* CHECK READY FLAG */
          if((RCC_CR & MASK_HSE_RDY) == MASK_HSE_RDY){

            /* SET SW */
            Loc_u32CFGR_Register  = RCC_CFGR;
            Loc_u32CFGR_Register &= ~MASK_SW;
            Loc_u32CFGR_Register |= (RCC_HSE<<0);
            RCC_CFGR = Loc_u32CFGR_Register;

            while(( (RCC_CFGR&MASK_SWS_HSE)!=MASK_SWS_HSE) && (Loc_u32TimeOut<100000)){
              Loc_u32TimeOut++;
            }
            if( Loc_u32TimeOut >= 100000 ){

              /* TIMEOUT */
              Ret_enuSystemCLOCK = RCC_enuNotOK;
            }
          }
          /* ERROR : NOT READY */
          else{
            Ret_enuSystemCLOCK = RCC_enuNotOK;
          }
        }
        /* CLOCK OFF */
        else{
          Ret_enuSystemCLOCK = RCC_enuNotOK;
        }

      break;

      case RCC_PLL:
        /* CHECK ON */
        if((RCC_CR & MASK_PLL_ON) == MASK_PLL_ON){

          /* CHECK READY FLAG */
          if((RCC_CR & MASK_PLL_RDY) == MASK_PLL_RDY){

            /* SET SW */
            Loc_u32CFGR_Register  = RCC_CFGR;
            Loc_u32CFGR_Register &= ~MASK_SW;
            Loc_u32CFGR_Register |= (RCC_PLL<<0);
            RCC_CFGR = Loc_u32CFGR_Register;

            while(( (RCC_CFGR&MASK_GET_SWS) !=MASK_SWS_PLL) && (Loc_u32TimeOut<100000)){
              Loc_u32TimeOut++;
            }
            if( Loc_u32TimeOut >= 100000 ){

              /* TIMEOUT */
              Ret_enuSystemCLOCK = RCC_enuNotOK;
            }
            else{
              /* DO NOTHING */
            }
          }
          /* ERROR : NOT READY */
          else{
            Ret_enuSystemCLOCK = RCC_enuNotOK;
          }
        }
        /* CLOCK OFF */
        else{
          Ret_enuSystemCLOCK = RCC_enuNotOK;
        }

      break;
    };

  }

  /* ERROR : NOT VALID CLOCK */
  else{
    Ret_enuSystemCLOCK = RCC_enuNotOK;
  }

  return Ret_enuSystemCLOCK;
}

/******************************* Control CLOCK  ****************************************
* @description :  control the clock on/off  -------------------------------------------*
* @param :        Copy_enuClk (RCC_CLK)                : RCC_HSI, RCC_HSE, RCC_PLL-----*
*                 Copy_enuClkStatus (RCC_enuClkStatus) : ON, OFF  ---------------------*
* @return         error, if changing the system clock, or choosing a wrong clock  -----*                                                                             *
****************************************************************************************/
RCC_enuErrorStatus  RCC_enuControlClk(u8 Copy_u8ClockSystem,RCC_enuClkStatus Copy_enuClkStatus){
  u32 Loc_u32SystemCLOCK = ((RCC_CFGR & MASK_GET_SYSCLK)>>2);
  RCC_enuErrorStatus Ret_enuSystemCLOCK = RCC_enuOK;
  u32 Loc_u32TimeOut = 0;

  /* ERROR : CHANGING ON THE SYSTEM CLOCK */
  if(Loc_u32SystemCLOCK == Copy_u8ClockSystem){
    Ret_enuSystemCLOCK = RCC_enuNotOK;
  }

  /* ERROR : CHOOSING WRONG CLOCK */
  else if((Copy_u8ClockSystem > RCC_PLL)){
    Ret_enuSystemCLOCK = RCC_enuNotOK;
  }

  else{
      switch(Copy_u8ClockSystem){

        /* CONTROL HSI CLK ON/OFF */
        case RCC_HSI:

          /* HSI : OFF */
          if(Copy_enuClkStatus == OFF){
            RCC_CR &= (~MASK_SET_HSI);
          }

          /* HSI : ON */
          else{
				RCC_CR |= (MASK_SET_HSI);

				/* WAIT TILL READY */
				while( ((RCC_CR & MASK_HSI_RDY) == 0) && (Loc_u32TimeOut<100000)){
					Loc_u32TimeOut++;
				}
				if( Loc_u32TimeOut >= 100000 ){

					/* TIMEOUT */
					Ret_enuSystemCLOCK = RCC_enuNotOK;
				}
          }
        break;

        /* CONTROL HSE CLK ON/OFF */
        case RCC_HSE:

          /* HSE : OFF */
          if(Copy_enuClkStatus == OFF){
            RCC_CR &= (~MASK_SET_HSE);
          }

          /* HSE : ON */
          else{
            RCC_CR |= (MASK_SET_HSE);

			   /* WAIT TILL READY */
			   while( ((RCC_CR & MASK_HSE_RDY) == 0) && (Loc_u32TimeOut<100000)){
				   Loc_u32TimeOut++;
			   }
			   if( Loc_u32TimeOut >= 100000 ){

				   /* TIMEOUT */
				   Ret_enuSystemCLOCK = RCC_enuNotOK;
			   }
		 }
        break;

        /* CONTROL PLL CLK ON/OFF */
        case RCC_PLL:

          /* PLL : OFF */
          if(Copy_enuClkStatus == OFF){
            RCC_CR &= (~MASK_SET_PLL);
          }

          /* PLL : ON */
          else{

        	/* SET PLL ON */
            RCC_CR |= (MASK_SET_PLL);

            /* WAIT TILL READY */
            while( ((RCC_CR & MASK_PLL_RDY) == 0) && (Loc_u32TimeOut<100000)){
				Loc_u32TimeOut++;
			}
			if( Loc_u32TimeOut >= 100000 ){

				/* TIMEOUT */
				Ret_enuSystemCLOCK = RCC_enuNotOK;
			}
          }
        break;
      };
  }

  return Ret_enuSystemCLOCK;
}


/****************************** Check Ready ********************************************
* @description :  check the clock ready/not ready -------------------------------------*
* @param  :       Copy_enuClk (RCC_CLK)                 RCC_HSI, RCC_HSE, RCC_PLL------*
*                 Add_enuReadyStatus (RCC_enuReadyStatus) : Address to get the status -*
* @return :       error, if changing the system clock, or choosing a wrong clock  -----*                                                                             *
****************************************************************************************/
RCC_enuErrorStatus  RCC_enuCheckReady(u8 Copy_u8ClockSystem, u8* Add_pu8ReadyStatus){
  RCC_enuErrorStatus Ret_enuSystemCLOCK = RCC_enuOK;


  if((Copy_u8ClockSystem <= RCC_PLL)){

      switch(Copy_u8ClockSystem){

        /* GET HSI CLK READY/NOT_READY */
        case RCC_HSI:
          if((RCC_CR&MASK_GET_RDY_HSI) == MASK_GET_RDY_HSI){
            *Add_pu8ReadyStatus = RCC_RDY_STATUS;
          }
          else{
            *Add_pu8ReadyStatus = RCC_NOT_RDY_STATUS;
          }
        break;

        /* GET HSE CLK READY/NOT_READY */
        case RCC_HSE:
          if((RCC_CR&MASK_GET_RDY_HSE) ==MASK_GET_RDY_HSE){
            *Add_pu8ReadyStatus = RCC_RDY_STATUS;
          }
          else{
            *Add_pu8ReadyStatus = RCC_NOT_RDY_STATUS;
          }
        break;

        /* GET PLL CLK READY/NOT_READY */
        case RCC_PLL:
          if((RCC_CR&MASK_GET_RDY_PLL) == MASK_GET_RDY_PLL){
            *Add_pu8ReadyStatus = RCC_RDY_STATUS;
          }
          else{
            *Add_pu8ReadyStatus = RCC_NOT_RDY_STATUS;
          }

        break;
      };
  }

  /* ERROR : CLOCK IS NOT TRUE */
  else{
      Ret_enuSystemCLOCK = RCC_enuNotOK;
  }

  return Ret_enuSystemCLOCK;
}

/****************************** Configure PLL ******************************************
* @description :  Configure the PLL configuration parameters --------------------------*
* @param :        Copy_structPLLCongif (RCC_structCLKPLL): struct, --------------------*
*                           RCC_enuPLLSCR : HSI, HSE.
*                           RCC_enuPLL_M : 2 : 63
*                           RCC_enuPLL_N : 2 : 511
*                           RCC_enuPLL_P : 2,4,6,8
*                           RCC_enuPLL_Q : 2 : 15
*
* @return  : checking M,N,P,Q parameters, or if the PLL is ON, or selecting wrong clock*
****************************************************************************************/
RCC_enuErrorStatus  RCC_enuConfigPLL(RCC_structCLKPLL Copy_structPLLCongif){

  RCC_enuErrorStatus Loc_enuerrorstatusPLL = RCC_enuOK;

  RCC_enuPLLSRC Loc_enuPLLSource = Copy_structPLLCongif.RCC_enuPLLSCR;
  u32 Loc_u32PLL_M = Copy_structPLLCongif.RCC_enuPLL_M;
  u32 Loc_u32PLL_N = Copy_structPLLCongif.RCC_enuPLL_N;
  u32 Loc_u32PLL_P = Copy_structPLLCongif.RCC_enuPLL_P;
  u32 Loc_u32PLL_Q = Copy_structPLLCongif.RCC_enuPLL_Q;

  u32 Loc_u32PLLCFGR_Register;

  /* CHECKING ERROR : M CONFIGURATION */
  if(Loc_u32PLL_M == 0 || Loc_u32PLL_M == 1 || Loc_u32PLL_M > 63){
    Loc_enuerrorstatusPLL = RCC_enuNotOK;
  }

  /* CHECKING ERROR : N CONFIGURATION */
  else if(Loc_u32PLL_N == 0 || Loc_u32PLL_N == 1 || Loc_u32PLL_N == 433 || Loc_u32PLL_N >= 511){
    Loc_enuerrorstatusPLL = RCC_enuNotOK;
  }

  /* CHECKING ERROR : P CONFIGURATION */
  else if((Loc_u32PLL_P != 2) && (Loc_u32PLL_P != 4) && (Loc_u32PLL_P != 6) && (Loc_u32PLL_P != 8)){
    Loc_enuerrorstatusPLL = RCC_enuNotOK;
  }

  /* CHECKING ERROR : Q CONFIGURATION */
  else if(Loc_u32PLL_Q == 0 || Loc_u32PLL_Q == 1 || Loc_u32PLL_Q > 15){
    Loc_enuerrorstatusPLL = RCC_enuNotOK;
  }

  /* CHECKING ERROR : CLOCK SOURCE */
  else if(Loc_enuPLLSource>HSE){
    Loc_enuerrorstatusPLL = RCC_enuNotOK;
  }

  /* CHICKING ERROR : PLL ON */
  else if((RCC_CR&MASK_GET_ON_PLL) != OFF){
        Loc_enuerrorstatusPLL = RCC_enuNotOK;
  }

  else{

    /*** CONFIGURE THE PPL ***/
    /* 1: CONFIGURE M */
    Loc_u32PLLCFGR_Register = RCC_PLLCFGR;
    Loc_u32PLLCFGR_Register &= ~MASK_SET_PPL_M;
    Loc_u32PLLCFGR_Register |= (Loc_u32PLL_M<<SHIFT_PLL_M);
    RCC_PLLCFGR = Loc_u32PLLCFGR_Register;

    /* 2: CONFIGURE N */
    Loc_u32PLLCFGR_Register = RCC_PLLCFGR;
    Loc_u32PLLCFGR_Register &= ~MASK_SET_PPL_N;
    Loc_u32PLLCFGR_Register |= (Loc_u32PLL_N<<SHIFT_PLL_N);
    RCC_PLLCFGR = Loc_u32PLLCFGR_Register;

    /* 3: CONFIGURE P */
    Loc_u32PLLCFGR_Register = RCC_PLLCFGR;
    Loc_u32PLLCFGR_Register &= ~MASK_SET_PPL_P;
    Loc_u32PLLCFGR_Register |= (Loc_u32PLL_P<<SHIFT_PLL_P);
    RCC_PLLCFGR = Loc_u32PLLCFGR_Register;

    /* 4: CONFIGURE Q */
    Loc_u32PLLCFGR_Register = RCC_PLLCFGR;
    Loc_u32PLLCFGR_Register &= ~ MASK_SET_PPL_Q;
    Loc_u32PLLCFGR_Register |= (Loc_u32PLL_Q<<SHIFT_PLL_Q);
    RCC_PLLCFGR = Loc_u32PLLCFGR_Register;

    /* 5: CONFIGURE CLOCK SOURCE */
    Loc_u32PLLCFGR_Register = RCC_PLLCFGR;
    Loc_u32PLLCFGR_Register &= ~MASK_SET_PPL_SRC;
    Loc_u32PLLCFGR_Register |= (Loc_enuPLLSource<<SHIFT_PLL_SRC);
    RCC_PLLCFGR = Loc_u32PLLCFGR_Register;
  }

  return Loc_enuerrorstatusPLL;
}

/************************************ Configure AHB ************************************
* @description :  Configure the AHB prescaler  ----------------------------------------*
* @param :        Copy_u32AHBPrescaler (u32)  : 0,2,4,8,16,64,128,254,512       -------*
* @return         error, you have a wrong choice --------------------------------------*                                                                             *
****************************************************************************************/
RCC_enuErrorStatus  RCC_enuControlPrescalerAHB(u32 Copy_u32AHBPrescaler){

  RCC_enuErrorStatus Ret_enuErrorStatus = RCC_enuOK;
  u32 Loc_u32CFGR_Register;

  /* SELECT PRESCALER */
  switch(Copy_u32AHBPrescaler){

    /* NO PRESCALER */
    case AHB_NO_CLOCK_FACTOR:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_AHB;
      Loc_u32CFGR_Register |= (AHB_NO_CLOCK_FACTOR<SHIFT_AHB);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 2 */
    case AHB_CLOCK_DIVIDED_BY_2:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_AHB;
      Loc_u32CFGR_Register |= (AHB_CLOCK_DIVIDED_BY_2<SHIFT_AHB);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 4 */
    case AHB_CLOCK_DIVIDED_BY_4:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_AHB;
      Loc_u32CFGR_Register |= (AHB_CLOCK_DIVIDED_BY_4<SHIFT_AHB);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 8 */
    case AHB_CLOCK_DIVIDED_BY_8:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_AHB;
      Loc_u32CFGR_Register |= (AHB_CLOCK_DIVIDED_BY_8<SHIFT_AHB);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 16 */
    case AHB_CLOCK_DIVIDED_BY_16:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_AHB;
      Loc_u32CFGR_Register |= (AHB_CLOCK_DIVIDED_BY_16<SHIFT_AHB);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 64 */
    case AHB_CLOCK_DIVIDED_BY_64:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_AHB;
      Loc_u32CFGR_Register |= (AHB_CLOCK_DIVIDED_BY_64<SHIFT_AHB);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 128 */
    case AHB_CLOCK_DIVIDED_BY_128:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_AHB;
      Loc_u32CFGR_Register |= (AHB_CLOCK_DIVIDED_BY_128<SHIFT_AHB);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 254 */
    case AHB_CLOCK_DIVIDED_BY_256:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_AHB;
      Loc_u32CFGR_Register |= (AHB_CLOCK_DIVIDED_BY_256<SHIFT_AHB);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 512 */
    case AHB_CLOCK_DIVIDED_BY_512:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_AHB;
      Loc_u32CFGR_Register |= (AHB_CLOCK_DIVIDED_BY_512<SHIFT_AHB);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* ERROR : NOT A CORRECT CHOICE */
    default:
      Ret_enuErrorStatus = RCC_enuNotOK;
    break;

  };
  return Ret_enuErrorStatus;
}

/****************************** Configure APB1 *****************************************
* @description :  Configure the APB1 prescaler  ---------------------------------------*
* @param :        Copy_u32AHBPrescaler (u32)  : 0,2,4,8,16       ----------------------*
* @return         error, you have a wrong choice --------------------------------------*                                                                             *
****************************************************************************************/
RCC_enuErrorStatus  RCC_enuControlPrescalerAPB1(u32 Copy_u32APB1Prescaler){
  RCC_enuErrorStatus Ret_enuErrorStatus = RCC_enuOK;
  u32 Loc_u32CFGR_Register;

  /* SELECT PRESCALER */
  switch(Copy_u32APB1Prescaler){

    /* NO PRESCALER */
    case APB1_NO_CLOCK_FACTOR:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_APB1;
      Loc_u32CFGR_Register |= (APB1_NO_CLOCK_FACTOR<SHIFT_APB1);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 2 */
    case AHB_CLOCK_DIVIDED_BY_2:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_APB1;
      Loc_u32CFGR_Register |= (APB1_CLOCK_DIVIDED_BY_2<SHIFT_APB1);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 4 */
    case AHB_CLOCK_DIVIDED_BY_4:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_APB1;
      Loc_u32CFGR_Register |= (APB1_CLOCK_DIVIDED_BY_4<SHIFT_APB1);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 8 */
    case AHB_CLOCK_DIVIDED_BY_8:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_APB1;
      Loc_u32CFGR_Register |= (APB1_CLOCK_DIVIDED_BY_8<SHIFT_APB1);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 16 */
    case AHB_CLOCK_DIVIDED_BY_16:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_APB1;
      Loc_u32CFGR_Register |= (APB1_CLOCK_DIVIDED_BY_16<SHIFT_APB1);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    default:
      Ret_enuErrorStatus = RCC_enuNotOK;
    break;
  };

  return Ret_enuErrorStatus;
}

/****************************** Configure APB2 *****************************************
* @description :  Configure the APB2 prescaler  ---------------------------------------*
* @param :        Copy_u32AHBPrescaler (u32)  : 0,2,4,8,16       ----------------------*
* @return         error, you have a wrong choice --------------------------------------*                                                                             *
****************************************************************************************/
RCC_enuErrorStatus  RCC_enuControlPrescalerAPB2(u32 Copy_u32APB2Prescaler){
  RCC_enuErrorStatus Ret_enuErrorStatus = RCC_enuOK;
  u32 Loc_u32CFGR_Register;

  /* SELECT PRESCALER */
  switch(Copy_u32APB2Prescaler){

    /* NO PRESCALER */
    case APB1_NO_CLOCK_FACTOR:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_APB2;
      Loc_u32CFGR_Register |= (APB1_NO_CLOCK_FACTOR<SHIFT_APB2);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 2 */
    case AHB_CLOCK_DIVIDED_BY_2:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_APB2;
      Loc_u32CFGR_Register |= (APB1_CLOCK_DIVIDED_BY_2<SHIFT_APB2);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 4 */
    case AHB_CLOCK_DIVIDED_BY_4:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_APB2;
      Loc_u32CFGR_Register |= (APB1_CLOCK_DIVIDED_BY_4<SHIFT_APB2);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 8 */
    case AHB_CLOCK_DIVIDED_BY_8:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_APB2;
      Loc_u32CFGR_Register |= (APB1_CLOCK_DIVIDED_BY_8<SHIFT_APB2);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    /* DIVIDED BY 16 */
    case AHB_CLOCK_DIVIDED_BY_16:
      Loc_u32CFGR_Register  = RCC_CFGR;
      Loc_u32CFGR_Register &= ~MASK_SET_PRE_APB2;
      Loc_u32CFGR_Register |= (APB1_CLOCK_DIVIDED_BY_16<SHIFT_APB2);
      RCC_CFGR = Loc_u32CFGR_Register;
    break;

    default:
      Ret_enuErrorStatus = RCC_enuNotOK;
    break;
  };

  return Ret_enuErrorStatus;
}

/****************************** Enable Peripheral **************************************
* @description :  Enable the prepherial on the selected bridge ------------------------*
* @param :        Copy_u32PeripheralBus (u32)  : AHB1,AHB2,APB1,APB2       ------------*
*                 Copy_u32Peripheral (u32): prepherials   -----------------------------*
*                   AHB1 : GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOH, CRC, DMA1, DMA2 --*
*                   AHB2 : OTGFS ------------------------------------------------------*
*                   APB1 : TIM2, TIM3, TIM4, TIM5, WWDG, SPI2, SPI3, USART2, I2C1,-----*
*                          I2C2,I2C3, PWR         -------------------------------------*
*                   APB2 : TIM1, USART1, USART6, ADC1, SDIO, SPI1, SPI4, SYSCFG,  -----*
*                          TIM9, TIM10, TIM11 -----------------------------------------*
* @return         error, you have a wrong choice of the bus ---------------------------*                                                                             *
****************************************************************************************/
RCC_enuErrorStatus  RCC_enuEnablePreipheral(u32 Copy_u32PeripheralBus, u32 Copy_u32Peripheral){

  RCC_enuErrorStatus Ret_enuErrorStatus = RCC_enuOK;

  /* SELECT BUS */
  switch(Copy_u32PeripheralBus){

    /* AHB1 BUS */
    case AHB1_BUS:
      RCC_AHB1ENR |= (1<<Copy_u32Peripheral);
    break;

    /* AHB2 BUS */
    case AHB2_BUS:
      RCC_AHB2ENR |= (1<<Copy_u32Peripheral);
    break;

    /* APB1 BUS */
    case APB1_BUS:
      RCC_APB1ENR |= (1<<Copy_u32Peripheral);
    break;

    /* APB2 BUS */
    case APB2_BUS:
      RCC_APB2ENR |= (1<<Copy_u32Peripheral);
    break;

    default:
      Ret_enuErrorStatus =  RCC_enuNotOK;
    break;
  };
  return Ret_enuErrorStatus;
}

/****************************** Disable Peripheral *************************************
* @description :  Disable the Prepherial on the selected bridge ------------------------*
* @param :        Copy_u32PeripheralBus (u32)  : AHB1,AHB2,APB1,APB2       ------------*
*                 Copy_u32Peripheral (u32): prepherials   -----------------------------*
*                   AHB1 : GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOH, CRC, DMA1, DMA2 --*
*                   AHB2 : OTGFS ------------------------------------------------------*
*                   APB1 : TIM2, TIM3, TIM4, TIM5, WWDG, SPI2, SPI3, USART2, I2C1,-----*
*                          I2C2,I2C3, PWR         -------------------------------------*
*                   APB2 : TIM1, USART1, USART6, ADC1, SDIO, SPI1, SPI4, SYSCFG,  -----*
*                          TIM9, TIM10, TIM11 -----------------------------------------*
* @return         error, you have a wrong choice of the bus ---------------------------*                                                                             *
****************************************************************************************/
RCC_enuErrorStatus  RCC_enuDisablePreipheral(u32 Copy_u32PeripheralBus, u32 Copy_u32Peripheral){

  RCC_enuErrorStatus Ret_enuErrorStatus = RCC_enuOK;

  /* SELECT BUS */
  switch(Copy_u32PeripheralBus){

    /* AHB1 BUS */
    case AHB1_BUS:
      RCC_AHB1ENR &= (~(1<<Copy_u32Peripheral));
    break;

    /* AHB2 BUS */
    case AHB2_BUS:
      RCC_AHB2ENR &= (~(1<<Copy_u32Peripheral));
    break;

    /* APB1 BUS */
    case APB1_BUS:
      RCC_APB1ENR &= (~(1<<Copy_u32Peripheral));
    break;

    /* APB2 BUS */
    case APB2_BUS:
      RCC_APB2ENR &= (~(1<<Copy_u32Peripheral));
    break;

    default:
      Ret_enuErrorStatus =   RCC_enuNotOK;
    break;
  };
  return Ret_enuErrorStatus;
}

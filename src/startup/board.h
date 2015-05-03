#ifndef _BOARD_H_
#define _BOARD_H_

#include "common.h"


/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define XTAL        (12000000UL)        /* Oscillator frequency               */
#define OSC_CLK     (      XTAL)        /* Main oscillator frequency          */
#define RTC_CLK     (   32000UL)        /* RTC oscillator frequency           */
#define IRC_OSC     ( 4000000UL)        /* Internal RC oscillator frequency   */


#define FOSC                        XTAL                           	 /*  ����Ƶ��                  */


// FCCLK = FOSC  * 8 = 96MHZ
#define FCCLK                      (FOSC  * 8)                          /*  ��ʱ��Ƶ��<=100Mhz          */
                                                                        /*  FOSC��������                */
#define FCCO                       (FCCLK * 4)                          /*  PLLƵ��(275Mhz~550Mhz)      */
                                                                        /*  ��FCCLK��ͬ���������ż���� */
#define FPCLK                      (FCCLK / 4)                          /*  ����ʱ��Ƶ��,FCCLK��1/2��1/4*/
                                                                        /*  ����FCCLK��ͬ               */

#define PLL_NVALUE                  1UL                                   /*  1~256                       */
#define PLL_MVALUE                ((FCCO / FOSC) * (PLL_NVALUE) / 2)    /*  6~512                       */


void LPC_systemInit (void);

#endif

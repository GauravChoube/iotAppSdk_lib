/*
 * cotrollerTimerLL.h
 *
 *  Created on: May 6, 2020
 *      Author: gaurav
 */

#ifndef MAIN_CONTROLLERLL_CONTROLLERTIMERLL_H_
#define MAIN_CONTROLLERLL_CONTROLLERTIMERLL_H_

#include <msp430.h>
#include "driverlib.h"


#define TIMER_DIVIDER         16                                // Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds


typedef enum
{
    timer0,
    timer1,
    timer2,
    timer3,
    timer4,
}timerCnt;

typedef enum
{
 mode_msec = 0,
 mode_sec,

}ModeOfTimer;


bool timerInit(uint8_t TimerSelction,bool modeoftimer,void (*funcptr)());
bool timerStart(uint8_t TimerSelction,double time );
bool timerStop(uint8_t TimerSelction);



#endif /* MAIN_CONTROLLERLL_CONTROLLERTIMERLL_H_ */

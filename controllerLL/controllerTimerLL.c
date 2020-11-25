/*
 * cotrollerTimerLL.c
 *
 *  Created on: May 6, 2020
 *      Author: gaurav
 */

#include "controllerTimerLL.h"
#include <stddef.h>


void (*timer0cb)() = 0;
void (*timer1cb)() = 0;
void (*timer2cb)() = 0;
void (*timer3cb)() = 0;
void (*timer4cb)() = 0;


#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER_A0_isr(void)
{
    timer0cb();
    TA0CTL &=~MC_3;
    TA0CTL|=TACLR;

}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER_A1_isr(void)
{
    timer1cb();
//    TA1CTL &=~MC_3;
    TA1CTL|=TACLR;

}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void TIMER_A2_isr(void)
{
    timer2cb();
    //TA2CTL &=~MC_3;
    TA2CTL|=TACLR;

}

#pragma vector=TIMER3_A0_VECTOR
__interrupt void TIMER_A3_isr(void)
{
    timer3cb();
    TA3CTL &=~MC_3;
    TA3CTL|=TACLR;

}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER_B0_isr(void)
{
    timer4cb();
    TB0CTL &=~MC_3;
    TB0CTL|=TACLR;

}




bool timerInit(uint8_t TimerSelction,bool modeoftimer,void (*funcptr)())
{

    Timer_A_initUpModeParam paramTimer={0};
    Timer_B_initUpModeParam paramTimer1 = {0};

                                                                   //set this 100msec
    paramTimer.captureCompareInterruptEnable_CCR0_CCIE=TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    paramTimer.timerInterruptEnable_TAIE =  TIMER_A_TAIE_INTERRUPT_DISABLE;
    paramTimer.startTimer=false;



    switch(TimerSelction)
    {

    case timer0:
        Timer_A_stop(TIMER_A0_BASE);
        paramTimer.clockSource=TIMER_A_CLOCKSOURCE_ACLK;
        paramTimer.clockSourceDivider=TIMER_A_CLOCKSOURCE_DIVIDER_32;
        paramTimer.timerPeriod=50;
        Timer_A_initUpMode(TIMER_A0_BASE,&paramTimer);
        Timer_A_clear(TIMER_A0_BASE);



        timer0cb = funcptr;

    	break;

    case timer1:
        Timer_A_stop(TIMER_A1_BASE);
        paramTimer.clockSource=TIMER_A_CLOCKSOURCE_ACLK;
        paramTimer.clockSourceDivider=TIMER_A_CLOCKSOURCE_DIVIDER_32;

        if(modeoftimer == mode_msec) paramTimer.timerPeriod=1;
        else if(modeoftimer == mode_sec) paramTimer.timerPeriod=1024;

        Timer_A_initUpMode(TIMER_A1_BASE,&paramTimer);
        Timer_A_clear(TIMER_A1_BASE);

        timer1cb = funcptr;

    	break;

    case timer2:
        Timer_A_stop(TIMER_A2_BASE);
        paramTimer.clockSource=TIMER_A_CLOCKSOURCE_ACLK;
        paramTimer.clockSourceDivider=TIMER_A_CLOCKSOURCE_DIVIDER_32;
        if(modeoftimer == mode_msec) paramTimer.timerPeriod=1;
        else if(modeoftimer == mode_sec) paramTimer.timerPeriod=1000;
        Timer_A_initUpMode(TIMER_A2_BASE,&paramTimer);
        Timer_A_clear(TIMER_A2_BASE);

        timer2cb = funcptr;
    	break;

    case timer3:
        Timer_A_stop(TIMER_A3_BASE);
        paramTimer.clockSource=TIMER_A_CLOCKSOURCE_ACLK;
        paramTimer.clockSourceDivider=TIMER_A_CLOCKSOURCE_DIVIDER_32;

        if(modeoftimer == mode_msec) paramTimer.timerPeriod=1;
        else if(modeoftimer == mode_sec) paramTimer.timerPeriod=1000;

        Timer_A_initUpMode(TIMER_A3_BASE,&paramTimer);
        Timer_A_clear(TIMER_A3_BASE);

        timer3cb = funcptr;
    	break;

    case timer4:
        Timer_B_stop(TIMER_B0_BASE);
        paramTimer1.clockSource=TIMER_B_CLOCKSOURCE_ACLK;
        paramTimer1.clockSourceDivider=TIMER_B_CLOCKSOURCE_DIVIDER_32;
        if(modeoftimer == mode_msec) paramTimer1.timerPeriod=1;
        else if(modeoftimer == mode_sec) paramTimer1.timerPeriod=1000;

        Timer_B_initUpMode(TIMER_B0_BASE,&paramTimer1);
        Timer_B_clear(TIMER_B0_BASE);


        timer4cb = funcptr;


        break;

    }


    return 1;
}
/*
 * Start timer has argument time must be enter in the form of ms
 */
bool timerStart(uint8_t TimerSelction,double time )
{
    switch(TimerSelction)
    {
    case timer0:

        Timer_A_stop(TIMER_A0_BASE);
        TA0CCR0 = (uint16_t)time;

        Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);


        break;

    case timer1:
        Timer_A_stop(TIMER_A1_BASE);
        TA1CCR0 = (uint16_t)time;

        Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);


        break;

    case timer2:
        Timer_A_stop(TIMER_A2_BASE);
        TA2CCR0 = (uint16_t)time;

        Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);

        break;

    case timer3:
        Timer_A_stop(TIMER_A3_BASE);
        TA3CCR0 = (uint16_t)time;

        Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
        break;

    case timer4:
        Timer_B_stop(TIMER_B0_BASE);
        TB0CCR0 = (uint16_t)time;

        Timer_B_startCounter(TIMER_B0_BASE, TIMER_A_UP_MODE);

            break;

    }

    return 1;
}

bool timerStop(uint8_t TimerSelction)
{

    switch(TimerSelction)
    {
    case timer0:     Timer_A_stop(TIMER_A0_BASE);      break;
    case timer1:     Timer_A_stop(TIMER_A1_BASE);      break;
    case timer2:     Timer_A_stop(TIMER_A2_BASE);      break;
    case timer3:     Timer_A_stop(TIMER_A3_BASE);      break;
    case timer4:     Timer_B_stop(TIMER_B0_BASE);      break;
    }

    return 1;
}


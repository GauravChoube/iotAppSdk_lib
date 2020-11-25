/*
 * internalRtc.c
 *
 *  Created on: Aug 25, 2020
 *      Author: Gcsystem
 */
#include "internalRtc.h"
#include <string.h>
#include <stdlib.h>

void (*rtcCb)();
char *lowPowerExitflagptr = 0;


uint16_t rtcInit(void (*Buff)(),char *lowPowerExitflag)
{
    RTC_B_holdClock(RTC_B_BASE);
    RTC_B_definePrescaleEvent(RTC_B_BASE, RTC_B_PRESCALE_0, RTC_B_PSEVENTDIVIDER_2);
    RTC_B_definePrescaleEvent(RTC_B_BASE, RTC_B_PRESCALE_1, RTC_B_PSEVENTDIVIDER_2);

    RTC_B_setCalendarEvent(RTC_B_BASE,RTC_B_CALENDAREVENT_MINUTECHANGE);

    RTC_B_enableInterrupt(RTC_B_BASE, RTC_B_TIME_EVENT_INTERRUPT);

    RTC_B_clearInterrupt(RTC_B_BASE, RTC_B_TIME_EVENT_INTERRUPT);

    RTC_B_startClock(RTC_B_BASE);

    rtcCb = Buff;
    lowPowerExitflagptr = lowPowerExitflag;

    return  1;


}

uint16_t rtcset(char *rtcbuffer)
{
    char temp[3]={0};
    RTC_B_holdClock(RTC_B_BASE);
    Calendar RTC_time={0};
    //2020-08-26T10:05:46.688Z
    strncpy(temp,rtcbuffer+17,2);
    RTC_time.Seconds=atoi(temp);

    strncpy(temp,rtcbuffer+14,2);
    RTC_time.Minutes=atoi(temp);

    strncpy(temp,rtcbuffer+11,2);
    RTC_time.Hours = atoi(temp);

    strncpy(temp,rtcbuffer+8,2);
    RTC_time.DayOfMonth=atoi(temp);

    strncpy(temp,rtcbuffer+5,2);
    RTC_time.Month=atoi(temp);

    strncpy(temp,rtcbuffer+2,2);
    RTC_time.Year=atoi(temp);

    RTC_B_initCalendar(RTC_B_BASE, &RTC_time, RTC_B_FORMAT_BINARY);

    RTC_B_startClock(RTC_B_BASE);

    return 1;
}

uint16_t rtcget(char *Rtc_Data)
{
    Calendar Current_Time;
        char  n1=0,n2=0;

        //&F10=115056,230515-hhmmss,ddmmyy

        //APPEND RTC VALUES TO THE DATA
        Current_Time = RTC_B_getCalendarTime(RTC_B_BASE);
        //HOUR-hh
        n1 = Current_Time.Hours/10;
        n1 = n1+48;
        *Rtc_Data++ = n1;
        n2 = Current_Time.Hours%10;
        n2 = n2+48;
        *Rtc_Data++ = n2;
        //MINUTES-mm
        n1 = Current_Time.Minutes/10;
        n1 = n1+48;
        *Rtc_Data++ = n1;
        n2 = Current_Time.Minutes%10;
        n2 = n2+48;
        *Rtc_Data++ = n2;
        //SECONDS-ss
        n1 = Current_Time.Seconds/10;
        n1 = n1+48;
        *Rtc_Data++ = n1;
        n2 = Current_Time.Seconds%10;
        n2 = n2+48;
        *Rtc_Data++ = n2;

        *Rtc_Data++ = ',';//TIME,DATE

        //DAY-dd
        n1 = Current_Time.DayOfMonth/10;
        n1 = n1+48;
        *Rtc_Data++ = n1;
        n2 = Current_Time.DayOfMonth%10;
        n2 = n2+48;
        *Rtc_Data++ = n2;
        //MONTH-mm
        n1 = Current_Time.Month/10;
        n1 = n1+48;
        *Rtc_Data++ = n1;
        n2 = Current_Time.Month%10;
        n2 = n2+48;
        *Rtc_Data++ = n2;
        //YEAR-yy
        n1 = Current_Time.Year/10;
        n1 = n1+48;
        *Rtc_Data++ = n1;
        n2 = Current_Time.Year%10;
        n2 = n2+48;
        *Rtc_Data++ = n2;
return 1;
}


#pragma vector = RTC_VECTOR
__interrupt void RTC_ISR(void)
{
    switch(__even_in_range(RTCIV, RTCIV_RT1PSIFG))
        {
            case RTCIV_NONE:      break;        // No interrupts
            case RTCIV_RTCOFIFG:  break;        // RTCOFIFG
            case RTCIV_RTCRDYIFG:               // RTCRDYIFG
                                // Toggles P1.0 every second
                break;
            case RTCIV_RTCTEVIFG:               // RTCEVIFG
            {
                rtcCb();
                if(*lowPowerExitflagptr == 2)
                {
                    *lowPowerExitflagptr = 0;
                    LPM3_EXIT;
                }
            }break;
            case RTCIV_RTCAIFG:   break;        // RTCAIFG
            case RTCIV_RT0PSIFG:  break;        // RT0PSIFG
            case RTCIV_RT1PSIFG:  break;        // RT1PSIFG
            default: break;
        }

}

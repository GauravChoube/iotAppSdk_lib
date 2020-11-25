/*
 * internalRtc.h
 *
 *  Created on: Aug 25, 2020
 *      Author: Gcsystem
 */

#ifndef APPLICATION_RTC_INTERNALRTC_H_
#define APPLICATION_RTC_INTERNALRTC_H_

#include "msp430.h"
#include "driverlib.h"




uint16_t rtcInit(void (*Buff)(),char *);
uint16_t rtcset(char *rtcbuffer);
uint16_t rtcget(char *rtcbuffer);



#endif /* APPLICATION_RTC_INTERNALRTC_H_ */

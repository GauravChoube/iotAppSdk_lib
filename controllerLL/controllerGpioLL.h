/*
 * cotrollerGpio.h
 *
 *  Created on: May 6, 2020
 *      Author: gaurav
 */

#ifndef MAIN_CONTROLLERLL_CONTROLLERGPIOLL_H_
#define MAIN_CONTROLLERLL_CONTROLLERGPIOLL_H_


#include <msp430.h>
#include "driverlib.h"

#define GPIO_OUTPUT 1
#define GPIO_INPUT  0

#define GPIO_HIGH    1
#define GPIO_LOW     0


uint16_t gpioSetup(uint16_t Port,uint16_t Pin,uint16_t direction,bool interrupenable,bool intedge,void (*func)(char));
uint16_t gpioSetHigh(uint16_t Port,uint16_t Pin);
uint16_t gpioSetLow(uint16_t Port,uint16_t Pin);
uint8_t  gpioGet(uint16_t Port,uint16_t Pin);
#endif /* MAIN_CONTROLLERLL_CONTROLLERGPIOLL_H_ */

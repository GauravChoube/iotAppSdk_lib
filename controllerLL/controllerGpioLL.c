/*
 * cotrollerGpioLL.c
 *
 *  Created on: May 6, 2020
 *      Author: gaurav
 */

#include "controllerGpioLL.h"

void (*gpiocb)(char ) = 0;

#pragma vector=PORT1_VECTOR
__interrupt void port1()
{
    gpiocb(1);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN_ALL16);
}

#pragma vector=PORT2_VECTOR
__interrupt void port2()
{
    gpiocb(2);
}


#pragma vector=PORT3_VECTOR
__interrupt void port3()
{
    gpiocb(3);
}


#pragma vector=PORT4_VECTOR
__interrupt void port4()
{
    gpiocb(4);
}


uint16_t gpioSetup(uint16_t Port,uint16_t Pin,uint16_t direction,bool interrupenable,bool intedge,void (*func)(char))
{
    if(direction == GPIO_OUTPUT)
    {
        GPIO_setAsOutputPin(Port, Pin);
    }
    else
    {
        GPIO_setAsInputPin(Port, Pin);
    }

    if(interrupenable == true)
    {
        gpiocb = func;
        GPIO_selectInterruptEdge(Port, Pin,intedge);
//        GPIO_enableInterrupt(Port, Pin);



    }



return 1;

}

uint16_t gpioSetHigh(uint16_t Port,uint16_t Pin)
{
    GPIO_setOutputHighOnPin(Port, Pin);
	return 1;
}


uint16_t gpioSetLow(uint16_t Port,uint16_t Pin)
{
    GPIO_setOutputLowOnPin(Port, Pin);
        return 1;

}

uint8_t gpioGet(uint16_t Port,uint16_t Pin)
{
   return GPIO_getInputPinValue(Port,Pin);

}

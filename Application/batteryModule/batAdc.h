/*
 * controllerAdcLL.h
 *
 *  Created on: Sep 19, 2020
 *      Author: Gcsystem
 */

#ifndef BATADC_H_
#define BATADC_H_
#include <msp430.h>
#include "driverlib.h"

/*
 * variable
 */
bool batMeasuringInProgress;
float batteryVoltage;

/*
 * macro
 */
#define BAT_PORT           GPIO_PORT_P1
#define BAT_PIN            GPIO_PIN4
#define BAT_AVG_CNT        10

#define MAINS_PORT         GPIO_PORT_P2
#define MAINS_PIN          GPIO_PIN4
#define MAINS_AVG_CNT      10

typedef enum
{
   MainsBat,
   BackupBat,
}powerType;



/*
 * function
 */
void batAdcInit();
unsigned int batConfiguration(char powerType,char batport,char batpin);
float batVoltageRead(char powerType);


#endif /* BATADC_H_ */

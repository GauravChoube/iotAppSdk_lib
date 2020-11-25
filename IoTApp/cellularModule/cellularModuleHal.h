
/***************************************************************************************
 * Filename    : cellularModulePeriApp.h
 * Created on  : Feb 11, 2020
 * Author      : Gaurav Choubey
 ****************************************************************************************/
#ifndef CELLULARMODULEHAL_H_
#define CELLULARMODULEHAL_H_

/****************************************************************************************
 * HEADER FILE
 ***************************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/****************************************************************************************
 * VARIABLE
 ***************************************************************************************/
extern volatile bool        cellularMAxTimeout;
extern bool                 directDataAvailable;
extern volatile int8_t      cellularMAxTimerOn;

extern bool                 StartModemtimerflag;
extern uint16_t             ModemtimerTime ;
extern volatile uint16_t    ModemtimerCounter;
extern uint8_t              ModemtimerOn;
extern volatile bool        ModemtimerTimeOut;
extern  char                modemReadyRecv;

/****************************************************************************************
* FUNCTION
***************************************************************************************/
void  CellularModuleUartCb(int8_t *Buff,uint16_t Buflen);
bool  cellularUartInit();
bool  cellularUartReInit();
void  CellularModuleTxChar(int8_t ch);
void  CellularModuleTxstr(int8_t *str);

bool  cellularMAxTimerInit();
void  StartcellularMAxTimer(uint16_t time);
void  StopcellularMAxTimer();

bool cellularOpertionAoTimerInit();
void StartcellularOpertionAoTimer(double time);
void StopcellularOpertionAoTimer();

bool cellularBufferTimerInit();
void startCellularBufferTimer(uint16_t timein_ms);
void cellularBufferTimerCb();

void waitForSecond(uint16_t sect);
void startModemTimer(uint16_t time);
void stopModemTimer();
void startCellularBufferTimer(uint16_t time10ms);



bool     ConfigureModemPins();
uint8_t  turnOnCellular();
uint8_t  turnOffCellular();
uint16_t powertheModem();
uint16_t restarttheModem();
char *strnstr( char *src, char *dest,unsigned int n);

#endif /* CELLULARMODULEHAL_H_ */

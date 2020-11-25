/***************************************************************************************
 * Filename    : iotAPP.h
 * Created on  : May 8, 2020
 * Author      : Gaurav Choubey
 ****************************************************************************************/
#ifndef MAIN_IOTAPP_IOTAPP_H_
#define MAIN_IOTAPP_IOTAPP_H_

/****************************************************************************************
 * HEADER FILE
 ***************************************************************************************/
#include <msp430.h>
#include <stdint.h>
#include <math.h>
#include "iotComman.h"
#include "../Application/Application.h"

/****************************************************************************************
 * VARIABLE
 ***************************************************************************************/
#ifdef CONFIG_CALL_ENABLE
#ifdef CONFIG_TIME_HARDCODED
#define CONFIG_CALL_TIME    1
#endif
#endif

/*
 * IOT application return value on different scenario.
 */
typedef enum
{
 FUNCTION_SUCCESS=0xf0,
 FUNCTION_FAILED ,
 PASS_SUCCESS     ,
 SERVER_BUSY       ,
 RESTART_MODEM,
 RESTART_INTERNET,
 HTTP_INIT_SUCCESS,
 CONNECTION_CLOSED,
 CONFIG_CALL_FAILED,

}MAIN_CODE_RESPONSES;

typedef enum
{
    CELLULAR_TYPE=1,
    WIFI_TYPE,


}MODULETYPE;


/*
 * iot Application's temporary variable which is store into volatile
 * memory.
 */
typedef struct
{
   char		Connectivity:1;
   char		ConfigCallReq:1;
   char     ConfigCallSend:1;
   char     prConfigDone:1;
   char     sendingInProcess:1;
   char     moduleChanged:1;
   char     deviceInOperation:1;
   char     dataCallCredentialForm:1;
   char     dataCallConfig:1;

   char     moduleType;
   int8_t   devicePowerOn;





} __attribute__((packed))iAF;

/*
 * iot Application's permanent variable which is store into non-volatile
 * memory.
 */
typedef struct
{
   uint8_t     setupConfigDone;
   uint8_t     devicePowerOn;
   uint8_t     subscriptionover;
   uint8_t     configCallRestartflag;


} __attribute__((packed))iAFPermant;

extern iAFPermant iotAppflagsPerment;
extern iAF        iotAppflags;
extern char       enterToSleep;
extern uint16_t   ConfigCallTime;

/****************************************************************************************
* FUNCTION
***************************************************************************************/
/**************************************************************************************
 * Description:
 * ============
 *  Initialize related all section for IOT Application.
 *  Hardware initialize and cellular module library initialized.
 *
 * INPUT:
 * ======
 * > None
 *
 * RETURN:
 * =======
 * > None
 *
 *
 *************************************************************************************/
uint16_t iotAppInit();

/**************************************************************************************
 * Description:
 * ============
 *  iotAppRun is a IOT Application which actually configure the device with configuration
 *  cloud if feature enable and upload the data whenever its available and depend of network
 *  it will upload the data on given server url over selected protocols.
 *  Please fill the all information in config.h file in order to use iot APP library.
 *
 *
 * INPUT:
 * ======
 * > None
 *
 * RETURN:
 * =======
 * > None
 *
 *
 *************************************************************************************/
void iotAppRun(void *arg);

#endif /* MAIN_IOTAPP_IOTAPP_H_ */

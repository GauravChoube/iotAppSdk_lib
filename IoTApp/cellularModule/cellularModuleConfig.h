/***************************************************************************************
 * Filename    : cellularModuleConfig.h
 * Created on  : Feb 12, 2020
 * Author      : Gaurav Choubey
 ****************************************************************************************/

#ifndef IOTAPP_CELLULARMODULE_CELLULARMODULECONFIG_H_
#define IOTAPP_CELLULARMODULE_CELLULARMODULECONFIG_H_

/****************************************************************************************
 * HEADER FILE
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../Config.h"
#include "../controllerLL/controllerGpioLL.h"
#include "../controllerLL/controllerTimerLL.h"
#include "../controllerLL/controllerUartLL.h"

/****************************************************************************************
 * VARIABLE
 ***************************************************************************************/

#define cellularBufferSize      1024
/***************************************************************
 *1.Configure Power and Status pin
 ***************************************************************/
#define cellMPowerPort          GPIO_PORT_P4
#define cellMPowerPin           GPIO_PIN6

#define cellMStatusPort         GPIO_PORT_P3
#define cellMStatusPin          GPIO_PIN7

/***************************************************************
 *                    CELLULAR PERIPERAL SECTION
 ***************************************************************/

#define cellularUart                uart0
#define cellularOpertionAotimer     timer1
#define cellularBufferTimer         timer0


/***************************************************************
 *                  RESET API
 ***************************************************************/
#define RESET_DEVICE            resetSystem

/***************************************************************
 *                  CORE DELAY API
 ***************************************************************/
#define delayCellMModule               __delay_cycles

/***************************************************************
 *                 Debug  UART API
 ***************************************************************/

#ifdef Debug_Enable
//#define debugStr                 debugLog
#endif
void (*debugStr)(char * );
/***************************************************************
 *                  GPS section
 ***************************************************************/
#define GPS_AUTO_ENABLE
#define GPS_OVER_RESTART

/***************************************************************
 *                Select Connectivity Module Network Type
 ***************************************************************/
//Set the APN of SIM here
#define APN    "bsnlnet"


/*  Type of Network
 NW_AUTO
 NW_GSM
 NW_WCDMA
 NW_LTE
 NW_TDS_CDMA
 NW_CDMA
 NW_EVDO
 NW_GSM_WCDMA
 NW_CDMA_EVDO
 NW_LTE_ANY_MODE
 NW_GSM_TDSCDMA
 NW_GSM_WCDMA_TDSCDMA
 NW_CDMA_EVDO_GSM_WCDMA_TDSCDMA
 NW_GSM_WDMA_LTE
 NW_WDMA_LTE

*/
#define NW_TYPE  NW_GSM

/**************************************************************************************/
typedef enum
{
    failed =0,
    Success,

    CELL_CLOUDCMD_RES_SUCCESS =10,
    CELL_CLOUDCMD_RES_FAILED,

    CELL_FUNCTION_SUCCESS=0xf0,
    CELL_FUNCTION_FAILED ,
    CELL_PASS_SUCCESS    ,
    CELL_SERVER_BUSY       ,
    CELL_RESTART_MODEM ,
    CELL_RESTART_INTERNET,
    CELL_HTTP_INIT_SUCCESS,
    CELL_CONNECTION_CLOSED,

}flag;



struct cell
{
    char configDone : 1;
    char httpConnPreConfigure:1;
    char tcpConnPreConfigure:1;
    char ssltlsConnPreConfigure:1;
    char mqttConnPreConfigure:1;

} __attribute__((packed));



#endif /* IOTAPP_CELLULARMODULE_CELLULARMODULECONFIG_H_ */

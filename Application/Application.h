/*
 * Application.h
 *
 *  Created on: Aug 21, 2020
 *      Author: Gcsystem
 */

#ifndef APPLICATION_APPLICATION_H_
#define APPLICATION_APPLICATION_H_

#include <msp430.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "driverlib/MSP430FR5xx_6xx/driverlib.h"
#include "extflash/spiFlash_AT45DB081.h"
#include "batteryModule/batAdc.h"
#include "clock/clock.h"
#include "rtc/internalRtc.h"

#include "controllerLL/controllerUartLL.h"
#include "controllerLL/controllerGpioLL.h"
#include "controllerLL/controllerTimerLL.h"


#include "IoTApp/iotAPP.h"
#include "IoTApp/cellularModule/cellularModuleHal.h"


/*****************************************************************************************************************************/
//CLOCK



/*****************************************************************************************************************************/
//DEBUG
#define debugUart   uart1


/*****************************************************************************************************************************/
//EXT FLASH
/******************************************************************************************
 * CLASS 1: PARAMETER STORAGE SECTION
 * ===================================
 * 1.First Block of external flash memory i.e. 64KB is always reserved for storing parameter
 *   of application.Its has 16 Sector of size 4KB and there description as follow
 *
 *   SECTOR-I (0x0000 -0x0FFF) : Parameter stored once after Power On or Reset.
 *   SECTOR-II(0x1000 -0x1FFF) : Parameter of setting(cloud or server) stored.
 *   SECTOR-III to XIV(0x2000 -0xFFFF): Reserved
 *
 * CLASS 2 : FOTA STORAGE SECTION
 * ===============================
 * 2.BLock 2 to block 7 are dedicated for fota process
 *   (0x10000 to 0x6FFFF) - 6 Block.
 *
 * CLASS 3 : APPLCATION STORAGE SECTION
 * ====================================
 * 3.Block-8 to Block-15 are dedicated application payload storage.
 *   0x70000 to 0xEFFFF) - 8 Block.
 *
 * CLASS 4 : CONNECTIVITY MODULE STORAGE SECTION
 * ==============================================
 * 4.Block-last are dedicated application connectivity module storage.
 *   0xEFFFF to 0xFFFFF) - 1 Block.
 *
 ******************************************************************************************/
//CLASS 1
//========
//SECTOR-I =>
#define SECTOR_1        0x0000

//SECTOR-II =>
#define SECTOR_2        0x1000
/****************************************/
//PARAMETER         : Alias Name (Machine Name)
//BYTES ALLOCATED   : 20
/****************************************/
#define ALIAS_NAME_SA   SECTOR_2
#define ALIAS_NAME_EA   ALIAS_NAME_SA+20

/****************************************/
//PARAMETER         : OWNER NAME
//BYTES ALLOCATED   : 20
/****************************************/
#define OWNER_NAME_SA       ALIAS_NAME_EA+1     //Starting Address
#define OWNER_NAME_EA       OWNER_NAME_SA+20     //End Addresss


/****************************************/
//PARAMETER         : HUB URL CONNNECTION STRING
//BYTES ALLOCATED   : 50
/****************************************/
#define DATA_URL_SA          OWNER_NAME_EA+1      //Starting Address
#define DATA_URL_EA          DATA_URL_SA+50      //End Address

/****************************************/
//PARAMETER         : HUB SAS TOKEN
//BYTES ALLOCATED   : 100
/****************************************/
#define DATA_PASS_SA          DATA_URL_EA+1      //Starting Address
#define DATA_PASS_EA          DATA_PASS_SA+100      //End Address

/****************************************/
//PARAMETER         : Country Name
//BYTES ALLOCATED   : 20
/****************************************/
#define COUNTRY_NAME_SA     DATA_PASS_EA+1      //Starting Address
#define COUNTRY_NAME_EA     COUNTRY_NAME_SA+20      //End Addresss

/****************************************/
//PARAMETER         : DATA_SENDENDPOINT
//BYTES ALLOCATED   : 100
/****************************************/
#define DATA_SENDENDPOINT_SA          COUNTRY_NAME_EA+1      //Starting Address
#define DATA_SENDENDPOINT_EA          DATA_SENDENDPOINT_SA+100      //End Address

/****************************************/
//PARAMETER         : DATA_RECIEVEPOINT
//BYTES ALLOCATED   : 100
/****************************************/
#define DATA_RECIEVEPOINT_SA          DATA_SENDENDPOINT_EA+1      //Starting Address
#define DATA_RECIEVEPOINT_EA          DATA_RECIEVEPOINT_SA+100      //End Address



//CLASS 2
//========

//CLASS 3
//========
#define APPLICATION_SA      SECTOR8_ADDRESSS
#define APPLICATION_EA      SECTOR16_ADDRESSS


//CLASS 4
//========


/*****************************************************************************************************************************/
//Application variable
typedef enum
{
    setupflag = 1,
    Suboverflag,


}internalFlashMacro;

typedef enum
{
    APP_FAILED=0,
    APP_SUCCESS,
    CLOUDCMD_RES_SUCCESS =10,
    CLOUDCMD_RES_FAILED,
    APP_PACKET_FOMR_SUCCESS,
}CLOUDCMD_RES;

typedef enum
{
    ACTIVE_POWER_MODE,
    LOW_POWER_MODE,
    BACKUP_POWER_MODE,


}powerState;

typedef enum
{
    FORM_LOCATION_PACKET = 0xDB,
    FORM_HEARTBEAT_PACKET,
    FORM_PW_FAIL_PACKET,
    FORM_BAT_ALT_PACKET,

}PACKET_TYPE;

extern int8_t DEVICE_ID[17];
extern char     lowPowerIndication;
#define LOW_VOLTAGE_THRESOLD 3.8

/*****************************************************************************************************************************/
//FUNCTION API
uint16_t applicationInit();
uint16_t applicationPacketFormTask();
uint16_t applicationCloudResponse(int8_t *buf,uint16_t buflen);
bool     applicationDataAvailable();
bool     applicationCofigCallform(char *iotCommanTemp);

uint16_t extFlashInit();
uint16_t storedAppData(int8_t *data,uint16_t datalen);
uint16_t getAppData(int8_t *Buffer,uint16_t Bufferlen);
void     updateAppDataAddress( uint16_t len);

uint16_t internalFlashInit();
bool     internalflashWrite(uint16_t key,uint8_t Var);
bool     internalflashRead(uint16_t key,uint8_t *Var);


uint16_t debugUartInit();
void debugUartTx(char * str);

uint16_t watchdogInit();
void     watchdogreset();

uint16_t powerModuleInit();
uint16_t powerModuleStatus(char *pwBuffer);

void getPowerUpReason();
void sleepDevice();
void applicationSyncCnt();
void restartEveryThing(void);









#endif /* APPLICATION_APPLICATION_H_ */

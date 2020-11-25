/***************************************************************************************
 * Filename    : iotComman.h
 * Created on  : May 9, 2020
 * Author      : Gaurav Choubey
 ****************************************************************************************/
#ifndef MAIN_IOTAPP_IOTCOMMAN_H_
#define MAIN_IOTAPP_IOTCOMMAN_H_

/*======================================================================================================================================================================
   Header declaration
======================================================================================================================================================================*/
#include "Config.h"

#ifdef WIFI_MODULE

#endif
#ifdef CELLULAR_MODULE
//#include "cellularModule/cellularModuleHeader.h"
//#include "cellularModule/coreCellularModule/cellularModuleLLSim800.h"
//#include "cellularModule/coreCellularModule/cellularUnsolicRespons.h"
//#include "cellularModule/coreCellularModule/cellularModuleHTTP.h"
//#include "cellularModule/coreCellularModule/cellularModuleMQTT.h"
//#include "cellularModule/cellularModuleHal.h"
#include "cellularModule/cellHeader.h"
#endif
#include "../controllerLL/controllerUartLL.h"
#include "../controllerLL/controllerGpioLL.h"
#include "../controllerLL/controllerTimerLL.h"

/*======================================================================================================================================================================
   MAacros and Custom variable declaration
======================================================================================================================================================================*/
#define iotCommanBufSize 			1024


typedef uint16_t (*setupfxn)(void);
typedef uint16_t (*checkNetworkfxn)(void);
typedef uint16_t (*mqttConnectionEstablishedfxn)(char *,char *,char *,char *,char *);
typedef uint16_t (*subscribefxn)(char * ,int );
typedef uint16_t (*publishfxn)(char *,char * ,unsigned int ,char );
typedef uint16_t (*powerOnModulefxn)(void);
typedef uint8_t  (*powerOffModulefxn)(void);
typedef uint16_t (*unsolictedResponsefxn)(void);
typedef uint16_t (*ConnectionInfofxn)(void);
typedef uint16_t (*httpfxn)(int8_t *,int8_t *,uint16_t ,int8_t ,int8_t *,uint16_t (*)(int8_t*,uint16_t));
typedef uint16_t (*httpinitfxn)(void);
typedef uint16_t (*httpsfxn)(int8_t *,int8_t *,uint16_t ,int8_t ,int8_t *,uint16_t (*)(int8_t*,uint16_t));
typedef uint16_t (*httpsinitfxn)(void);
typedef uint16_t (*mqttsinitfxn)(void);
typedef uint16_t (*mqttsfxn)(int8_t *,uint16_t ,int8_t *,uint8_t );
typedef uint16_t (*mqttfxn)(int8_t *,uint16_t ,int8_t *,uint8_t );
typedef void     (*mqttCredentialsetupfxn)(int8_t *,int8_t *,int8_t *,int8_t *,int8_t *,int8_t *,int8_t *);


typedef uint16_t (*restartInternetfxn)(void);
typedef uint16_t (*resatrtConnectivityModulefxn)(void);

typedef struct {

	setupfxn        				setup;
	checkNetworkfxn 				checkNetwork;
	mqttConnectionEstablishedfxn 	mqttConnectionEstablished;
	subscribefxn 					subscribe;
	publishfxn 						publish;
	powerOnModulefxn 				powerOnModule;
	powerOffModulefxn 				powerOffModule;
	unsolictedResponsefxn 			unsolictedResponse;
	httpinitfxn 					httpinit;
	httpfxn                         http;
	httpsinitfxn                    httpsinit;
	httpsfxn                        https;
	mqttsinitfxn                    mqttsinit;
	mqttsfxn                        mqtts;
	mqttfxn                         mqtt;
	mqttCredentialsetupfxn          mqttCredentialsetup;

	ConnectionInfofxn 				ConnectionInfo;
	resatrtConnectivityModulefxn    resatrtConnectivityModule;
	restartInternetfxn              restartInternet;
	char *							unsolictedResponseFlag;
	struct cell *					connectivityFlags;

} __attribute__((packed))connModuleFxn;



typedef enum
{
   getModuleidea,
   getmodulestartcell,
   getmodulecellready,
   getmoduleSetupCellular,
   getmodleDetection,
}getm;

typedef enum
{
    sr,      // setup request
    pr,      // power request
    subover, // subover request
    wo,      // watchdogoccured request,
    spr,     // software power request
    str,     // setting request,
    hb,      // heartbeat

}configcallstatuskeys;

/*======================================================================================================================================================================
   Variable declaration
======================================================================================================================================================================*/

connModuleFxn *connectivityModule;

int8_t * HttpResponsBuffer;
uint16_t HttpResponsBufferLen;
uint16_t  dataCallPayloadLen;


extern int8_t MQTT_IOT_HUB_URL[50];
extern int8_t MQTT_IOT_HUB_SAS_TOKEN[100];
extern int8_t MQTT_IOT_HUB_PUB_TOPIC[100];
extern int8_t MQTT_IOT_HUB_SUB_TOPIC[100];
extern int8_t MQTT_IOT_HUB_USERNAME[100];
extern int8_t MQTT_IOT_HUB_PASSWORD[150];


extern char configCallStatus;



/*======================================================================================================================================================================
 function declaration
======================================================================================================================================================================*/
/*  MAIN Function
 *  ==============
 */
/**************************************************************************************
 * Description:
 * ============
 *  Initialize the connectivity module either is cellular or wifi.Setup the peripheral
 *  and variable to initiate state of related to connectivity module.
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
void ConfigureModule();

/**************************************************************************************
 * Description:
 * ============
 *  This Select connectivity module depend situation or logic. There are two Internet
 *  connectivity module approach, WIFI and cellular. WIFI has high preference than
 *  cellular. Whenever wifi get Internet or available,its switched to wifi and vice versa
 *  when no network with wifi.
 *  Once module selected,its configure and get network and attached IP address,return from
 *  function.
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
unsigned int getModule();

/**************************************************************************************
 * Description:
 * ============
 *  Configuration call is HTTP or HTTPS call. It handle provision of device,subscription
 *  of device, device status updatation.
 *  Call form packet with some connectivity module info like signal strength,network name,
 *  module id,firmware and other information.Send to configuration cloud and process respective
 *  setting or response.
 *  IOT APP call this function on every power restart,on heartbeat sync time,and when device
 *  need setting.
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
unsigned int configCall();

/**************************************************************************************
 * Description:
 * ============
 *  Data call is MQTT/s or HTTP/S call. Its handle application data transmission
 *  to customer cloud that received on provisioning one by one packet wise.
 *  It fetch the packet from application data sector of flash memory and erase it once
 *  uploaded.Data call take care everything regarding connection establishment.
 *  Its will call till all application data send to cloud.
 *
 * INPUT:
 * ======
 * >
 *
 * RETURN:
 * =======
 * >
 *
 *
 *************************************************************************************/
uint16_t dataCall();

/**************************************************************************************
 * Description:
 * ============
 * Its process the response receive on config call request. This will have setting or
 * other information
 *
 * INPUT:
 * ======
 * >
 *
 * RETURN:
 * =======
 * >
 *
 *
 *************************************************************************************/
uint16_t cloudResponse(int8_t* buf,uint16_t buflen);

/**************************************************************************************
 * Description:
 * ============
 *  This check application Data is present or not.
 *
 * INPUT:
 * ======
 * >
 *
 * RETURN:
 * =======
 * >
 *
 *
 *************************************************************************************/
uint16_t isDataAvailable();

/**************************************************************************************
 * Description:
 * ============
 *  Restart Internet connection the connectivity module
 *
 * INPUT:
 * ======
 * >
 *
 * RETURN:
 * =======
 * >
 *
 *
 *************************************************************************************/
uint16_t restartInternet();

/**************************************************************************************
 * Description:
 * ============
 *  Restart  the connectivity module
 *
 * INPUT:
 * ======
 * >
 *
 * RETURN:
 * =======
 * >
 *
 *
 *************************************************************************************/
uint16_t restartModule();

/**************************************************************************************
 * Description:
 * ============
 *  Clear all flag of selected connectivity module variable and flags.
 *
 * INPUT:
 * ======
 * >
 *
 * RETURN:
 * =======
 * >
 *
 *
 *************************************************************************************/
void clearAllConnectivityflag();


void ftoa(float , char* , int );
#endif /* MAIN_IOTAPP_IOTCOMMAN_H_ */

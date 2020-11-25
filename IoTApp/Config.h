/***************************************************************************************
 * Filename    : Config.h
 * Created on  : Feb 11, 2020
 * Author      : Gaurav Choubey
 ****************************************************************************************/
#ifndef IOTAPP_CONFIG_H_
#define IOTAPP_CONFIG_H_

/****************************************************************************************
 * HEADER FILE
 ***************************************************************************************/
#include "iotComman.h"
#include "Application/Application.h"


/*======================================================================================================================================================================
  Project Version Information
  ===========================
  Give the Project version and has to update with time.
  Format is like a [ (application name) + (Soc) + (ConnectivityModule) + (Firmware Version) + (Date of Released)].
   ISM5969S800V1.0_190820    =>  IOT SDK(IS)     + MSP430fr5969(M5969) + V1.0.0 + 190820
======================================================================================================================================================================*/


#define Project_Firmware_Version       "ISM5969S800V1.0_190820"



/*======================================================================================================================================================================
  DEBUG
  ======
  Configure and Enable/Disable DEBUG console.
 # Debug_Enable
======================================================================================================================================================================*/

#define  Debug_Enable

#ifdef   Debug_Enable
#define  debugLog                 debugUartTx
#endif


/*======================================================================================================================================================================
  DELAY(system Based)
  ===================
======================================================================================================================================================================*/
//__inline delaySystem(uint16_t msec){; }

#define delaySystem(msec)     __delay_cycles((msec * 16000))


/*======================================================================================================================================================================
  RESET(system Based)
  ===================
======================================================================================================================================================================*/
#define resetSystem()                    PMM_trigBOR()




/*======================================================================================================================================================================
   Selection of Connectivity Module:
   Two Module are available so define with respect to work.
   # CELLULAR_MODULE
   # WIFI_MODULE(Coming soon)
======================================================================================================================================================================*/
#define CELLULAR_MODULE



/*=======================================================================================================================================================================
 HARDCODE VALUE
 ===============
 Define any value which want to hardcode in code
 User can any macro and related definition here.
 If define any below macro,dont forget about definition
 in Important Definition section.

 # HARDCODED_DEV_ID
 # HARDCODED_MQTT_CREDENTIAL
 # HARDCODED_MQTT_DEV_ID
 # OW_MQTT_CLOUD
 # THINGSBOARD
 # HIVEMQ
 # CONFIG_CALL_ENABLE
 # CONFIG_TIME_HARDCODED
 # SLEEP_MODE_ENABLE

=======================================================================================================================================================================*/
#define CONFIG_CALL_ENABLE
#define OW_MQTT_CLOUD
#define SLEEP_MODE_ENABLE
#define HARDCODED_DEV_ID


/*======================================================================================================================================================================
   Selection of Connection Call Type:
   Two Call are available so define with respect to work.
   Both call can be connected using follow network protocols
   1. HTTP
   2. HTTPS,
   3. MQTT,
   4. MQTTS,
   5. TCP,
   6. SSL

Note:
   #DATA_CALL_TYPE
       Data call can be any above protocols based
   #CONFIG_CALL_TYPE
       Config call must be http or https based otherwise code will crashed
======================================================================================================================================================================*/
#define  HTTP   0x01
#define  HTTPS  0x02
#define  MQTT   0x03
#define  MQTTS  0x04
#define  TCP    0x05
#define  SSL    0x06


#define DATA_CALL_TYPE      MQTT

#define CONFIG_CALL_TYPE    HTTPS


/*======================================================================================================================================================================
   Selection of Mode of protocol Stack:
   Protocols from cellular module can be used in three different
   way

   a.over AT command.
   b.over TCP protocols
   c.over PPP protocols

   1.FOR HTTP STACK
   # HTTP_OVER_AT
   # HTTP_OVER_TCP
   # HTTP_OVER_PPP

   2.FOR HTTPS STACK
   # HTTPS_OVER_AT
   # HTTPS_OVER_SSL
   # HTTPS_OVER_PPP

   3.FOR MQTT STACK
   # MQTT_OVER_AT
   # MQTT_OVER_TCP
   # MQTT_OVER_PPP

   4.FOR MQTTS STACK
   # MQTTS_OVER_AT
   # MQTTS_OVER_SSL
   # MQTTS_OVER_PPP

   Note: If any security level command used from modem,don't
   forget to used proper ssl/tls related command and certificate
   to upload before start operation
======================================================================================================================================================================*/
#define HTTPS_OVER_AT
#define MQTT_OVER_TCP
/*======================================================================================================================================================================
   Selection of Stack of protocol:
   Server protocol are available which is used to do
   communication with server.so define the only require
   Stack to avoid extra access memory usage in code.

   # TCP_ENABLE
   # SSL_TLS_ENABLE
   # HTTP_ENABLE
   # HTTPS_ENABLE
   # MQTT_ENABLE
   # MQTTS_ENABLE
=====================================================================================================================================================================*/
//Config call section
//--------------------
#if     CONFIG_CALL_TYPE == HTTP
#ifndef  HTTP_ENABLE
#define HTTP_ENABLE
#endif

#ifdef  HTTP_OVER_TCP
#define TCP_ENABLE

#endif

#elif   CONFIG_CALL_TYPE == HTTPS
#ifndef  HTTPS_ENABLE
#define HTTPS_ENABLE
#endif

#ifdef  HTTPS_OVER_SSL
#define SSL_TLS_ENABLE

#endif

#elif CONFIG_CALL_TYPE == MQTT
#ifndef  MQTT_ENABLE
#define MQTT_ENABLE
#endif

#ifdef  MQTT_OVER_TCP
#define TCP_ENABLE
#endif


#elif CONFIG_CALL_TYPE == MQTTS

#ifndef  MQTTS_ENABLE
#define MQTTS_ENABLE
#endif

#ifdef  MQTTS_OVER_SSL
#define SSL_TLS_ENABLE
#endif

#elif CONFIG_CALL_TYPE == TCP
#define TCP_ENABLE

#elif CONFIG_CALL_TYPE == SSL
#define SSL_TLS_ENABLE
#endif


//Data call section
//--------------------

#if     DATA_CALL_TYPE == HTTP
#ifndef  HTTP_ENABLE
#define HTTP_ENABLE
#endif

#ifdef  HTTP_OVER_TCP
#define TCP_ENABLE
#endif

#elif   DATA_CALL_TYPE == HTTPS
#ifndef  HTTPS_ENABLE
#define HTTPS_ENABLE
#endif

#ifdef  HTTPS_OVER_SSL
#define SSL_TLS_ENABLE
#endif

#elif DATA_CALL_TYPE == MQTT
#ifndef  MQTT_ENABLE
#define MQTT_ENABLE
#endif

#ifdef  MQTT_OVER_TCP
#define TCP_ENABLE
#endif


#elif DATA_CALL_TYPE == MQTTS

#ifndef  MQTTS_ENABLE
#define MQTTS_ENABLE
#endif

#ifdef  MQTTS_OVER_SSL
#define SSL_TLS_ENABLE
#endif

#elif DATA_CALL_TYPE == TCP
#define TCP_ENABLE

#elif DATA_CALL_TYPE == SSL
#define SSL_TLS_ENABLE
#endif

/*=======================================================================================================================================================================
 Important Definition
 ======================
 Define Configcall and DataCall URL, Any Hard coded value

 # CONFIG_URL
 # DATA_URL
=======================================================================================================================================================================*/
//CONFIG_URL
//==========

#ifdef  CONFIG_CALL_ENABLE
#define CONFIG_URL                   	"https://cloud.gciot.tk/api/v1/"
#define CONFIG_URL_PATH                 "/rpc"
#define CONFIG_USERNAME                 ""
#define CONFIG_PASSWORD                 ""
#define CONFIG_AUTH_KEY                 ""

#else
#define HARDCODED_MQTT_CREDENTIAL
#endif

//====================================================================================================================================

//DATA_URL
//=========
#ifdef  HARDCODED_DEV_ID
#define DEV_ID                          "Device 1"
#endif

#if ((DATA_CALL_TYPE == MQTT) ||(DATA_CALL_TYPE == MQTTS))


#ifdef HARDCODED_MQTT_CREDENTIAL
//HiveMQ MQTT Broker
//===================
#ifdef  HIVEMQ
#define MQTT_URL              			"broker.hivemq.com"
#define MQTT_PORT             			"1883"
#define MQTT_USERNAME          			NULL
#define MQTT_PASS              			NULL
#define MQTT_PUBLISH_TOPIC    			"iBotSend"
#define MQTT_SUBSCRIBE_TOPIC  			"iBotRecv"
#endif


//ThingsBoard MQTT Broker
//=======================
#ifdef  THINGSBOARD
#define MQTT_URL              			"cloud.thingsboard.io"
#define MQTT_PORT             			"1883"
#define MQTT_USERNAME         			"autovolt"
#define MQTT_PASS              			NULL
#define MQTT_PUBLISH_TOPIC    			"v1/devices/me/telemetry"
#define MQTT_SUBSCRIBE_TOPIC  			"v1/devices/me/attributes"
#endif

// OW MQTT Cloud CREADENTIAL
//==========================
#endif

#ifdef OW_MQTT_CLOUD
#define MQTT_ID                         DEVICE_ID
#define MQTT_URL                        MQTT_IOT_HUB_URL
#define MQTT_SAS_TOKEN                  MQTT_IOT_HUB_SAS_TOKEN
#define MQTT_API_VERSION                "2018-06-30"
#define MQTT_PORT                       "1883"
#define MQTT_USERNAME                   MQTT_IOT_HUB_USERNAME
#define MQTT_PASS                       MQTT_IOT_HUB_PASSWORD
#define MQTT_PUBLISH_TOPIC              MQTT_IOT_HUB_PUB_TOPIC
#define MQTT_SUBSCRIBE_TOPIC            MQTT_IOT_HUB_SUB_TOPIC

#endif
#endif

#if ((DATA_CALL_TYPE == HTTP) ||(DATA_CALL_TYPE == HTTPS))

#define HTTP_S_URL
#define HTTP_S_PORT
#define HTTP_S_USERNAME
#define HTTP_S_PASS
#define HTTP_S_PUBLISH_TOPIC
#define HTTP_S_SUBSCRIBE_TOPIC

#endif

#if ((DATA_CALL_TYPE == TCP) ||(DATA_CALL_TYPE == SSL))

#define TCP_SSL_URL
#define TCP_SSL_PORT
#define TCP_SSL_USERNAME
#define TCP_SSL_PASS
#define TCP_SSL_PUBLISH_TOPIC
#define TCP_SSL_SUBSCRIBE_TOPIC


#endif



/**************************************************************************************************************************
 *                             RESTRICTED AREA: DON'T CHANGED ANYTHING FURTHER DOWN
 *************************************************************************************************************************/


/*======================================================================================================================================================================
   TCP or SSL/TLS Credential:
   Define the TCP or SSL/TLS Url,Port,Username,and Password
   and other credential required to configure
======================================================================================================================================================================*/
#if ((DATA_CALL_TYPE == TCP) || (DATA_CALL_TYPE == SSL))

#define URL							TCP_SSL_URL
#define PORT						TCP_SSL_PORT
#define USERNAME					TCP_SSL_USERNAME
#define PASSWORD					TCP_SSL_PASS
#define PB_TOPIC					TCP_SSL_PUBLISH_TOPIC
#define SB_TOPIC					TCP_SSL_SUBSCRIBE_TOPIC

#endif
/*======================================================================================================================================================================
   MQTT or MQTTS Credential:
   Define the TCP url
======================================================================================================================================================================*/
#if ((DATA_CALL_TYPE == MQTT) || (DATA_CALL_TYPE == MQTTS))

#define ID                          MQTT_ID
#define URL                         MQTT_URL
#define PORT						MQTT_PORT
#define USERNAME					MQTT_USERNAME
#define PASSWORD					MQTT_PASS
#define PB_TOPIC					MQTT_PUBLISH_TOPIC
#define SB_TOPIC					MQTT_SUBSCRIBE_TOPIC

#endif

#if ((DATA_CALL_TYPE == HTTP) || (DATA_CALL_TYPE == HTTPS))

#define URL                         HTTP_S_URL
#define PORT                        HTTP_S_PORT
#define USERNAME                    HTTP_S_USERNAME
#define PASSWORD                    HTTP_S_PASS
#define PB_TOPIC                    HTTP_S_PUBLISH_TOPIC
#define SB_TOPIC                    HTTP_S_SUBSCRIBE_TOPIC

#endif


#endif /* MAIN_IOTAPP_CONFIG_H_ */

/***************************************************************************************
 * Filename    : cellularModuleLLSim800.h
 * Created on  : Feb 11, 2020
 * Author      : Gaurav Choubey
 ****************************************************************************************/
#ifndef MAIN_CELLULARMODULE_CORECELLULARMODULE_CELLULARMODULESIM800LL_H_
#define MAIN_CELLULARMODULE_CORECELLULARMODULE_CELLULARMODULESIM800LL_H_


/****************************************************************************************
 * HEADER FILE
 ***************************************************************************************/
#include "../cellularModuleConfig.h"
//#include "../cellularModuleHeader.h"
#include <stdint.h>





/****************************************************************************************
 * VARIABLE
 ***************************************************************************************/
#define     cellularBufferSize 1024
extern char cellularBuffer[cellularBufferSize];

extern volatile uint16_t 	celMBufferIndex;
extern bool   				celMATCmd ;
extern bool   				celMBufReady;
extern char  	            celMUsolicRes ;
extern uint16_t 			cellMHttpAction;
extern struct cell          cellularflags;


typedef enum
{
   cellularModemReady = 0x200,
   cellularModemnotReady,
   cellularModemBusy,
   cellularModemRestartSuccess,
   celMSimCardAbsent,
   celMATMaxOccured,
   cellMTCPStackNotOpen,
   cellMTcpOpen,
   cellMTcpClose,
   cellMTcpAlreadyConnected,
   cellMTcpConnected,
   cellMTcpDisconnected,
   cellMTcpNotConnected,
   cellMTcpInitial,
   cellMTcpConnectionProcessing,
   cellMTcpSendSuccess,
   cellMTcpSendfailed,
   cellMTimeClockdisable,
   cellMGPSPowerOn,
   cellMGPSPowerOFF,
   cellMSimcardAbsent,
   cellMNetworkSearching,
   cellMturnoffecho,
   cellMGprsattached,
   cellMGprsdeattached,
   cellMPingOut,
   cellMTcpConnectionClosed,
   cellMTcpConnectionfailed,
   cellMNoIpAddress,
   cellMHttpTimeoutOccured,


   cellMSslStartFailed,
   cellMSslCertnotfound,
   cellMrestartEverything,

   cellMHttpAddrAvailable,

   cellularModuleInitSUCCESS,
   cellularCheckNetworkSUCCESS,
   celullarHttpInitSUCCESS,
   ConnectionInfoSUCCESS,
   cellularTcpInitSUCCESS,

   ccellularSslTlsInitSuccess,

}cellmodumReturn;





typedef enum
{
//GSM Cmd for initialisation of GSMmodem
 Init_GSM_CMD=0,
 AT_RESTORE_IQ  ,
 AT_ATTENTION_COMMAND    ,
 AT_DISABLE_VERBOSE      ,
 AT_TURN_OFF_ECHO       ,
 AT_AUTO_BAUD_ENABLE        ,
 AT_REJECT_INCOMING_CALLS   ,
 AT_CHECK_AUTO_TIMESTAMP_ENABLE  ,
 AT_ENABLE_TIMESTAMP_AUTO_UPDATE,
 AT_CLOCK            ,
 AT_REQUEST_IMSI       ,
 AT_REQUEST_IMEI,
 AT_SIM_NUMBER       ,
 AT_SIM_PRESENCE_CMD      ,
 AT_SET_APN,
 AT_SET_DEFAULT_CONTEXT,
 AT_SAVE           ,
 AT_MODEM_FW_VERSION,

 //GSM Cmd for check for network

  AT_CHECK_NETWORK       ,
  AT_CHECK_PACKET_NETWORK,
  AT_CHECK_GPRS_ATTACHED,
  AT_CHECK_GPRS_ACTIVATED,
  AT_ATTACHED_GPRS,
  AT_ACTIVATE_GPRS,
  AT_SIGNAL_STRENGTH,
  AT_GET_IP_ADDRESS,

 //GSM CMD FOR initialisation of tcp
  AT_TCP_SHUT                   ,
  AT_TCP_ENABLE_MULTIPLE          ,
  AT_TCP_SET_APN                 ,
  AT_TCP_BRINGUP_GPRS           ,
  AT_TCP_GET_LOCAL_IP           ,
  AT_TCP_OPEN_SCOKET      ,
  AT_TCP_START_CONNECTION_PLUS    ,
  AT_TCP_SEND_DATA                ,
  AT_TCP_CLOSE_CONNECTION           ,
  AT_TCP_CONNECTION_STATUS                     ,
  AT_TCP_SEND_DATA_PLUS             ,
  AT_TCP_SHOW_RECV_ONRECEPTION,
  AT_TCP_ATTACHED_IP,
  AT_TCP_CFG_PARA,
  AT_TCP_STACK_OPEN_Q,
  AT_TCP_STACK_OPEN,
  AT_TCP_STACK_CLOSE,

  //SSL/TLS cmd
  AT_SSLTLS_CFG,
  AT_SSLTLS_CCHSET,
  AT_SSTLS_SETCONTEXT,
  AT_SSTLS_SETCONTEXT_Q,
  AT_SSLTLS_OPEN_STACK,
  AT_SSLTLS_CLOSE_STACK,
  AT_SSLTLS_OPEN_SOCKET,
  AT_SSLTLS_CLOSE_SOCKET,
  AT_SSLTLS_GET_ADRESS,
  AT_SSLTLS_SEND,
  AT_SSLTLS_SEND_EXTRA,
  AT_SSLTLS_CERT_DOWNLOAD,
  AT_SSLTLS_CERTLIST,



 //GSM Cmd for intialisation of http
  AT_HTTP_SET_BEARER_SETTINGS        ,
  AT_HTTP_BEARER_APN_SETTINGS      ,
  AT_HTTP_GET_LOCAL_IP                ,
  AT_HTTP_BRINGUP_GPRS             ,
  AT_HTTP_ENABLE_SSL                ,
  AT_HTTP_START                    ,
  AT_HTTP_STOP  ,
  AT_HTTP_ACTION                   ,
  AT_HTTP_READ                   ,
  AT_HTTP_DISABLE_SSL               ,
  AT_HTTP_CID_ENABLE                ,
  AT_HTTP_PARRA                ,
  AT_HTTP_POST_DATA,
  AT_HTTPSSL,

 //other cmd need in GSm
  AT_GET_CELLULAR_LOCATION               ,
  AT_GET_MOBILE_OPERATOR_NAME      ,
  AT_GET_BATTERY_PARAMETERS        ,
  AT_CHECK_STK_SWITCH              ,
  AT_TURN_ON_STK                  ,
  AT_SET_OPERATOR                 ,
  AT_POWER_OFF                      ,
  AT_GET_NW_TYPE               ,
  AT_SET_NW_TYPE               ,
  AT_PING_MODEM,


 //gps related cmd
  AT_GPS_CHECK_POWERED    ,
  AT_GPS_POWER,
  AT_GPS_GET_LOCATION,
  AT_GPS_SET_PARA,
  AT_GPS_POWER_AUTO,

}CELLULAR_CMD_NUMS;

struct st
{
	char imei[16];
	char imsi[16];
	char csq[3];
	char ccid[25];
	char FWversion[25];
	char cops[30];
	char cbc[10];
    char operattype[15];
    char celllocation[20];
} __attribute__((packed));

extern struct st cellularData;

#define cellMinteral   0
#define cellMexternal  1
#define cellMNoSpace   2

//type of network
#define NW_AUTO                     "2"
#define NW_GSM                      "13"
#define NW_WCDMA                    "14"
#define NW_LTE                      "38"
#define NW_TDS_CDMA                 "59"
#define NW_CDMA                     "9"
#define NW_EVDO                     "10"
#define NW_GSM_WCDMA                "19"
#define NW_CDMA_EVDO                "22"
#define NW_LTE_ANY_MODE             "48"
#define NW_GSM_TDSCDMA              "60"
#define NW_GSM_WCDMA_TDSCDMA        "63"
#define NW_CDMA_EVDO_GSM_WCDMA_TDSCDMA   "67"
#define NW_GSM_WDMA_LTE             "39"
#define NW_WDMA_LTE                 "54"

//SSL-TLS Auth mode
#define    sslTlsAuth_NoCert              1
#define    sslTlsAuth_SererCert           2
#define    sslTlsAuth_ClientCert          3
#define    sslTlsAuth_Server_ClientCert   4

//http stack
#define CELLULAR_HTTP_STACK_OVER_TCP      5
#define CELLULAR_HTTP_STACK_OVER_AT		  6


//mqtt stack
#define CELLULAR_MQTT_STACK_OVER_TCP      7
#define CELLULAR_MQTT_STACK_OVER_AT       8

typedef enum
{
    TCP_MODE,
    UDP_MODE,
    SSL_MODE,

}MODEOFCONNECTION;

/****************************************************************************************
* FUNCTION
***************************************************************************************/

/**************************************************************************************
 * Description:
 * ============
 *  Send the AT command to cellular module over UART.Its low level communication with
 *  cellular Module.Function return on first response received regardless of expected
 *  output.
 *
 * INPUT:
 * ======
 * > cmd    : Its command in string format.
 * > cmd_num: Number from structure CELLULAR_CMD_NUMS.
 * > timeout: Maximum time to wait for cellular module response.
 * > retry  : Number of retry in case of failed.
 *
 * RETURN:
 * =======
 * >
 *
 *
 *************************************************************************************/
uint16_t celMSendATCmd(char *cmd,uint8_t cmd_num,uint8_t timeout,uint8_t retry);

/**************************************************************************************
 * Description:
 * ============
 *  Send the AT command to cellular module over UART.Its low level communication with
 *  cellular Module.Function do error checking till respective response received or timeout
 *  occurred.
 *
 * INPUT:
 * ======
 * > cmd    : Its command in string format.
 * > cmd_num: Number from structure CELLULAR_CMD_NUMS.
 * > timeout: Maximum time to wait for cellular module response.
 * > retry  : Number of retry in case of failed.
 *
 * RETURN:
 * =======
 * >
 *
 *
 *************************************************************************************/
uint16_t celMSendATCmdWait(char *cmd,uint8_t cmd_num,uint16_t timeout,int8_t retry);

/**************************************************************************************
 * Description:
 * ============
 *  Configure the cellular module such echo off,setting baud rate,get IMEI and CCID
 *  number and save configuration.
 *
 * INPUT:
 * ======
 * > No argument
 *
 * RETURN:
 * =======
 * > cellularModuleInitSUCCESS - Function executed Successfully.
 * > celMSimCardAbsent         - SIMCARD not present or not DETECTED.
 *                               Device will restart.
 * > celMATMaxOccured          - Maximum attempt of command occurred and some command
 *                               does not responding.
 *
 *
 *************************************************************************************/
uint16_t cellularModuleInit();


/**************************************************************************************
 * Description:
 * ============
 *  Search and Get the cellular network, attached packet service and
 *  get assign the IP address.
 *
 * INPUT:
 * ======
 * > No argument
 *
 * RETURN:
 * =======
 * > cellularCheckNetworkSUCCESS - Function executed Successfully.
 * > cellMNetSearching         - cellular module still searching for network.
 * > celMATMaxOccured          - Maximum attempt of command occurred and some command
 *                               does not responding.
 *
 *
 *************************************************************************************/
uint16_t cellularCheckNetwork();

/**************************************************************************************
 * Description:
 * ============
 *  Give and stored the cellular module related information like signal strength,
 *  operator name,operator type into cellularData structure.
 *
 * INPUT:
 * ======
 * > No argument
 *
 * RETURN:
 * =======
 * > ConnectionInfoSUCCESS - Function executed Successfully.
 * > cellMrestartEverything- Maximum attempt of command occurred and some command
 *                           does not responding.
 *
 *
 *************************************************************************************/
uint16_t cellularConnectionInfo();

/**************************************************************************************
 * Description:
 * ============
 *  Check and validate the given command response and return appropriate response.
 *
 * INPUT:
 * ======
 * > cmd : command number from structure CELLULAR_CMD_NUMS.
 *
 * RETURN:
 * =======
 * > depend on given command.
 *
 *
 *************************************************************************************/
uint16_t celMATCmdResponse(uint8_t cmd);

#ifdef HTTP_ENABLE

/**************************************************************************************
 * Description:
 * ============
 *  Initialize the HTTP stack of cellular module by sending related at command
 *  as mentioned in document. Also attached gprs connectivity to HTTP stack.
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > depend on given command.
 *
 *
 *************************************************************************************/
uint16_t celullarHttpInit();

/**************************************************************************************
 * Description:
 * ============
 *  Start HTTP stack of cellular module by sending related at command
 *  as mentioned in document.
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > depend on given command.
 *
 *
 *************************************************************************************/
uint16_t celullarHttpStart();

/**************************************************************************************
 * Description:
 * ============
 *  stop HTTP stack of cellular module by sending related at command
 *  as mentioned in document.
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > depend on given command.
 *
 *
 *************************************************************************************/
uint16_t celullarHttpStop();

/**************************************************************************************
 * Description:
 * ============
 *  Send the Data over HTTP on given url.Please Initiate and Start http stack before
 *  used this function.
 *
 * INPUT:
 * ======
 * > socketId - socket number to connect with server
 * > url      - server url or ip address to connect
 * > payload  - Data(actual data) to be send
 * > payload_len -  length of data
 * > method     - HTTP method(GET/POST/PUT/HEAD)
 *
 * RETURN:
 * =======
 * > cellMHttpTimeoutOccured  - No HTTP response receive within time of 60 second.
 * > cellMrestartEverything   - Maximum attempt of command occurred and some command
 *                               does not responding.
 * > cellMTcpConnectionClosed - Socket connection closed
 * > HTTP_300,HTTP_400,HTTP_500,HTTP_600,HTTP_700,HTTP_200,HTTP_201,HTTP_200_OK - HTTP response code
 *
 *
 *
 *************************************************************************************/
uint16_t celullarHttpSend(int8_t socketId,int8_t *url,int8_t *payload,uint16_t payload_len,int8_t method);

/**************************************************************************************
 * Description:
 * ============
 *  Read the HTTP response payload.
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > Http_badrequest  - Badrequest received in payload
 * > Http_no_response - no payload received
 * > Success          - function is successful.
 * > failed           - Error and command not responding
 *
 *************************************************************************************/
uint16_t celullarHttpRead();
#endif


#ifdef HTTPS_ENABLE
/**************************************************************************************
 * Description:
 * ============
 *  Initialize the HTTP stack of cellular module by sending related at command
 *  as mentioned in document.
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > depend on given command.
 *
 *
 *************************************************************************************/
uint16_t celullarHttpsInit();

/**************************************************************************************
 * Description:
 * ============
 *  Start HTTP stack of cellular module by sending related at command
 *  as mentioned in document.
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > depend on given command.
 *
 *
 *************************************************************************************/
uint16_t celullarHttpsStart();

/**************************************************************************************
 * Description:
 * ============
 *  stop HTTP stack of cellular module by sending related at command
 *  as mentioned in document.
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > depend on given command.
 *
 *
 *************************************************************************************/
uint16_t celullarHttpsStop();

/**************************************************************************************
 * Description:
 * ============
 *  Send the Data over HTTP on given url.Please Initiate and Start http stack before
 *  used this function.
 *
 * INPUT:
 * ======
 * > socketId - socket number to connect with server
 * > url      - server url or ip address to connect
 * > payload  - Data(actual data) to be send
 * > payload_len -  length of data
 * > method     - HTTP method(GET/POST/PUT/HEAD)
 *
 * RETURN:
 * =======
 * > cellMHttpTimeoutOccured  - No HTTP response receive within time of 60 second.
 * > cellMrestartEverything   - Maximum attempt of command occurred and some command
 *                               does not responding.
 * > cellMTcpConnectionClosed - Socket connection closed
 * > HTTP_300,HTTP_400,HTTP_500,HTTP_600,HTTP_700,HTTP_200,HTTP_201,HTTP_200_OK - HTTP response code
 *
 *
 *
 *************************************************************************************/
uint16_t celullarHttpsSend(int8_t socketId,int8_t *url,int8_t *payload,uint16_t payload_len,int8_t method);

/**************************************************************************************
 * Description:
 * ============
 *  Read the HTTP response payload.
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > Http_badrequest  - Badrequest received in payload
 * > Http_no_response - no payload received
 * > Success          - function is successful.
 * > failed           - Error and command not responding
 *
 *************************************************************************************/
uint16_t celullarHttpsRead();

#endif

#ifdef SSL_TLS_ENABLE
/**************************************************************************************
 * Description:
 * ============
 *  Initialize the ssl stack of cellular module using mentioned command in document.
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > ccellularSslTlsInitSuccess  - function executed successfully
 * > cellMrestartEverything - Maximum attempt of command occurred and some command
 *                               does not responding.

 *
 *************************************************************************************/

uint16_t cellularSslTlsInit();

#endif

#ifdef TCP_ENABLE
/**************************************************************************************
 * Description:
 * ============
 *  Initialize the TCP stack of cellular module using mentioned command in document.
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > cellularTcpInitSUCCESS  - function executed successfully
 * > cellMrestartEverything  - Maximum attempt of command occurred and some command
 *                             does not responding.

 *
 *************************************************************************************/

uint16_t cellularTcpInit();
#endif

/**************************************************************************************
 * Description:
 * ============
 *  Check the status of given socket number
 *
 * INPUT:
 * ======
 * > socketid         - socket number of connection.
 *                      0-4
 * > Connection_Mode  - Type of Communication
 *                      TCP_MODE,SSL_MODE,UDP_MODE from structure MODEOFCONNECTION.
 *
 * RETURN:
 * =======
 * > cellMTcpClose                - Socket is closed
 * > cellMTcpAlreadyConnected     - socket is already connected
 * > cellMTcpConnectionProcessing - socket is processing. Wait for some time and try again.
 * > cellMTcpInitial              - socket is ready to configure or get connected.
 * > cellMrestartEverything       - Maximum attempt of command occurred and some command
 *                                  does not responding.
 *

 *
 *************************************************************************************/
uint16_t socketConnectionStatus(int8_t socket_id,uint16_t Connection_Mode);

/**************************************************************************************
 * Description:
 * ============
 *  Open the socket connection base on type of connection.
 *
 * INPUT:
 * ======
 * > socketid         - socket number of connection.
 *                      0-4
 * > Connection_Mode  - Type of Communication.
 *                      TCP_MODE,SSL_MODE,UDP_MODE from structure MODEOFCONNECTION.
 *
 * > Url              - server url or ip address to be connect in string format.
 * > Port             - port number to connect in string format.
 *                      0 to 65535
 *
 * RETURN:
 * =======
 * > cellMTcpConnected         - Socket succesfully Open.
 * > cellMTcpConnectionClosed  - Socket already closed.Please Reestablished the connection.
 *                               does not responding.
 * > cellMTcpConnectionfailed  - Socket unable to open. Please check Internet connection as well
 *                               as network.

 *
 *************************************************************************************/
uint16_t socketConnectionOpen(int8_t Socket_id,int16_t Connection_Mode,const int8_t *Url,const int8_t *Port);

/**************************************************************************************
 * Description:
 * ============
 *  Send the given payload on given socket id with respective type of connection.
 *
 * INPUT:
 * ======
 * > socketid         - socket number of connection.
 *                      0-4
 * > Connection_Mode  - Type of Communication.
 *                      TCP_MODE,SSL_MODE,UDP_MODE from structure MODEOFCONNECTION.
 * > data             - Actual Payload send on open socket connection with given socket id.
 * > len              - Paylaod length.
 *
 * RETURN:
 * =======
 * > cellMTcpSendSuccess    - Data send over socket successfully
 * > cellMTcpConnectionClosed - Socket already closed.Please Reestablished the connection.
 * > cellMTcpSendfailed      - Data send failed. Either socket close or wrong connection.
 *                              Please reopen the connection and try it again.
 *
 *************************************************************************************/
uint16_t socketSend(int8_t socket,uint16_t Connection_Mode,int8_t *data,uint16_t len);

/**************************************************************************************
 * Description:
 * ============
 *  Close the give number of socket with respective mode.
 *  Mode are TCP,SSL and UDP that mean function send respective
 *  close command to cellular module.
 *
 * INPUT:
 * ======
 * > socketid         - socket number of connection.
 *                      0-4
 * > Connection_Mode  - Type of Communication.
 *                      TCP_MODE,SSL_MODE,UDP_MODE from structure MODEOFCONNECTION.
 *
 * RETURN:
 * =======
 * > cellMTcpDisconnected - socket close successfully
 * > celMATMaxOccured     - Maximum attempt of command occurred and some command
 *                          does not responding.

 *
 *************************************************************************************/
uint16_t socketClose(uint8_t socketid,uint16_t Connection_Mode);


/**************************************************************************************
 * Description:
 * ============
 *  Restart the Internet connection by sending the related AT command
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > CELL_FUNCTION_SUCCESS  - function executed successfully
 * > CELL_RESTART_MODEM  -   Maximum attempt of command occurred and some command
 *                           does not responding.Restart modem

 *
 *************************************************************************************/
uint16_t celMrestartInternetConnection();

/**************************************************************************************
 * Description:
 * ============
 *  Erase the cellular module buffer which received all cellular AT command response.
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > No return

 *
 *************************************************************************************/
void     celMClearBuffer();

/**************************************************************************************
 * Description:
 * ============
 *  Validate the return value and depend on given rval, state variable is changed
 *  which is increment or decrement state variable.
 *
 * INPUT:
 * ======
 * > state_variable - Address of State variable of particular function.
 * > rval           - Address of return variable of particular function.
 *
 * RETURN:
 * =======
 * Depend on function called.

 *
 *************************************************************************************/
void     checkReturnValue(uint8_t *state_variable,uint16_t *rval);


#endif /* MAIN_CELLULARMODULE_CORECELLULARMODULE_CELLULARMODULELL_H_ */

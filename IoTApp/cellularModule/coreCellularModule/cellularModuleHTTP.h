/***************************************************************************************
 * Filename    : cellularModuleHTTP.h
 * Created on  : Feb 11, 2020
 * Author      : Gaurav Choubey
 ****************************************************************************************/
#ifndef CELLULARMODULE_CORECELLULARMODULE_CELLULARMODULEHTTP_H_
#define CELLULARMODULE_CORECELLULARMODULE_CELLULARMODULEHTTP_H_

/****************************************************************************************
 * HEADER FILE
 ***************************************************************************************/
#include "../../Config.h"


/****************************************************************************************
 * VARIABLE
 ***************************************************************************************/

//#ifdef CELLULAR_HTTP_ENABLE
typedef enum
{
    HTTP_GET_METHOD,
    HTTP_POST_METHOD,
    HTTP_HEAD_METHOD,
    HTTP_DELET_METHOD,


}httpMethod;


typedef enum
{
    HTTP_INVALID=0xC000,
    HTTP_200            ,
    HTTP_201             ,
    HTTP_300              ,
    HTTP_400               ,
    HTTP_500                ,
    HTTP_600                 ,
    HTTP_700                  ,
    HTTP_200_OK                ,
    HTTP_SUCCESS,

    HTTP_FAILED,
    HTTP_BADREQUEST,
    HTTP_ERROR,


}HTTPRESPONSE;




uint16_t cellMHttpRecvBuffLen;
int8_t  *cellMHttpRecvBuff;
//int8_t   cellMHttpRecvLenBuff[5];

/****************************************************************************************
* FUNCTION
***************************************************************************************/
#ifdef HTTP_ENABLE
/**************************************************************************************
 * Description:
 * ============
 *  Initialize the HTTP stack and reconfigure cellular module library for http section
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > CELL_HTTP_INIT_SUCCESS  - function executed successfully
 * > CELL_CONNECTION_CLOSED  - Socket closed or network lost.Please check it again.
 * > CELL_RESTART_MODEM  -   Maximum attempt of command occurred and some command
 *                           does not responding.Restart modem

 *
 *************************************************************************************/
uint16_t cellMHttpInit();

/**************************************************************************************
 * Description:
 * ============
 *  HTTP function which request GET/POST/HEAD/PUT to given url and send payload data
 *  and once response received,send to ProcessMsgCb callback function for validation
 *  and extraction of data.
 *
 * INPUT:
 * ======
 * > url          - Server url or ip address to connect
 * > payload      - Data(actual data) to be send
 * > payload_len  - Length of data
 * > method       - HTTP method(GET/POST/PUT/HEAD)
 * > contentType  - Type of Data in string format.
 *                  application/json,text/plain.
 * > ProcessMsgCb - function pointer which point to function which validate http response
 *
 * RETURN:
 * =======
 * > HTTP_SUCCESS
 *      - Successfully on receiving of HTTP 200 which mean execute the HTTPS request
 *
 * > HTTP HTTP_BADREQUEST
 *      - HTTP 400 error receive from cloud which mean something wrong in request message.
 *      - Restart the whole socket connection and try it again.
 *
 * > HTTP_ERROR
 *      - Maximum attempt of receiving of HTTP 400 error.Device must be restart or validate
 *        data or url separately.
 *
 * > SERVER_BUSY
 *      - Maximum attempt of HTTP 500,600,700 error which mean something wrong with server,
 *        stay quit for some time and later retry again.
 *
 * This is Are the Modem related that mean its must be executed
 * > CONNECTION_CLOSED
 *     - API request for re-starting whole socket connection,something wrong is appearing
 *       socket communicaton.
 *
 * > RESTART_MODEM
 *    - API request for restarting connectivity module as its not responding.
 *
 *************************************************************************************/
uint16_t cellMHttp(int8_t *url,int8_t *payload,uint16_t payload_len,int8_t method,int8_t *contentType,uint16_t (*ProcessMsgCb)(int8_t*,uint16_t));
#endif

#ifdef HTTPS_ENABLE
/**************************************************************************************
 * Description:
 * ============
 *  Initialize the HTTP stack and reconfigure cellular module library for http section
 *
 * INPUT:
 * ======
 * > No argument.
 *
 * RETURN:
 * =======
 * > CELL_HTTP_INIT_SUCCESS  - function executed successfully
 * > CELL_CONNECTION_CLOSED  - Socket closed or network lost.Please check it again.
 * > CELL_RESTART_MODEM  -   Maximum attempt of command occurred and some command
 *                           does not responding.Restart modem

 *
 *************************************************************************************/
uint16_t cellMHttpsInit();

/**************************************************************************************
 * Description:
 * ============
 *  HTTP function which request GET/POST/HEAD/PUT to given url and send payload data
 *  and once response received,send to ProcessMsgCb callback function for validation
 *  and extraction of data.
 *
 * INPUT:
 * ======
 * > url          - Server url or ip address to connect
 * > payload      - Data(actual data) to be send
 * > payload_len  - Length of data
 * > method       - HTTP method(GET/POST/PUT/HEAD)
 * > contentType  - Type of Data in string format.
 *                  application/json,text/plain.
 * > ProcessMsgCb - function pointer which point to function which validate http response
 *
 * RETURN:
 * =======
 * > HTTP_SUCCESS
 *      - Successfully on receiving of HTTP 200 which mean execute the HTTPS request
 *
 * > HTTP HTTP_BADREQUEST
 *      - HTTP 400 error receive from cloud which mean something wrong in request message.
 *      - Restart the whole socket connection and try it again.
 *
 * > HTTP_ERROR
 *      - Maximum attempt of receiving of HTTP 400 error.Device must be restart or validate
 *        data or url separately.
 *
 * > SERVER_BUSY
 *      - Maximum attempt of HTTP 500,600,700 error which mean something wrong with server,
 *        stay quit for some time and later retry again.
 *
 * This is Are the Modem related that mean its must be executed
 * > CONNECTION_CLOSED
 *     - API request for re-starting whole socket connection,something wrong is appearing
 *       socket communicaton.
 *
 * > RESTART_MODEM
 *    - API request for restarting connectivity module as its not responding.
 *
 *************************************************************************************/

uint16_t cellMHttps(int8_t *url,int8_t *payload,uint16_t payload_len,int8_t method,int8_t *contentType,uint16_t (*ProcessMsgCb)(int8_t*,uint16_t));
#endif
//#endif
#endif /* MAIN_CELLULARMODULE_CORECELLULARMODULE_CELLULARMODULEHTTP_H_ */

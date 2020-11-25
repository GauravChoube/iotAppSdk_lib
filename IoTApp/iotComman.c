/***************************************************************************************
 * Filename    : iotComman.c
 * Created on  : May 9, 2020
 * Author      : Gaurav Choubey
 ****************************************************************************************/
/*======================================================================================================================================================================
   HEADER
======================================================================================================================================================================*/
#include <Application/batteryModule/batAdc.h>
#include "iotComman.h"
#include "iotAPP.h"


/*======================================================================================================================================================================
   VARIABLE
======================================================================================================================================================================*/


volatile char getModuleState = getModuleidea;
char cellularstartupState    = 1;
char configCallState         = 1;
char dataCallState           = 1;

#pragma PERSISTENT(configCallStatus)
char configCallStatus = pr;

connModuleFxn connectivityModuleWifi = {0};
connModuleFxn connectivityModuleCellular={0};

#pragma PERSISTENT(iotCommanBuf)
char iotCommanBuf[100]={0};

#pragma PERSISTENT(iotCommanTemp)
char iotCommanTemp[iotCommanBufSize] = {0};

#pragma PERSISTENT(configUrl)
char configUrl[50]={0};

#ifdef OW_MQTT_CLOUD
#pragma PERSISTENT(MQTT_IOT_HUB_URL)
int8_t MQTT_IOT_HUB_URL[50]={0};

#pragma PERSISTENT(MQTT_IOT_HUB_SAS_TOKEN)
int8_t MQTT_IOT_HUB_SAS_TOKEN[100]={0};

#pragma PERSISTENT(MQTT_IOT_HUB_PUB_TOPIC)
int8_t MQTT_IOT_HUB_PUB_TOPIC[100]={0};

#pragma PERSISTENT(MQTT_IOT_HUB_SUB_TOPIC)
int8_t MQTT_IOT_HUB_SUB_TOPIC[100]={0};

#pragma PERSISTENT(MQTT_IOT_HUB_USERNAME)
int8_t MQTT_IOT_HUB_USERNAME[100]={0};

#pragma PERSISTENT(MQTT_IOT_HUB_PASSWORD)
int8_t MQTT_IOT_HUB_PASSWORD[150]={0};
#endif



/*======================================================================================================================================================================
   FUNCTION
======================================================================================================================================================================*/


void ConfigureModule()
{
#ifdef wifiModuleActive

#endif
//    memset(&connectivityModule,0x00,sizeof(connModuleFxn));
    connectivityModule = 0;
    iotAppflags.moduleType = 0;

    memset(&connectivityModuleCellular,0x00,sizeof(connModuleFxn));
    memset(&connectivityModuleCellular.connectivityFlags,0x00,sizeof(cellularflags));

    connectivityModuleCellular.setup = cellularModuleInit;
    connectivityModuleCellular.checkNetwork = cellularCheckNetwork;
    connectivityModuleCellular.powerOnModule = powertheModem;
    connectivityModuleCellular.powerOffModule = turnOffCellular;
    connectivityModuleCellular.unsolictedResponse = unsolicCellularResponse;
    connectivityModuleCellular.unsolictedResponseFlag = &celMUsolicRes;
    connectivityModuleCellular.ConnectionInfo = cellularConnectionInfo;
    connectivityModuleCellular.restartInternet = celMrestartInternetConnection;
    connectivityModuleCellular.resatrtConnectivityModule = restarttheModem;
    debugStr = debugLog;

#if CONFIG_CALL_TYPE == HTTP
    connectivityModuleCellular.http = cellMHttp;
    connectivityModuleCellular.httpinit = cellMHttpInit;
#endif

#if CONFIG_CALL_TYPE == HTTPS
    connectivityModuleCellular.https = cellMHttps;
    connectivityModuleCellular.httpsinit = cellMHttpsInit;
#endif

#if DATA_CALL_TYPE == HTTP

#endif

#if DATA_CALL_TYPE == MQTT
    connectivityModuleCellular.mqtt = cellMqtt;
    connectivityModuleCellular.mqttCredentialsetup =cellMQttSetup;
#endif

#if DATA_CALL_TYPE == MQTTS
    connectivityModuleCellular.mqtts = cellMqtt;
    connectivityModuleCellular.mqttCredentialsetup =cellMQttSetup;
#endif

    connectivityModuleCellular.connectivityFlags = &cellularflags;

    ConfigureModemPins();
    cellularOpertionAoTimerInit();
    cellularBufferTimerInit();
    cellularUartInit();




}

unsigned int cellularstartup()
{
    volatile unsigned int rval = 0xff;
    switch(cellularstartupState)
    {
    case 1:
        //config done flag set once after power on or whenever its require to reconfigure.
        if(connectivityModule->connectivityFlags->configDone == 0)
        {


            rval = connectivityModule->setup();

            if(rval == cellularModuleInitSUCCESS)
            {
                memset(DEVICE_ID,0x00,sizeof(DEVICE_ID));
#ifdef  HARDCODED_DEV_ID
                memcpy(DEVICE_ID,DEV_ID,strlen(DEV_ID));
#else
                memcpy(DEVICE_ID,cellularData.imei,strlen(cellularData.imei));
#endif

                rval = PASS_SUCCESS;
                cellularstartupState = 2;
                connectivityModule->connectivityFlags->configDone = 1;
            }
            else
                if(rval == celMSimCardAbsent)
                {
                    restartEveryThing();
                }

        }
        else
        {
            cellularstartupState = 2;
        }
        break;
    case 2:
        rval = connectivityModule->checkNetwork();
        if(rval == cellularCheckNetworkSUCCESS)
        {
            cellularstartupState = 1;
            rval = FUNCTION_SUCCESS;
            debugLog("IA:NW Registered\n");
        }
        else
            if(rval == cellMrestartEverything)
            {
                restartEveryThing();
            }
            else
            {
                rval = PASS_SUCCESS;
            }
        break;

    }
    return rval;
}

unsigned int getModule()
{
    unsigned int rval = 0xff;

    switch(getModuleState)
    {
    case getModuleidea:
        //select the connectivity module:
        //1. cellular Module
        //2. wifi Module.
#ifdef wifiModuleActive
        iotAppflags.moduleType = WIFI_TYPE;
#else
        connectivityModule = &connectivityModuleCellular;
        connectivityModule->connectivityFlags->configDone = 0;
        connectivityModule->connectivityFlags->httpConnPreConfigure = 0;
        connectivityModule->connectivityFlags->mqttConnPreConfigure = 0;
        connectivityModule->connectivityFlags->ssltlsConnPreConfigure = 0;
        connectivityModule->connectivityFlags->tcpConnPreConfigure = 0;

        iotAppflags.Connectivity = 0;
        iotAppflags.moduleType = CELLULAR_TYPE;

        getModuleState = getmodulestartcell;

        debugLog("IA:cellular Module Selected\n");

#endif

        break;

    case getmodulestartcell:
        rval = connectivityModule->powerOnModule();
        if(rval != cellularModemReady)
        {
            debugLog("IA:Modem unable to start.Device is restarting\n");
            __delay_cycles(100);
            resetSystem();
        }
        else
        {
            getModuleState = getmodulecellready;
            //			while(1);


        }
        break;


    case getmodulecellready:


        rval = cellularstartup();
        if(rval == FUNCTION_SUCCESS)
        {
            getModuleState = getmodleDetection;
            iotAppflags.Connectivity = 1;
        }
        else
            if(rval == cellMrestartEverything)
            {
                connectivityModule->powerOffModule();
                getModuleState = getmodulestartcell;


            }
        break;

    case getmodleDetection:
        if(iotAppflags.moduleChanged == 1)
        {
            getModuleState = getModuleidea;
            iotAppflags.moduleChanged = 0 ;
        }
        else
        {
            getModuleState = getmodulecellready;
        }
        break;
    }


    return rval;
}
/******************************************************************************************
 * point remember is each protocol exit in three way in cellular module
 * 1. Over AT command
 * 2. Over TCP protocol
 * 3. Over PPP protocol
 ******************************************************************************************/
/*
 * Configuration Request Call:
 * ===========================
 * 1.Requesting the configuration request to get provision or update to cofig cloud.
 * 2.This call specify only for acquiring the setting or configuration of  device for
 *   respective project.
 * 3.This call handle the all request which is required for
 *   a.provisioning,
 *   b.updating device own setting,
 *   c.updatig device status,
 *   d.updating device firmware
 *   e.updatind about device error
 *
 *
 *
 * Handling Case of configCall:
 * =============================
 * 1. SR - Setup Request
 *      * This first config request after factory made.
 *      * Return Value
 *           Success : Go to next Section or Sleep.
 *           fail    : Try for 3 time,go sleep and retry again after 30 Minute.
 *
 * 2.PR - PowerOn request
 *      * This is requested every normal power on
 *      * Return Value
 *           Success : Go to next Section or Sleep.
 *           fail    : Try for 3 time. Validate the previous setting and go forward
 *                     if validation is fine or if validation failed,go to sleep and
 *                     after 30 Minute,restart device and try its again.
 *
 * 3.SPR - Software PowerOn Request
 *      * This is requested after device restart itself to get recover from unexpected
 *        problem.
 *      * Return Value
 *          Please follow the PR case.
 *
 * 4.STR - Setting Request
 *      * This is requested when any device setting parameter is corrupted or cloud
 *        does not responding with current parameter.
 *      * Return Value
 *          Please follow the PR case.
 *
 * 5.WO - Watchdog Occurred
 *      * This is requested when device is restarted because of watchdog timeout.
 *      * Return Value
 *          Please follow the PR case.
 *
 * 6.HB - HeartBeat
 *      * This is request to update cloud about device condition
 *      * Return Value
 *          Please follow the PR case.
 *
 *
 *
 */

unsigned int configCall()
{

    volatile unsigned int rval = 0xff;

    switch(configCallState)
    {
    case 1:
       // Initialized http or http stack
        if(connectivityModule->connectivityFlags->httpConnPreConfigure == 0)
        {
            rval = connectivityModule->httpsinit();

        }
        else
        {
            configCallState = 2;
        }
        break;

    case 2:
        //capture live info from connectivity modem
        rval = connectivityModule->ConnectionInfo();

        break;
    case 3:
        /******************************************************
         * form a packet if required.
         *
         ******************************************************/
/******************** Parameter comman for all project *****************************************/

        //		iotCommanTemp = (char *)calloc(1,iotCommanBufSize);
        memset(iotCommanTemp,0x00,iotCommanBufSize);
        strcpy(iotCommanTemp,"{");
        strcat(iotCommanTemp,"\"method\":\"CONFIG\",\"params\":{\"P00\":\"");

#ifdef HARDCODED_DEV_ID
        strcat(iotCommanTemp,DEV_ID);

#else
        strcat(iotCommanTemp,cellularData.imei);
#endif

        strcat(iotCommanTemp,"\",\"P01\":\"");

        if(iotAppflagsPerment.setupConfigDone == 0)
        {
            configCallStatus = sr;
        }
        else
        {
            if(iotAppflags.devicePowerOn == 0)
            {
                if(configCallStatus < wo )
                    configCallStatus = pr;
            }
            else
                if(iotAppflagsPerment.subscriptionover == true)
                {
                    configCallStatus = subover;
                }


        }


        switch(configCallStatus)
        {

        case hb:
            strcat(iotCommanTemp,"HB");break;
        case spr:
            strcat(iotCommanTemp,"SPR");
            break;

        case sr:
            strcat(iotCommanTemp,"SR");break;
        case pr:
            strcat(iotCommanTemp,"PR");break;
        case str:
            strcat(iotCommanTemp,"STR");
            break;
        case wo:
            strcat(iotCommanTemp,"WO");
            break;
        case subover:
            strcat(iotCommanTemp,"SUBOVER");break;

        default: strcat(iotCommanTemp,"");

        }


        strcat(iotCommanTemp,"\",\"P02\":\"");
#ifdef wifiModuleActive
        if(iotAppflags.moduleType == CELLULAR_TYPE)
        {
            strcat(iotCommanTemp,"cell");
        }
        else
            if(iotAppflags.moduleType == WIFI_TYPE)
            {
                strcat(iotCommanTemp,"wifi");
            }
#else
        strcat(iotCommanTemp,"cell");
#endif



        strcat(iotCommanTemp,"\",\"P03\":\"");
        strcat(iotCommanTemp,cellularData.csq);

 /******************** Parameter of cellular Modem for all project *****************************************/

        strcat(iotCommanTemp,"\",\"P10\":\"");
        strcat(iotCommanTemp,cellularData.ccid);

        strcat(iotCommanTemp,"\",\"P11\":\"");
        strcat(iotCommanTemp,cellularData.FWversion);

        strcat(iotCommanTemp,"\",\"P12\":\"");
        strcat(iotCommanTemp,cellularData.cops);
        strcat(iotCommanTemp,"_");
        strcat(iotCommanTemp,cellularData.operattype);

        strcat(iotCommanTemp,"\",\"P13\":\"");
        strcat(iotCommanTemp,cellularData.celllocation);

/******************** Parameter of wifi Modem for all project *****************************************/



/******************** Parameter of project *****************************************/

        strcat(iotCommanTemp,"\",");

        applicationCofigCallform(iotCommanTemp+strlen(iotCommanTemp));

        strcat(iotCommanTemp,"}}");

//                strcpy(iotCommanTemp,"{\"A\":1}");
        debugLog("\nIA:Config Call:\n");
        debugLog(iotCommanTemp);
        debugLog("\n");
        __delay_cycles(100);

        memset(configUrl, 0x00, sizeof(configUrl));
        strncpy(configUrl,CONFIG_URL,strlen(CONFIG_URL));
        strncat(configUrl,DEVICE_ID,strlen(DEVICE_ID));
        strncat(configUrl,CONFIG_URL_PATH,strlen(CONFIG_URL_PATH));

        configCallState++;
        HttpResponsBuffer = 0;
        HttpResponsBufferLen = 0;

        break;

        case 4:
//            //http/https call
            rval = connectivityModule->https((int8_t *)configUrl,(int8_t *)iotCommanTemp,strlen(iotCommanTemp),HTTP_POST_METHOD,(int8_t *)"application/json",cloudResponse);
            break;

        default:
        {
            iotAppflags.devicePowerOn = 1;
            configCallState = 0;
            rval = FUNCTION_SUCCESS;
        }break;


    }

    switch(rval)
    {
    case HTTP_INIT_SUCCESS : connectivityModule->connectivityFlags->httpConnPreConfigure = 1;
    case HTTP_SUCCESS:
    case ConnectionInfoSUCCESS:
    case FUNCTION_SUCCESS:
        configCallState++; break;

    case HTTP_ERROR:
        //restart whole device
        // Check first device is register and provision if not,
        // go to sleep and try again after 30 min otherwise restart
        if(configCallStatus == pr)
        {
//            configCallTime  = 30;
            debugLog("IA:Device unable to provision.Retry after 30min\n");
            rval = CONFIG_CALL_FAILED;
        }
        else
        {
            if(false)
            {
                //validate the setting receiving in previous call
                rval = FUNCTION_SUCCESS;

            }
            else
            {
                if (iotAppflagsPerment.configCallRestartflag == 0)
                {
                    debugLog("IA:Config call failed.Restarting...\n");
                    iotAppflagsPerment.configCallRestartflag = 1;
                    restartEveryThing();
                }
                else
                {
                    debugLog("IA:Config call failed.Retry after 30min\n");
                    iotAppflagsPerment.configCallRestartflag = 0;
                    rval = CONFIG_CALL_FAILED;
                }
            }

        }
        break;


    case HTTP_BADREQUEST:   connectivityModule->connectivityFlags->configDone = 0;
    case SERVER_BUSY:
        debugLog("IA:Config Server not responding\n");
        __delay_cycles(10);

    case CONNECTION_CLOSED : debugLog("IA:Connection closed requested \n");
    case RESTART_MODEM     :

        clearAllConnectivityflag();
        configCallState = 1;
        break;
    default:break;

    }

    return rval;
}



/*
 *FUNCTION_SUCCESS  - Everything Success
 *CONNECTION_CLOSED - Connection closed and reestablished
 *RESTART_MODEM     - Module unable to respond
 *SERVER_BUSY       - Server not responding.Try later
 */
uint16_t dataCall()
{
    uint16_t rval = 0xff;

    switch(dataCallState)
    {
    case 1:
        if(iotAppflags.dataCallCredentialForm == false)
        {
            //form the credentional here and stored variable
#if ((DATA_CALL_TYPE == MQTT) || (DATA_CALL_TYPE == MQTTS))
#ifdef OW_MQTT_CLOUD
            SPI_MultipleContinuouseRead((unsigned char *)MQTT_IOT_HUB_URL, DATA_URL_SA, DATA_URL_EA-DATA_URL_SA);
            SPI_MultipleContinuouseRead((unsigned char *)MQTT_IOT_HUB_PASSWORD, DATA_PASS_SA, DATA_PASS_EA-DATA_PASS_SA);
            SPI_MultipleContinuouseRead((unsigned char *)MQTT_IOT_HUB_PUB_TOPIC, DATA_SENDENDPOINT_SA, DATA_SENDENDPOINT_EA-DATA_SENDENDPOINT_SA);
            SPI_MultipleContinuouseRead((unsigned char *)MQTT_IOT_HUB_SUB_TOPIC, DATA_RECIEVEPOINT_SA, DATA_RECIEVEPOINT_EA-DATA_RECIEVEPOINT_SA);
            memcpy((char *)MQTT_IOT_HUB_USERNAME,(char *) DEVICE_ID, strlen((char *)DEVICE_ID));
//            memcpy(MQTT_IOT_HUB_PUB_TOPIC, "v1/devices/me/telemetry", strlen("v1/devices/me/telemetry"));
//            memcpy(MQTT_IOT_HUB_SUB_TOPIC, "v1/devices/me/attributes", strlen("v1/devices/me/attributes"));


#endif
       connectivityModule->mqttCredentialsetup(URL,PORT,USERNAME,PASSWORD,ID,0,SB_TOPIC);

#elif ((DATA_CALL_TYPE == HTTP) || (DATA_CALL_TYPE == HTTPS))


#endif
            iotAppflags.dataCallCredentialForm = true;
        }

       //get the packet from here
        memset(iotCommanTemp,0x00,iotCommanBufSize);
        dataCallPayloadLen = getAppData((int8_t*)iotCommanTemp, iotCommanBufSize);
       //validate the packet receive


        dataCallState = 2;


    case 2:
           rval = connectivityModule->mqtt((int8_t*)iotCommanTemp,dataCallPayloadLen,PB_TOPIC,1);
        break;

    default:
        updateAppDataAddress(dataCallPayloadLen);
        dataCallState = 0;
        rval = FUNCTION_SUCCESS;
        break;

    }

    switch(rval)
    {
    case Flash_Empty:  rval = FUNCTION_SUCCESS;
    case MQTT_SUCCESS:
    case FUNCTION_SUCCESS: dataCallState++;    break;


    case RESTART_INTERNET:
    case CONNECTION_CLOSED:
        //clear the all config variable
        clearAllConnectivityflag();
        dataCallState = 1;

        break;

    case MQTT_CONNECTION_NOT_OPEN:
             rval = RESTART_MODEM;
             dataCallState = 1;
             break;

    case MQTT_NOT_RESPOND:
        // Three time tried and got same MQTT error
        // Reason could be Server Busy or Connectivity Module issue
        // Returning server Busy
        rval  = SERVER_BUSY;
        dataCallState = 1;
        break;




    case MQTT_ACK_CRED_ERR:
        configCallStatus = str;
        iotAppflags.ConfigCallSend = true;
    case MQTT_ACK_ID_ERR:
        rval = CONNECTION_CLOSED;
        iotAppflags.moduleChanged = 1 ;
        connectivityModule->connectivityFlags->configDone = 0;
        dataCallState = 1;

        break;

    }

    return rval;
}

uint16_t isDataAvailable()
{

    return applicationDataAvailable();

}

uint16_t restartModule()
{
    return connectivityModule->resatrtConnectivityModule();

}

uint16_t restartInternet()
{
    return connectivityModule->restartInternet();
}


uint16_t cloudResponse(int8_t* buf,uint16_t buflen)
{

    debugLog((char *)buf);
    return applicationCloudResponse(buf,buflen);
}


void clearAllConnectivityflag()
{
    iotAppflags.Connectivity = 0;
    connectivityModule->connectivityFlags->httpConnPreConfigure = 0;
    connectivityModule->connectivityFlags->mqttConnPreConfigure = 0;
    connectivityModule->connectivityFlags->ssltlsConnPreConfigure = 0;
    connectivityModule->connectivityFlags->tcpConnPreConfigure = 0;
}




/************************************************************************************************************

                                       SYSTEM CORE API
************************************************************************************************************/
void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int itoa(int x, char str[])
{
    int i = 0;
    if(x == 0)
        str[i++] = '0';
    else
    {
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
       // str[i++] = '0';

    reverse(str, i);
    }
    str[i] = '\0';
    return i;
}


void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = itoa(ipart, res);
    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
       //

        while(afterpoint--)
        {
            fpart *= 10;

        }


        itoa((int)fpart, res + i + 1);
    }
}

//char *ltoa(long N, char *str, int base)
//{
//	register int i = 2;
//	long uarg;
//	char *tail, *head = str, buf[1024];
//
//	if (36 < base || 2 > base)
//		base = 10;                    /* can only use 0-9, A-Z        */
//	tail = &buf[1024 - 1];           /* last character position      */
//	*tail-- = '\0';
//
//	if (10 == base && N < 0L)
//	{
//		*head++ = '-';
//		uarg    = -N;
//	}
//	else  uarg = N;
//
//	if (uarg)
//	{
//		for (i = 1; uarg; ++i)
//		{
//			register ldiv_t r;
//
//			r       = ldiv(uarg, base);
//			*tail-- = (char)(r.rem + ((9L < r.rem) ?
//					('A' - 10L) : '0'));
//			uarg    = r.quot;
//		}
//	}
//	else  *tail-- = '0';
//
//	memcpy(head, ++tail, i);
//	return str;
//}

//void delay(uint32_t msec)
//{
//	 vTaskDelay(msec / portTICK_PERIOD_MS);
//}

//char *strnstr( char *src, char *dest,unsigned int n)
//{
//    char *s=src;
//    char *d=dest;
//    char cp,*temp,matched=0;
//
//    n+=1;
//    while(n)
//    {
//        temp=s;
//        d=dest;
//        do
//        {
//            if((*s - (cp =*d)))
//            {
//                break;
//            }
//            else
//            {
//                matched=1;
//                s++;d++;
//                n--;
//            }
//
//        }while (cp && n);
//
//        if(cp == 0 )
//            return temp;
//
//        if(matched == 0)
//        {
//            s++; n--;
//        }
//        else
//            matched=0;
//
//    }
//
//    return 0;
//}
//
//

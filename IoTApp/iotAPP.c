/*
 * iotAPP.c
 *
 *  Created on: May 8, 2020
 *      Author: gaurav
 */

#include "iotAPP.h"
#include "Application/Application.h"

uint8_t    iotAppState    = 0;
uint16_t   rval           = 0xff;
char       enterToSleep;


iAF iotAppflags;

#pragma PERSISTENT(iotAppflagsPerment)
iAFPermant iotAppflagsPerment={0};

uint16_t iotAppInit()
{

//Configure or Initialize the hardware and interface with cellular Module.
    ConfigureModule();

//Configure or Initialize the variable which is need in application
#ifdef CONFIG_CALL_ENABLE
    iotAppflags.ConfigCallReq = true;
#endif

   return 1;
}



void iotAppRun(void *arg)
{


//Handle unsolicited response
		if(iotAppflags.moduleType && (*connectivityModule->unsolictedResponseFlag))
		{
			rval = connectivityModule->unsolictedResponse();
			iotAppflags.Connectivity = 0;
			*connectivityModule->unsolictedResponseFlag = 0;
		}

//Handle the Sleep mode if enable.
#ifdef SLEEP_MODE_ENABLE
		if(enterToSleep && ((iotAppflags.sendingInProcess == false)))
		    iotAppState = 6;
#endif

//Handle the configuration call if enable.
#ifdef CONFIG_CALL_ENABLE
		if((iotAppflags.ConfigCallReq == 1) && (iotAppflags.sendingInProcess == false) && (iotAppflags.Connectivity == 1))
		{
		    iotAppState = 2;
		    iotAppflags.ConfigCallSend = 1;
		    iotAppflags.ConfigCallReq = 0;

		}
#endif


//MAIN IOT APPLICATION
		switch(iotAppState)
		{

		case 0:
			//Wait for trigger to start IOT Application or directly start//
			iotAppState = 1;
			break;
		case 1:
			//Selection of connectivity module
		    //Get the network and Internet attached.
			rval = getModule();
			if(rval == FUNCTION_SUCCESS)
			{
			    debugLog("IA:Connectivity Module Ready\n");
				iotAppState = 2;

			}
			break;

		case 2:
		    //Configuration call to get configure,update setting and
		    //subscription as well as update device state in HB
#ifdef CONFIG_CALL_ENABLE
			if(iotAppflags.ConfigCallSend == true)
			{
			    iotAppflags.sendingInProcess = true;
				rval = configCall();
				switch(rval)
				{
				case SERVER_BUSY:
				case FUNCTION_SUCCESS :
				    iotAppState = 3;
				    iotAppflags.ConfigCallSend = false;
				    iotAppflags.sendingInProcess = false;
				    break;

				case HTTP_BADREQUEST:
				case CONNECTION_CLOSED: iotAppState = 1; iotAppflags.sendingInProcess = false;break;
				case RESTART_MODEM    : iotAppState = 5;iotAppflags.sendingInProcess = false; break;
				case RESTART_INTERNET : iotAppState = 4;iotAppflags.sendingInProcess = false; break;

				case CONFIG_CALL_FAILED: iotAppState = 6; iotAppflags.ConfigCallSend = false; iotAppflags.sendingInProcess = false;break;
				}
			}
			else
			{
				iotAppState = 3;
			}
			break;
#else
			iotAppState = 3;
#endif

		case 3:
			//Operation or Data call to send application data to receive cloud information

		    if((iotAppflagsPerment.setupConfigDone == 1) && (iotAppflags.prConfigDone == 1))
		    {
		        if(iotAppflags.Connectivity == true)
		        {
		            if(isDataAvailable())
		            {
		                iotAppflags.deviceInOperation = true; // this flag only used to keep alive mqtt connection in some case
		                iotAppflags.sendingInProcess = false;
		                rval = dataCall();
		                switch(rval)
		                {
		                case FUNCTION_SUCCESS:
		                    debugLog("IA:DATA Uploaded\n");
		                    iotAppflags.deviceInOperation = false;
		                    iotAppflags.sendingInProcess = false;
		                    break;
		                case SERVER_BUSY:
		                    debugLog("IA:DATA Server Busy\n");
		                    iotAppState = 6;

		                break;
		                case RESTART_INTERNET : iotAppState = 4; iotAppflags.sendingInProcess = false;break;
		                case CONNECTION_CLOSED: iotAppState = 1;iotAppflags.sendingInProcess = false; break;
		                case RESTART_MODEM    : iotAppState = 5;iotAppflags.sendingInProcess = false; break;
		                }
		            }
		            else
		            {
		                if(iotAppflags.deviceInOperation == false)
		                {
		                    debugLog("IA:No DATA Available\n");
		                    iotAppState = 6;
		                }

		            }
		        }
		        else
		        {
		            iotAppState = 1;
		        }
		    }
		    else
		    {
		        configCallStatus = str;
		        iotAppflags.ConfigCallReq = true;
		        iotAppState = 1;
		        debugLog("IA:No Setting found.Requesting Again\n");
		    }

			break;
		case 4:
			//Restart the Internet of connectivity Module.
		    rval = restartInternet();
		    switch(rval)
		    {
		    case FUNCTION_SUCCESS: iotAppState = 1; clearAllConnectivityflag();  break;
		    case RESTART_MODEM:  iotAppState = 5; break;
		    }

			break;

		case 5:
			//Restart the connectivity Module.
		    rval = restartModule();
		    switch(rval)
		    {
		    case cellularModemRestartSuccess: iotAppState = 1; clearAllConnectivityflag(); break;
		    }
			break;

		case 6:
			//Sleep Section if enable.
#ifdef SLEEP_MODE_ENABLE
		    connectivityModule->powerOffModule();
		    WDT_A_hold(WDT_A_BASE);
		    sleepDevice();
		    clearAllConnectivityflag();
		    iotAppState = 1;
		    iotAppflags.moduleChanged = 1 ;
		    connectivityModule->connectivityFlags->configDone =0;
		    debugLog("\nIA:wakeup from Sleep\n");
#else

#endif
			break;
		default:
		    //Device never enter this state if come,it will restart
		    debugLog("\nIA:Code Crash\n");
		    restartEveryThing();
			break;


		}



}



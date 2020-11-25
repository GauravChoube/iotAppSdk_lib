/*
 * Application.c
 *
 *  Created on: Aug 21, 2020
 *      Author: Gcsystem
 */
#include "Application.h"

char     lowPowerIndication = 0;

uint32_t FlashWriteAdrress      = APPLICATION_SA;
uint32_t CloudSendAddress       = APPLICATION_SA;

#pragma PERSISTENT(appBuff)
int8_t appBuff[512]={0};


#pragma PERSISTENT(DEVICE_ID)
int8_t DEVICE_ID[17]={0};


//sysn counter and timer
uint16_t   configCounter = 0;
uint16_t   configCallTime = 5;

//app data counter
uint16_t appHBcnt = 0;
uint16_t appHBtime = 300;
bool     appHBSync = true;

//application packet
bool    forminpacketinprogress = false;
uint8_t PackteFrmFlag = 0;
uint8_t applicationPacketFormState = 1;
uint16_t Pcktrval = 0;

uint16_t applicationInit()
{
    uint16_t rval = 0xff;


    rval =  extFlashInit();
    if(rval != 1)
    {
        debugLog("AP:Ext Flash Configured failed\n");
        return rval;
    }
    else
    {
        rval = check_flash();
        if(rval != 1)
        {
            debugLog("AP:Ext Flash Init failed\n");
            return rval;
        }
        else
        {
            debugLog("AP:Ext Flash Initialized\n");
            if(APPLICATION_SA <0x10000)
            {
                debugLog("AP:APPLICATION_SA set to reserved 1st Sector\n");
                resetSystem();
            }
        }

    }

    rval = internalFlashInit();
    if(rval !=1)
    {
        debugLog("AP:Internal Flash Init failed\n");
        return rval;
    }
    else
    {
        debugLog("AP:Internal Flash Initialized\n");
    }

    internalflashRead(setupflag,&iotAppflagsPerment.setupConfigDone);
    internalflashRead(Suboverflag,&iotAppflagsPerment.subscriptionover);

    if(iotAppflagsPerment.setupConfigDone == 0)
    {
        //erase the external flash completely
        debugLog("AP:EXT Flash Erasing..\n");
        SPI_Erase(SFM_Chip_erase, SFM_Chip_erase_1);
        debugLog("AP:Erased Done\n");
    }





    return 1;
}



uint16_t applicationPacketForm(uint16_t packettype)
{
    uint16_t rval = 0xff;
    uint8_t rtcBuffer[15]={0};

    switch(applicationPacketFormState)
    {

    case 1:
    {
        strcpy((char *)appBuff,"{");
        strcat((char *)appBuff,"\"BID\":\"");

        strncat((char *)appBuff,(char *)DEVICE_ID,strlen((char *)DEVICE_ID));
        strcat((char *)appBuff,"\",\"");
        rval = APP_SUCCESS;

    } break;

    case 2:
    {
        //get data from individual and store into buffer or location i flash
        switch(packettype)
        {
        case FORM_LOCATION_PACKET:

            rval =  APP_SUCCESS;
            break;
        case FORM_BAT_ALT_PACKET:

            rval =  APP_SUCCESS;
            break;

        case FORM_PW_FAIL_PACKET:
            rval =  APP_SUCCESS;
            break;

        case FORM_HEARTBEAT_PACKET:

            rval =  APP_SUCCESS;
            break;
        }

    }break;

    case 3:
    {
        //form the actual packet


        switch(packettype)
        {

        case FORM_HEARTBEAT_PACKET:
            strcat((char *)appBuff,"HB\":\"1");
            rval =  APP_SUCCESS;
            break;


        case FORM_LOCATION_PACKET:

            rval =  APP_SUCCESS;

            break;


        case FORM_BAT_ALT_PACKET:
        {
            strcat((char *)appBuff,"LVA\":\"1");
            rval =  APP_SUCCESS;
        }break;

        case FORM_PW_FAIL_PACKET:

            strcat((char *)appBuff,"PWR\":\"1");
            rval = APP_SUCCESS;
            break;
        }



    }break;


    case 4:
    {
        //finished the packet and load into flash memoroy


        strcat((char *)appBuff,"\",\"BTS\":\"");
        //rtc
         memset(rtcBuffer,0x00,sizeof(rtcBuffer));
         rtcget((char *)rtcBuffer);

        strncat((char *)appBuff,(char *)rtcBuffer,strlen((char *)rtcBuffer));
        strcat((char *)appBuff,"\"}");

        switch(packettype)
        {
        case FORM_LOCATION_PACKET:
            debugLog("\nAP:Location: ");

            break;
        case FORM_PW_FAIL_PACKET:
            debugLog("\nAP:Power Fail: ");
            break;

        case FORM_BAT_ALT_PACKET:
        {
            debugLog("\nAP:Battery Alert: ");
        }break;

        case FORM_HEARTBEAT_PACKET:
        {
            debugLog("\nAP:HeartBeat: ");
        }break;


        }

        debugLog((char *)appBuff);
        debugLog("\n");
        //store the data into flash
        storedAppData(appBuff, strlen((char *)appBuff));
        memset(appBuff,0x00,sizeof(appBuff));
        rval = APP_SUCCESS;

    }break;

    default:
        rval = APP_PACKET_FOMR_SUCCESS;
        applicationPacketFormState = 0;

    }

    switch(rval)
    {

    case APP_PACKET_FOMR_SUCCESS:
    case APP_SUCCESS:  applicationPacketFormState++; break;

    }



    return rval;

}

uint16_t applicationPacketFormTask()
{
    if((appHBSync == true) && ((forminpacketinprogress == false) || !(FORM_HEARTBEAT_PACKET ^ (PackteFrmFlag))))
    {
        PackteFrmFlag = FORM_HEARTBEAT_PACKET;
        forminpacketinprogress = true;
        Pcktrval =  applicationPacketForm(FORM_HEARTBEAT_PACKET);
        if(Pcktrval == APP_PACKET_FOMR_SUCCESS)
        {
            forminpacketinprogress = false;
            PackteFrmFlag = 0;
            appHBSync = false;

        }
    }

    return 1;
}

uint16_t applicationCloudResponse(int8_t *buf,uint16_t buflen)
{

    unsigned int rval = 0xff;
    char *SettingFromCloud = 0,*str = 0;
    char *settingsToken = 0;
    unsigned int buffer_len = 0,i = 0;
    char tmp[20]={0};



    if(SettingFromCloud = (char *)strnstr((char *)buf,"\"TYPE\":\"SET\"",buflen))
    {

        if(SettingFromCloud == NULL)
        {
            return rval = CLOUDCMD_RES_SUCCESS;
        }
        else
        {

        }

        settingsToken = strtok((char *)SettingFromCloud+20, "}");


        buffer_len = strlen((char *)settingsToken);

        settingsToken = strtok((char *)settingsToken , ",");
        str = 0;

        for(i=0;i < buffer_len;i++)
        {

            if(str=strstr(settingsToken, "\"F01\""))
            {
                SPI_Erase(SFM_SectorErase, SECTOR_2);
                //DATA URL
                SPI_CheckAndWriteIntoFlash(str + 7, strlen(str + 7)-1, DATA_URL_SA, DATA_URL_EA, true);
            }
            else if(str=strstr(settingsToken, "\"F02\""))
            {
                //DATA PASS
                SPI_CheckAndWriteIntoFlash(str + 7, strlen(str + 7)-1, DATA_PASS_SA, DATA_PASS_SA, true);

            }
            else if(str=strstr(settingsToken, "\"F03\""))
            {
                //DATA SEND ENDPOINT
                SPI_CheckAndWriteIntoFlash(str + 7, strlen(str + 7)-1, DATA_SENDENDPOINT_SA, DATA_SENDENDPOINT_EA, true);

            }
            else if(str=strstr(settingsToken, "\"F04\""))
            {
                //DATA RECEIVE ENDPOINT
                SPI_CheckAndWriteIntoFlash(str + 7, strlen(str + 7)-1, DATA_RECIEVEPOINT_SA, DATA_RECIEVEPOINT_EA, true);

            }
            else if(str=strstr(settingsToken, "\"F05\""))
            {
                //DATA PROTOCOL TYPE

            }
            else if(str=strstr(settingsToken, "\"F06\""))
            {
                //COUNTRY NAME
                SPI_CheckAndWriteIntoFlash(str + 7, strlen(str + 7)-1, COUNTRY_NAME_SA, COUNTRY_NAME_EA, true);
            }
            else if(str=strstr(settingsToken, "\"F07\""))
            {
                //RTC FROM CLOUD
                memset(tmp,0x00,sizeof(tmp));
                memcpy(tmp,str + 7, strlen(str+7));

                if(rtcset((char *)tmp))
                {

                }


            }
            else if(str=strstr(settingsToken, "\"F11\""))
            {
                //ALIASE NAME
                SPI_CheckAndWriteIntoFlash(str + 7, strlen(str + 7)-1, ALIAS_NAME_SA, ALIAS_NAME_SA, true);
            }
            else if(str=strstr(settingsToken, "\"F12\""))
            {
                //OWNER NAME
                SPI_CheckAndWriteIntoFlash(str + 7, strlen(str + 7)-1, OWNER_NAME_SA, OWNER_NAME_SA, true);
            }

            else if(str=strstr(settingsToken, "\"F13\""))
            {
                //HEART BEAT TIME
                memset(tmp,0x00,sizeof(tmp));
                memcpy(tmp,str + 7, strlen(str+7)-1);
#ifdef CONFIG_TIME_HARDCODED
                configCallTime = CONFIG_CALL_TIME;
#else
                configCallTime  = atoi(tmp);
#endif

            }
            else if(str=strstr(settingsToken, "\"F20\""))
            {
                memset(tmp,0x00,sizeof(tmp));
                memcpy(tmp,str + 7, strlen(str+7)-1);
                appHBtime  = atoi(tmp);

            }

            else if(str=strstr(settingsToken, "\"F21\""))
            {
                //THRESOLD PVERSPEED


            }

            debugLog(settingsToken);
            debugLog("\n");
            i += strlen(settingsToken);
            settingsToken = strtok(NULL, ",");



        }



        internalflashWrite(setupflag, 1);
        iotAppflags.prConfigDone = 1;


        rval = CLOUDCMD_RES_SUCCESS;

    }
    else if(strnstr((char *)buf,"\"TYPE\":\"OTA\",\"VAL\":\"1\"",buflen))
    {
        //           OTA_requested = true;

    }
    else if(strnstr((char *)buf,"\"TYPE\":\"RST\",\"VAL\":\"1\"",buflen))
    {

        restartEveryThing();
    }
    else if(strnstr((char *)buf,"\"TYPE\":\"RSTF\",\"VAL\":\"1\"",buflen))
    {
        //factory reset

        //           rst_requested = true;
        //           //   Flash_State =1;
        //           BoT_power_ON = true;
        //           GetRTCfromibothive = true;
        //           OTAUpgradeProcess = false;
        //           RTC_Ready = false;
        //           ProvisionDone = false;
        //           FC = true;
        //
        //           GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);
        //           GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN1);
        //           __delay_cycles(160000);
        //           GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);
        //  PMM_trigBOR();

    }
    else if(strnstr((char *)buf,"\"TYPE\":\"SUBOVER\",\"VAL\":\"1\"",buflen))
    {
        internalflashWrite(Suboverflag, 1);
        rval = CLOUDCMD_RES_SUCCESS;

    }
    else if(strnstr((char *)buf,"\"TYPE\":\"SUBOVER\",\"VAL\":\"0\"",buflen))
    {
        internalflashWrite(Suboverflag, 0);
        rval = CLOUDCMD_RES_SUCCESS;

    }
    else if(settingsToken = (char *)strnstr((char *)buf,"\"TYPE\":\"CLK\"",buflen))
    {

    }
    else if(strnstr((char *)buf,"\"TYPE\":\"RES\",\"VAL\":\"1\"",buflen))
    {
        rval = CLOUDCMD_RES_SUCCESS;
    }
    else if(strnstr((char *)buf,"\"TYPE\":\"RES\",\"VAL\":\"0\"",buflen))
    {
        rval = CLOUDCMD_RES_FAILED;
    }
    else
    {
        rval = CLOUDCMD_RES_FAILED;
    }

    return rval;


}

bool applicationDataAvailable()
{
    return CloudSendAddress != FlashWriteAdrress;
}

bool applicationCofigCallform(char *iotCommanTemp)
{
/******************** Parameter depend on project *****************************************/

    //aliase name
    strcat(iotCommanTemp,"\"P30\":\"");
    memset(appBuff,0x00,sizeof(appBuff));
    SPI_MultipleContinuouseRead((unsigned char *)appBuff, ALIAS_NAME_SA, ALIAS_NAME_EA-ALIAS_NAME_SA);
    strcat(iotCommanTemp,(char *)appBuff);

    //owner name
    strcat(iotCommanTemp,"\",\"P31\":\"");
    memset(appBuff,0x00,sizeof(appBuff));
    SPI_MultipleContinuouseRead((unsigned char *)appBuff, OWNER_NAME_SA, OWNER_NAME_EA-OWNER_NAME_SA);
    strcat(iotCommanTemp,(char *)appBuff);

    //project Firmware version
    strcat(iotCommanTemp,"\",\"P32\":\"");
       strcat(iotCommanTemp,(char *)Project_Firmware_Version);

/******************** Parameter common with project *****************************************/

    //battery voltage of main or backup
    strcat(iotCommanTemp,"\",\"BT\":\"");

    memset(appBuff,0x00,sizeof(appBuff));
    powerModuleStatus((char *)appBuff);

    strcat(iotCommanTemp,(char *)appBuff);
    //rtc
    memset(appBuff,0x00,sizeof(appBuff));
    rtcget((char *)appBuff);
    strcat(iotCommanTemp,"\",\"TS\":\"");
    strcat(iotCommanTemp,(char *)appBuff);
    strcat(iotCommanTemp,"\"");





    return 1;
}
/***************************************************************************************************************************/
//EXTERNAL FLASH SECTION
//======================
uint16_t extFlashInit()
{
    return SPI_Init(SPI_CLK_SMCKL, 16000000, 16000000);
}


uint16_t storedAppData(int8_t *data,uint16_t datalen)
{
    uint16_t rval = 0xff;

    if( ((CloudSendAddress < APPLICATION_SA+0x10000) && (FlashWriteAdrress+1024 >= APPLICATION_EA)) )
    {
        flash_memory_full = true;
        datalen = 0;
        rval = Flash_FULL;

        debugLog("AP:Ext Flash is Full\n");
    }
    else
    {
        if((FlashWriteAdrress+1024)  >= APPLICATION_EA)
        {
            if(CloudSendAddress >= APPLICATION_SA)
            {
                //u can change fFlashWriteAdrress to application_Sa and start writing
                FlashWriteAdrress = APPLICATION_SA;
                rval =   SPI_ModifyWrite_Flash((char *)data, FlashWriteAdrress, datalen, true);
                FlashWriteAdrress = FlashWriteAdrress + rval+1;
            }
        }
        else
        {
            if((CloudSendAddress >= APPLICATION_SA))
            {

                if(FlashWriteAdrress == CloudSendAddress)
                {
                    rval=SPI_ModifyWrite_Flash((char *)data, FlashWriteAdrress, datalen, true);
                    FlashWriteAdrress = FlashWriteAdrress + rval+1;
                }
                else
                    if((FlashWriteAdrress + 0xffff) > CloudSendAddress)
                    {
                        flash_memory_full = true;
                        datalen = 0;
                        rval = Flash_FULL;

                        debugLog("AP:Ext Flash is Full\n");
                    }
                    else
                    {
                        rval=SPI_ModifyWrite_Flash((char *)data, FlashWriteAdrress, datalen, true);
                        FlashWriteAdrress = FlashWriteAdrress + rval+1;
                    }
            }
            else
            {
                rval =  SPI_ModifyWrite_Flash((char *)data, FlashWriteAdrress, datalen, true);
                FlashWriteAdrress = FlashWriteAdrress + rval+1;
            }
        }

    }

    return rval;
}

uint16_t getAppData(int8_t *Buffer,uint16_t Bufferlen)
{
    uint16_t rval = 0;


    if(FlashWriteAdrress == CloudSendAddress)
    {
        rval = Flash_Empty;
        debugLog("AP: Ext Flash is empty\n");

    }
    else
    {
        if(CloudSendAddress+1024 > APPLICATION_EA)
        {
           // debugLog("AP:Ext Flash Sector Erase\n");
            CloudSendAddress = APPLICATION_SA;
            SPI_Erase(SFM_BlockErase, SECTOR9_ADDRESSS);

        }
        rval = SPI_MultipleContinuouseRead((unsigned char *)Buffer, CloudSendAddress, Bufferlen);


    }


    return  rval;
}

void updateAppDataAddress( uint16_t len)
{
    uint16_t l = len;
    volatile long  tmpAddr = 0;
    while((((CloudSendAddress+tmpAddr) & 0xffff) != 0xffff) && len)
    {
        tmpAddr++;
        --len;
    }

    if(len != 0)
    {

        tmpAddr = CloudSendAddress;
        tmpAddr = tmpAddr >> 4;
        tmpAddr = tmpAddr & ~(0xfff);
        Erase_Sector = tmpAddr << 4;


       // debugLog("AP:Flash:Application Sector Erase\n");
        SPI_Erase(SFM_BlockErase, Erase_Sector);

    }


    CloudSendAddress +=l+1;

}
/***************************************************************************************************************************/
//Application Timer
//======================
void appTimerCB()
{


}

void appTimerInit()
{
    timerInit(timer2, mode_sec, appTimerCB);
    timerStart(timer2, 1);
}


/***************************************************************************************************************************/
//INTERNAL FLASH SECTION
//======================
uint16_t internalFlashInit()
{
    return 1;
}

bool internalflashWrite(uint16_t key,uint8_t Var)
{
    switch(key)
    {
    case setupflag:
        iotAppflagsPerment.setupConfigDone = Var;
        break;

    case Suboverflag:
        iotAppflagsPerment.subscriptionover = Var;
        break;


    }
    return 1;
}
bool internalflashRead(uint16_t key,uint8_t *var)
{
    switch(key)
    {
    case setupflag:
        *var = iotAppflagsPerment.setupConfigDone ;
        break;

    case Suboverflag:
        *var = iotAppflagsPerment.subscriptionover;
        break;


    }
    return 1;
}

/***************************************************************************************************************************/




//DEBUG UART SECTION
//===================
uint16_t debugUartInit()
{
    return uartInit(debugUart, 115200, 0, 0, 0);
}

void debugUartTx(char * str)
{
    uartWrite(debugUart,(int8_t *)str,strlen(str));
}


/***************************************************************************************************************************/
//WATCHDOG SECTION
//===================
uint16_t watchdogInit()
{
    WDT_A_initWatchdogTimer(WDT_A_BASE , WDT_A_CLOCKSOURCE_SMCLK , WDT_A_CLOCKDIVIDER_2G);  //DIVIDER/SMCLK = 2G/8MHz = (2*10^9)/(8*10^6) = 250sec = 4.1min
    //    WDT_A_initIntervalTimer(WDT_A_BASE , WDT_A_CLOCKSOURCE_SMCLK , WDT_A_CLOCKDIVIDER_2G);
//    SFRIE1 |= WDTIE;
    WDT_A_start(WDT_A_BASE);

    return 1;
}

void watchdogreset()
{
    WDT_A_resetTimer(WDT_A_BASE);
}


#pragma vector=WDT_VECTOR
__interrupt void wdt_isr(void)
{
    PMM_trigBOR();
}

/***************************************************************************************************************************/
//Power SECTION
//===================
void powerModuleCB(char typePort)
{
    //change the config for low power

}

uint16_t powerModuleInit()
{
    gpioSetup(GPIO_PORT_P1, GPIO_PIN2, GPIO_INPUT,true,GPIO_HIGH_TO_LOW_TRANSITION,powerModuleCB);
    batAdcInit();

    batConfiguration(MainsBat, MAINS_PORT, MAINS_PIN);
    batConfiguration(BackupBat, BAT_PORT, BAT_PIN);

return 1;
}

uint16_t powerModuleStatus(char *pwBuffer)
{
    //mode of power
    //1 -> Mians Power
    //2 -> Backup Power
    char ModeofPower = 0xff;
    char PowerState = 0xff;
    float mainsVoltage = 0.0;
    float batVoltage = 0.0;

    ModeofPower = gpioGet(GPIO_PORT_P1, GPIO_PIN2);
    if(ModeofPower == 1)
    {

        batVoltage = batVoltageRead(BackupBat);
        mainsVoltage = batVoltageRead(MainsBat);

        if(mainsVoltage <= LOW_VOLTAGE_THRESOLD)
        {
            PowerState = LOW_POWER_MODE;
        }
        else{
            PowerState = ACTIVE_POWER_MODE;
        }
    }
    else
    {
        PowerState = BACKUP_POWER_MODE;
        batVoltage = batVoltageRead(BackupBat);
    }

    if(pwBuffer != 0)
    {
        //sprintf(pwBuffer,"%d,%0.2f,%0.2f",ModeofPower,mainsVoltage,batVoltage);
        ModeofPower +=48;
        strncpy(pwBuffer,&ModeofPower,1);
        strncat(pwBuffer,",",1);
        ftoa(mainsVoltage, pwBuffer+strlen(pwBuffer), 2);
        strncat(pwBuffer,",",1);
        ftoa(batVoltage, pwBuffer+strlen(pwBuffer), 2);

    }

return PowerState;
}






/***************************************************************************************************************************/

void getPowerUpReason()
{
    uint16_t ResetCause=0xff;

//    if(configCallStatus < wdtrst)
    {

        ResetCause = SYSRSTIV;
        switch(ResetCause)
        {
        case 0x02:
            //BrownOut restart
            debugLog("Device Restart:BrownOut\n");
            configCallStatus = pr;
            break;
        case 0x04:
            //Reseet restart
            debugLog("Device Restart:Reset\n");
            configCallStatus = pr;
            break;

        case 0x06:
            //Software brownout restart
            debugLog("Device Restart:Software BrownOut\n");
            configCallStatus = spr;

            break;
        case 0x16:

            //watchdog restart
            debugLog("Device Restart:Watchdog Occurred\n");
            configCallStatus = wo;



            break;
        case 0x2A:
            //memory voliation 1 restart
            debugLog("Device Restart:Memory violation 1\n");
//            configCallStatus = mpv1;
            break;
        case 0x2C:
            //memory voliation 2 restart
            debugLog("Device Restart:Memory violation 2\n");
//            configCallStatus = mpv2;
            break;

        case 0x2E:
            //memory voliation 3 restart
            debugLog("Device Restart:Memory violation 3\n");
//            configCallStatus = mpv3;
            break;

        default:
            debugLog("Device Restart:UNknown Reason:");
            configCallStatus = pr;
            //              //////DebugUart(sys_var)
            break;
        }
    }
}

void ioUnusedInit()
{

}

void ioSleepEntry()
{

}

void ioSleepExit()
{

}

void sleepDevice()
{
    ioSleepEntry();
    SPI_Deep_Sleep_Entry();
    //application timer off if there
    //application adc off if there
    lowPowerIndication = 1;
    LPM3;
    SPI_Deep_Sleep_Exit();
    ioSleepExit();
    //application timer on if there
    //application adc on if there

}

void applicationSyncCnt()
{
    //min based counter
#ifdef CONFIG_CALL_ENABLE
    if(++configCounter >= configCallTime)
    {
        configCounter = 0;
        iotAppflags.ConfigCallReq = 1;
        configCallStatus = hb;
        lowPowerIndication = 2;//exit from low powr mode


    }
#endif

    if(++appHBcnt >= appHBtime)
    {
        lowPowerIndication = 2;//exit from low powr mode
        appHBSync = true;
        appHBcnt = 0;

    }




}

void restartEveryThing(void)
{
    turnOnCellular();
    resetSystem();
}

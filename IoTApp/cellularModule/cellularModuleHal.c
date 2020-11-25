
/*******************************************************************************
 * Name         : cellularModuleHal.c
 * Created on   : Feb 11, 2020
 * Author       : Gaurav Choubey
 * Description  :
 ******************************************************************************/
#include "cellularModuleConfig.h"
#include "cellHeader.h"
//#include "cellularModuleHeader.h"

/*********************************************************************************************************************************
 *                                                          UART SECTION
 *********************************************************************************************************************************/
#pragma PERSISTENT(celMUartBuf)
int8_t celMUartBuf[1]={0};


void CellularModuleUartCb(int8_t *Buff,uint16_t Buflen)
{

    if((celMBufferIndex+Buflen) < cellularBufferSize)
    {
        cellularBuffer[celMBufferIndex] = Buff[0];
        celMBufferIndex += Buflen;


    }
    else
    {
        //         if(GSMBuffer_Overflow == false)
        //         {
        //             GSMBuffer_Overflow = true;
        //             Extended_GSMBuffe_Address = Extended_GSMBuffer_SA;
        //             Extended_GSMBuffe_Address += SPI_Modify_MultipleContinuouseWrite(App_uart0_Buff, Extended_GSMBuffe_Address, App_uart0_CB_len, false);
        //
        //         }
        //         else
        //         {
        //             if((Extended_GSMBuffe_Address+App_uart0_CB_len) < Extended_GSMBuffer_EA-1)
        //             {
        //                 Extended_GSMBuffe_Address += SPI_Modify_MultipleContinuouseWrite(App_uart0_Buff, Extended_GSMBuffe_Address, App_uart0_CB_len, false);
        //             }
        //         }

    }
    startCellularBufferTimer(100);
}

bool cellularUartInit()
{
    return uartInit(cellularUart, 115200, CellularModuleUartCb,celMUartBuf,sizeof(celMUartBuf));
}

bool cellularUartReInit()
{
    return uartReinit(cellularUart, 115200);
}

void CellularModuleTxChar(int8_t ch)
{
    uartWrite(cellularUart,&ch, 1);
}

void CellularModuleTxstr(int8_t *str)
{

    uartWrite(cellularUart,str, strlen((char *)str));

}

/*********************************************************************************************************************************
 *                                                          TIMER SECTION
 *********************************************************************************************************************************/
volatile uint16_t       cellularMAxTime;
volatile uint16_t       cellularMAxCounter;
volatile int8_t         cellularMAxTimerOn;
volatile bool           cellularMAxTimeout;
volatile bool 		    StartcellularMAxTimerflag;


volatile uint16_t       cellularBufferTime;
volatile uint16_t       cellularBufferTimerCounter;
int8_t                  cellularBufferTimerOn;
bool                    cellularBufferTimeout;
volatile bool 		    StartBufferTimerflag;



volatile bool           startWaitTimer;
volatile uint16_t       SleepTime;
volatile uint16_t       SleepCounter;
volatile uint8_t        SleepTimerOn;
bool                    SleepCmdtimout;


bool                    StartModemtimerflag = false;
uint16_t                ModemtimerTime ;
volatile uint16_t       ModemtimerCounter;
uint8_t                 ModemtimerOn;
volatile bool           ModemtimerTimeOut = false;

volatile uint16_t       configCallCounter;

uint16_t                prescalar  = 0;



void cellularOpertionAoTimerCb()
{

    if(startWaitTimer)
    {
        if(++SleepCounter >= SleepTime)
        {

            SleepCmdtimout=true;
            SleepTimerOn=false;
            SleepCounter=0;
            startWaitTimer = false;

        }

    }

    if(StartModemtimerflag)
    {
        if(++ModemtimerCounter >= ModemtimerTime)
        {
            ModemtimerTimeOut =true;
            ModemtimerOn=false;
            ModemtimerCounter=0;
            StartModemtimerflag = false;
        }
    }

    if(StartcellularMAxTimerflag)
    {
        if(++cellularMAxCounter >= cellularMAxTime)
        {
            cellularMAxTimeout=true;
            cellularMAxTimerOn=false;
            cellularMAxCounter=0;
            StartcellularMAxTimerflag = false;
           // debugStr("MAxtimer\n'");
        }
    }

}

bool cellularOpertionAoTimerInit()
{
    bool rval = 0;

    rval = timerInit(cellularOpertionAotimer, mode_sec, cellularOpertionAoTimerCb);//cellularOpertionAoTimerCb
    if(rval)
    {
        StartcellularOpertionAoTimer(1024);
    }
    else
        return rval;

    return rval;
}

void StartcellularOpertionAoTimer(double time)
{
    prescalar = 1;
    timerStart(cellularOpertionAotimer,time);

}

void StopcellularOpertionAoTimer()
{
    timerStop(cellularOpertionAotimer);

}


void waitForSecond(uint16_t sect)
{
    SleepCounter=0;
    SleepTime= (sect* prescalar);
    SleepTimerOn=true;

    startWaitTimer = true;
    while(SleepTimerOn);
}


void startModemTimer(uint16_t time)
{


    ModemtimerCounter=0;
    ModemtimerTimeOut=false;
    ModemtimerTime= (time * prescalar);
    ModemtimerOn=true;
    StartModemtimerflag =  true;

}

void stopModemTimer()
{
    ModemtimerTimeOut=false;
    ModemtimerOn=false;
    StartModemtimerflag =  false;

}

void StartcellularMAxTimer(uint16_t time)
{

    cellularMAxCounter=0;
    cellularMAxTimeout=false;
    cellularMAxTime= (time *prescalar);
    cellularMAxTimerOn=true;
    StartcellularMAxTimerflag = true;

}

void StopcellularMAxTimer()
{

    cellularMAxTimeout=false;
    cellularMAxTimerOn=false;
    StartcellularMAxTimerflag = false;


}


void cellularBufferTimerCb()
{

        if(celMATCmd)
        {
            celMBufReady = true;

        }
        else
        {
            celMUsolicRes = true;

        }

        StopcellularMAxTimer();
        StartcellularMAxTimerflag = false;

}

bool cellularBufferTimerInit()
{
    bool rval = 0;

    rval = timerInit(cellularBufferTimer, mode_msec, cellularBufferTimerCb);

    return rval;
}

void startCellularBufferTimer(uint16_t timein_ms)
{
    //    cellularBufferTime = timein_ms;
    //    cellularBufferTimerCounter = 0;
    //    cellularBufferTimerOn = true;
    //    cellularBufferTimeout = false;
    //    StartBufferTimerflag = true;
    timerStop(cellularBufferTimer);
    timerStart(cellularBufferTimer, timein_ms);
}


char modemReadyRecv;
bool ConfigureModemPins()
{
    gpioSetup(cellMPowerPort,cellMPowerPin, GPIO_OUTPUT,0,0,0);
    gpioSetup(cellMStatusPort,cellMStatusPin, GPIO_INPUT,0,0,0);
    return 1;
}



uint8_t turnOnCellular()
{
    int8_t Pin_value =0;

    Pin_value = gpioGet(cellMStatusPort,cellMStatusPin);
    if(Pin_value)
    {
#ifdef Debug_Enable
        debugStr("\ncm:Cellular Modem is already on");
#endif
        modemReadyRecv = 1;

        return 1;
    }
    else
    {
#ifdef Debug_Enable
        debugStr("\ncm:Cellular Modem is OFF.Turnnig On the Modem");
#endif
        gpioSetHigh(cellMPowerPort,cellMPowerPin);
        waitForSecond(2);
        gpioSetLow(cellMPowerPort,cellMPowerPin);
        waitForSecond(1);

    }
    return 1;

}

uint8_t turnOffCellular()
{
    volatile int8_t Pin_value =0;

    Pin_value = gpioGet(cellMStatusPort,cellMStatusPin);
    if(Pin_value)
    {

#ifdef Debug_Enable
        debugStr("\ncm:Cellular Modem is turning off");
#endif

        gpioSetHigh(cellMPowerPort,cellMPowerPin);
        waitForSecond(3);
        gpioSetLow(cellMPowerPort,cellMPowerPin);
        waitForSecond(2);

        cellularMAxTimeout =  false;
        modemReadyRecv = 0;

        celMATCmd = false;
        StartcellularMAxTimer(30);



        while(!(cellularMAxTimeout))
        {
            Pin_value = gpioGet(cellMStatusPort,cellMStatusPin);
            if(Pin_value)
            {
                continue;
            }
            else
            {
#ifdef Debug_Enable
                debugStr("\ncm:Cellular Modem is OFF");
#endif
                StopcellularMAxTimer();
                break;
            }
        }

        if(cellularMAxTimeout)
        {
#ifdef Debug_Enable
            debugStr("\ncm:Cellular Modem turn OFF timeout occurred");
#endif

        }


    }
    return 1;
}




uint16_t powertheModem()
{
    unsigned int rval = 0xff;

    cellularMAxTimeout =  false;
    modemReadyRecv = 0;

    turnOnCellular();
    celMATCmd = false;
    //    debugStr("J");
    StartcellularMAxTimer(30);

    while(!(cellularMAxTimeout || modemReadyRecv))
    {

        if(celMUsolicRes)
        {

            celMUsolicRes = 0;
            rval = cellularModemReady;
            unsolicCellularResponse();
            waitForSecond(2);

        }
    }

    if(modemReadyRecv)
    {

        rval = cellularModemReady;
        waitForSecond(1);
#ifdef Debug_Enable
        debugStr("\ncm:Modem is Ready");
#endif

    }
    else
        if(cellularMAxTimeout)
        {
            rval = cellularModemnotReady;
        }
    StopcellularMAxTimer();

    return rval;
}

uint8_t Restart_Modem_State = 1;

uint16_t restarttheModem()
{
    uint16_t rval=0xff;
    char Pin_value =0xff;

    switch(Restart_Modem_State)
    {
    case 1:

#ifdef Debug_Enable
        debugStr("\ncm:Restart Modem Initiate");
#endif

        Pin_value = gpioGet(cellMStatusPort,cellMStatusPin);
        if(Pin_value)
        {
            gpioSetHigh(cellMPowerPort,cellMPowerPin);
            waitForSecond(3);
            gpioSetLow(cellMPowerPort,cellMPowerPin);
            waitForSecond(2);

            ModemtimerTimeOut =  false;
            modemReadyRecv = false;
            celMATCmd = false;
            startModemTimer(30);



        }
        rval = Success;
        break;
    case 2:

        if(ModemtimerTimeOut)
        {
#ifdef Debug_Enable
            debugStr("\ncm:Restarting Modem Off timeout occured");
#endif
            resetSystem();

        }
        else
        {
            Pin_value = gpioGet(cellMStatusPort,cellMStatusPin);
            if(Pin_value)
            {

            }
            else
            {
#ifdef Debug_Enable
                debugStr("\ncm:Restarting Modem is OFF");
#endif
                rval = Success;
                waitForSecond(2);
                stopModemTimer();
            }
        }

        break;
    case 3:
        ModemtimerTimeOut =  false;
        modemReadyRecv = false;

        turnOnCellular();
        celMATCmd = false;
        startModemTimer(30);
        rval = Success;

        break;
    case 4:
        if(modemReadyRecv)
        {
            if(celMUsolicRes)
            {
                celMUsolicRes = 0;
                rval = cellularModemReady;
                unsolicCellularResponse();
                waitForSecond(2);
#ifdef Debug_Enable
                debugStr("\ncm:Restarting Modem is ON");
#endif
                stopModemTimer();
            }
        }
        else
            if(ModemtimerTimeOut)
            {
                rval = cellularModemnotReady;
#ifdef Debug_Enable
                debugStr("cm:4G Modem unable to start.Device is restarting..\n");
#endif

                resetSystem();
            }

        break;

    default:
        rval = cellularModemRestartSuccess;
        Restart_Modem_State = 0;
#ifdef Debug_Enable
        debugStr("\ncm:Restarting Modem is Done");
#endif

    }
    checkReturnValue(&Restart_Modem_State,&rval);

    return rval;
}

/*********************************************************************************************************************************
 *                                                          Conversion API
 *********************************************************************************************************************************/
char *strnstr( char *src, char *dest,unsigned int n)
{
    char *s=src;
    char *d=dest;
    char cp,*temp,matched=0;

    n+=1;
    while(n)
    {
        temp=s;
        d=dest;
        do
        {
            if((*s - (cp =*d)))
            {
                break;
            }
            else
            {
                matched=1;
                s++;d++;
                n--;
            }

        }while (cp && n);

        if(cp == 0 )
            return temp;

        if(matched == 0)
        {
            s++; n--;
        }
        else
            matched=0;

    }

    return 0;
}


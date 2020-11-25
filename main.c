#include <msp430.h> 
#include "iotAPP.h"
#include "Application/rtc/internalRtc.h"
#include "Application/Application.h"

/**
 * main.c
 */

char powerStatmain = 0xff;
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;
	PM5CTL0 &= ~LOCKLPM5;
	__bis_SR_register(GIE);

	/*=======================================================
	 * SOC initialised
	 ========================================================*/
	 TA1CCTL0 = 0x0000;
	 TA1CCTL1 = 0x0000;
	 TA1CCTL2 = 0x0000;
	 TA1CTL = 0x0000;
	 TA1CCR0 = 0x0000;
	 TA1CCR1 = 0x0000;
	 TA1CCR2 = 0x0000;

	Init_Clock();
	watchdogInit();
	debugUartInit();

	/*==============================================================
	 * Welcome Message
	 ==============================================================*/
    debugLog("/**************************************************/\n");
    debugLog("Project    : IOT Application SDK \n");
    debugLog("Version    : ");
    debugLog(Project_Firmware_Version);
    debugLog("  \n");
    debugLog("Develop By : Gaurav Choubey\n\n");
    debugLog("/**************************************************/\n");

    powerModuleInit();
    char tmp[50]={0};
    memset(tmp,0x00,sizeof(tmp));
    powerStatmain = powerModuleStatus(tmp);
//    debugLog(tmp);

    if(powerStatmain == LOW_POWER_MODE)
    {

    }
    else if(powerStatmain == BACKUP_POWER_MODE)
    {

    }

    getPowerUpReason();
	rtcInit(applicationSyncCnt,&lowPowerIndication);

	/*==========================
	 * application initialized
	 ===========================*/
     applicationInit();

	/*============================
	 * iot app initialized
	 *===========================*/
	iotAppInit();

	while(1)
	{
        WDT_A_resetTimer(WDT_A_BASE);
        applicationPacketFormTask();
	    iotAppRun(0);


//	    memset(tmp,0x00,sizeof(tmp));
//	    powerStatmain = powerModuleStatus(tmp);
//	    debugLog(tmp);
//	    debugLog("\n");
//
//	    __delay_cycles(16000000);
	}

	
}

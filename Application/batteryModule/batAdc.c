/*
 * controllerAdcLL.c
 *
 *  Created on: Sep 19, 2020
 *      Author: Gcsystem
 */
#include <Application/batteryModule/batAdc.h>
#include <msp430.h>
#include "driverlib.h"

char batAvgCnt = 0;
unsigned long batRawValue = 0;

char mainsAvgCnt = 0;
unsigned long mainsRawValue = 0;

void adcReferenceInit()
{
    Ref_A_setReferenceVoltage(REF_A_BASE, REF_A_VREF2_5V);

    Ref_A_enableReferenceVoltage(REF_A_BASE);
}

void batAdcInit()
{
    adcReferenceInit();


    ADC12_B_initParam initParam = {0};
    initParam.sampleHoldSignalSourceSelect = ADC12_B_SAMPLEHOLDSOURCE_SC;
    initParam.clockSourceSelect = ADC12_B_CLOCKSOURCE_ADC12OSC;
    initParam.clockSourceDivider = ADC12_B_CLOCKDIVIDER_1;
    initParam.clockSourcePredivider = ADC12_B_CLOCKPREDIVIDER__1;
    initParam.internalChannelMap = ADC12_B_NOINTCH;
    ADC12_B_init( ADC12_B_BASE, &initParam );

    //Enable the ADC12B module
    ADC12_B_enable( ADC12_B_BASE );

    ADC12_B_setupSamplingTimer(ADC12_B_BASE, ADC12_B_CYCLEHOLD_16_CYCLES, ADC12_B_CYCLEHOLD_16_CYCLES, ADC12_B_MULTIPLESAMPLESENABLE);



}


unsigned int batConfiguration(char powerType,char batport,char batpin)
{


    switch(powerType)
    {
    case MainsBat:
    {
        GPIO_setAsPeripheralModuleFunctionOutputPin(MAINS_PORT ,MAINS_PIN, GPIO_TERNARY_MODULE_FUNCTION);

        ADC12_B_configureMemoryParam configureMemoryParam = {0};
        configureMemoryParam.memoryBufferControlIndex   =       ADC12_B_MEMORY_0;
        configureMemoryParam.inputSourceSelect          =       ADC12_B_INPUT_A7;
        configureMemoryParam.refVoltageSourceSelect     =       ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
        configureMemoryParam.endOfSequence              =       ADC12_B_ENDOFSEQUENCE;
        configureMemoryParam.windowComparatorSelect     =       ADC12_B_WINDOW_COMPARATOR_DISABLE;
        configureMemoryParam.differentialModeSelect     =       ADC12_B_DIFFERENTIAL_MODE_DISABLE;

        ADC12_B_configureMemory(ADC12_B_BASE, &configureMemoryParam);

        ADC12_B_clearInterrupt(ADC12_B_BASE, 0, ADC12_B_IFG0);

        ADC12_B_enableInterrupt(ADC12_B_BASE, ADC12_B_IE0,0, 0);


    }break;

    case BackupBat:
    {
        GPIO_setAsPeripheralModuleFunctionOutputPin(BAT_PORT ,BAT_PIN, GPIO_TERNARY_MODULE_FUNCTION);

        ADC12_B_configureMemoryParam configureMemoryParam = {0};
        configureMemoryParam.memoryBufferControlIndex   =       ADC12_B_MEMORY_1;
        configureMemoryParam.inputSourceSelect          =       ADC12_B_INPUT_A4;
        configureMemoryParam.refVoltageSourceSelect     =       ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
        configureMemoryParam.endOfSequence              =       ADC12_B_ENDOFSEQUENCE;
        configureMemoryParam.windowComparatorSelect     =       ADC12_B_WINDOW_COMPARATOR_DISABLE;
        configureMemoryParam.differentialModeSelect     =       ADC12_B_DIFFERENTIAL_MODE_DISABLE;

        ADC12_B_configureMemory(ADC12_B_BASE, &configureMemoryParam);

        ADC12_B_clearInterrupt(ADC12_B_BASE, 0, ADC12_B_IFG1);

        ADC12_B_enableInterrupt(ADC12_B_BASE, ADC12_B_IE1,0, 0);

    }break;

    }

return 1;
}

void batVoltgeMeasureStart(char powerType)
{
//    batRawValue = 0;
    ADC12_B_enable( ADC12_B_BASE );
    while(ADC12_B_isBusy(ADC12_B_BASE));
    if(powerType == MainsBat)
        ADC12_B_startConversion(ADC12_B_BASE, ADC12_B_START_AT_ADC12MEM0, ADC12_B_SINGLECHANNEL);
    else if(powerType == BackupBat)
        ADC12_B_startConversion(ADC12_B_BASE, ADC12_B_START_AT_ADC12MEM1, ADC12_B_SINGLECHANNEL);

    batMeasuringInProgress = 1;
}

float batVoltageRead(char powerType)
{

    if(powerType == BackupBat)
    {
        batAvgCnt = 0;
        batRawValue = 0;
        while(++batAvgCnt <= BAT_AVG_CNT)
        {
            batVoltgeMeasureStart(BackupBat);
            while(batMeasuringInProgress);

        }
        batRawValue /=  BAT_AVG_CNT;
        return     batRawValue* 0.001468;//0.0014676; //0.001025;//0.00061035;  //(batRawValue*(2.5/4096));
    }
    else if(powerType == MainsBat)
    {

        mainsAvgCnt = 0;
        mainsRawValue = 0;

        while(++mainsAvgCnt <= MAINS_AVG_CNT)
        {
            batVoltgeMeasureStart(MainsBat);
            while(batMeasuringInProgress);

        }

        mainsRawValue /=  MAINS_AVG_CNT;
        return     mainsRawValue* 0.001098;//0.0014676; //0.001025;//0.00061035;  //(batRawValue*(2.5/4096));

    }



    //    batVoltgeMeasureStart();
    //    while(batMeasuringInProgress);
    //    ADC12_B_disableConversions(ADC12_B_BASE, ADC12_B_COMPLETECONVERSION);
    //    //        ADC12_B_disable(ADC12_B_BASE);
    //    batRawValue /=  BAT_AVG_CNT;
    //    return     batRawValue* 0.00061035;  //(batRawValue*(2.5/4096));

return 0;
}


#pragma vector=ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    switch(__even_in_range(ADC12IV,76))
    {
    case  0: break;                         // Vector  0:  No interrupt
    case  2: break;                         // Vector  2:  ADC12BMEMx Overflow
    case  4: break;                         // Vector  4:  Conversion time overflow
    case  6: break;                         // Vector  6:  ADC12BHI
    case  8: break;                         // Vector  8:  ADC12BLO
    case 10: break;                         // Vector 10:  ADC12BIN
    case 12:                                // Vector 12:  ADC12BMEM0 Interrupt
        //        if((batMeasuringInProgress) &&(++batAvgCnt <= BAT_AVG_CNT))
        //        {
        //            batRawValue += ADC12_B_getResults(ADC12_B_BASE, ADC12_B_MEMORY_0);
        //        }
        //        else
        //        {
        //            ADC12_B_getResults(ADC12_B_BASE, ADC12_B_MEMORY_0);
        //            batAvgCnt = 0;
        //            batMeasuringInProgress = 0;
        //            ADC12_B_disable(ADC12_B_BASE);
        //
        //        }


        mainsRawValue += ADC12_B_getResults(ADC12_B_BASE, ADC12_B_MEMORY_0);
        batMeasuringInProgress = 0;

        ADC12_B_clearInterrupt(ADC12_B_BASE, 0, ADC12_B_IFG0);



        break;
    case 14:// Vector 14:  ADC12BMEM1
        batRawValue += ADC12_B_getResults(ADC12_B_BASE, ADC12_B_MEMORY_1);
        batMeasuringInProgress = 0;

        ADC12_B_clearInterrupt(ADC12_B_BASE, 0, ADC12_B_IFG1);

        break;
    case 16: // Vector 16: ADC12BMEM2

        break;
    case 18:// Vector 18:  ADC12BMEM3

        break;
    case 20: // Vector 20:  ADC12BMEM4

        break;
    case 22: break;                         // Vector 22:  ADC12BMEM5
    case 24: break;                         // Vector 24:  ADC12BMEM6
    case 26: break;                         // Vector 26:  ADC12BMEM7
    case 28: break;                         // Vector 28:  ADC12BMEM8
    case 30: break;                         // Vector 30:  ADC12BMEM9
    case 32: break;                         // Vector 32:  ADC12BMEM10
    case 34: break;                         // Vector 34:  ADC12BMEM11
    case 36: break;                         // Vector 36:  ADC12BMEM12
    case 38: break;                         // Vector 38:  ADC12BMEM13
    case 40: break;                         // Vector 40:  ADC12BMEM14
    case 42: break;                         // Vector 42:  ADC12BMEM15
    case 44: break;                         // Vector 44:  ADC12BMEM16
    case 46: break;                         // Vector 46:  ADC12BMEM17
    case 48: break;                         // Vector 48:  ADC12BMEM18
    case 50: break;                         // Vector 50:  ADC12BMEM19
    case 52: break;                         // Vector 52:  ADC12BMEM20
    case 54: break;                         // Vector 54:  ADC12BMEM21
    case 56: break;                         // Vector 56:  ADC12BMEM22
    case 58: break;                         // Vector 58:  ADC12BMEM23
    case 60: break;                         // Vector 60:  ADC12BMEM24
    case 62: break;                         // Vector 62:  ADC12BMEM25
    case 64: break;                         // Vector 64:  ADC12BMEM26
    case 66: break;                         // Vector 66:  ADC12BMEM27
    case 68: break;                         // Vector 68:  ADC12BMEM28
    case 70: break;                         // Vector 70:  ADC12BMEM29
    case 72: break;                         // Vector 72:  ADC12BMEM30
    case 74: break;                         // Vector 74:  ADC12BMEM31
    case 76: break;                         // Vector 76:  ADC12BRDY
    default: break;
    }
}


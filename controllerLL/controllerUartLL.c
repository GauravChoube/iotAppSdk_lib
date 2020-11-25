/*
 * cotrollerUartLL.c
 *
 *  Created on: May 6, 2020
 *      Author: gaurav
 */
#include "controllerUartLL.h"
//#include "../IoTApp/Config.h"
//#include "../cellularModule/coreCellularModule/cellularModuleHal.h"
//#include "../cellularModule/coreCellularModule/cellularModuleLL.h"

void (*cbUart0App)(int8_t *,uint16_t);
void (*cbUart1App)(int8_t *,uint16_t);


int8_t *RxU1Buff;
uint16_t RxU1BuffLen;

int8_t *RxU0Buff;
uint16_t RxU0BuffLen;




#define uartClock       UART_CLOCK_SMCLK
#define uartClockFreq   UART_CLOCK_SOURC_FREQ_16MHz

#pragma vector = USCI_A0_VECTOR
__interrupt void UART0_ISR(void)
{


    switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
    {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
    {
//        UCA1TXBUF = UCA0RXBUF;
        RxU0Buff[0] = UCA0RXBUF;
        cbUart0App(RxU0Buff,1);

    }
    break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
    }

}

#pragma vector = USCI_A1_VECTOR
__interrupt void UART1_ISR(void)
{

    switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
    {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
    {
        RxU1Buff[0] = UCA1RXBUF;
        cbUart1App(RxU1Buff,1);

//        if(GSM_Buffer_Index < GSM_Buffer_Size)
//        {
//            GSM_Buffer[GSM_Buffer_Index++]= ch;
//        }
////        else
////        {
////
////            if(GSM_Buffer_Excced == false)
////            {
////
////                TA0CCR0 = 4096;
////
////                GSM_Buffer_Excced = true;
////                tempsendAddr = Extended_GSMBuffer_SA;
////                SingleBytewrite(tempsendAddr++, ch);
////            }
////            else
////            {
////                if(tempsendAddr < Extended_GSMBuffer_EA )
////                    SingleBytewrite(tempsendAddr++, ch);
////            }
////
////        }
//        TA0CTL |=MC_3;;
//        Start_Buffering=false;

    }
   break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
}

}




bool uartInit(uint8_t uartSelection,uint32_t baudRate,void (*funcptr)(int8_t *,uint16_t),int8_t *uartRxBuf,uint16_t uartRxbuflen)
{
    //INITIALISATION OF HARDWARE SETTINGS FOR UARTA0 RX0 & TX0 PINS
    if(uartSelection==uart0)
    {
        P2SEL1 |= BIT0 | BIT1;
        P2SEL0 &= ~(BIT0 | BIT1);
        UCA0CTLW0 = UCSWRST;   // Put eUSCI in reset

        switch(baudRate)
        {
        case 115200:
        {

            switch(uartClock)
            {
            case UART_CLOCK_ACLK: return 0;
            case UART_CLOCK_SMCLK:
            {
                UCA0CTL1 |= UCSSEL__SMCLK;
                switch(uartClockFreq)
                {
                case UART_CLOCK_SOURC_FREQ_16MHz:
                {
                    UCA0BRW = 8;
                    UCA0MCTLW = 0xF7A1;

                }break;
                case UART_CLOCK_SOURC_FREQ_8MHz:
                {
                    UCA0BRW = 4;                              // 8000000/115200 = 69.444   UCA0BRW=69.44/16=4  when U016=1
                    UCA0MCTLW = 0x5551;

                }break;
                case UART_CLOCK_SOURC_FREQ_1MHz:
                {
                    UCA0BRW = 8;                              // 8000000/115200 = 69.444   UCA0BRW=69.44/16=4  when U016=1
                    UCA0MCTLW = 0xD600;

                }break;

                }

            }break;

            }

        }break;

        case 9600:
        {

            switch(uartClock)
            {
            case UART_CLOCK_ACLK:
                UCA0CTL1 |= UCSSEL__ACLK;

                UCA0BRW =3;
                UCA0MCTLW = 0x9200;

                break;
            case UART_CLOCK_SMCLK:
            {
                UCA0CTL1 |= UCSSEL__SMCLK;
                switch(uartClockFreq)
                {
                case UART_CLOCK_SOURC_FREQ_16MHz:
                {
                    UCA0BRW = 104;
                    UCA0MCTLW = 0xD621;

                }break;
                case UART_CLOCK_SOURC_FREQ_8MHz:
                {
                    UCA0BRW = 52;                              // 8000000/115200 = 69.444   UCA0BRW=69.44/16=4  when U016=1
                    UCA0MCTLW = 0x4911;

                }break;
                case UART_CLOCK_SOURC_FREQ_1MHz:
                {
                    UCA0BRW = 6;                              // 8000000/115200 = 69.444   UCA0BRW=69.44/16=4  when U016=1
                    UCA0MCTLW = 0x2081;

                }break;

                }
            }


            }

        }

        }

        UCA0CTL1 &= ~UCSWRST;
        UCA0IE |= UCRXIE;

        cbUart0App = funcptr;
        RxU0Buff =  uartRxBuf;
        RxU0BuffLen = uartRxbuflen;

    }
    else
    {

        P2SEL1 |= BIT5 | BIT6;
        P2SEL0 &= ~(BIT5 | BIT6);
        UCA1CTLW0 = UCSWRST;

        switch(baudRate)
        {
        case 115200:
        {

            switch(uartClock)
            {
            case UART_CLOCK_ACLK:return 0;
            case UART_CLOCK_SMCLK:
            {
                UCA1CTL1 |= UCSSEL__SMCLK;
                switch(uartClockFreq)
                {
                case UART_CLOCK_SOURC_FREQ_16MHz:
                {
                    UCA1BRW = 8;
                    UCA1MCTLW = 0xF7A1;

                }break;
                case UART_CLOCK_SOURC_FREQ_8MHz:
                {
                    UCA1BRW = 4;                              // 8000000/115200 = 69.444   UCA0BRW=69.44/16=4  when U016=1
                    UCA1MCTLW = 0x5551;

                }break;
                case UART_CLOCK_SOURC_FREQ_1MHz:
                {
                    UCA1BRW = 8;                              // 8000000/115200 = 69.444   UCA0BRW=69.44/16=4  when U016=1
                    UCA1MCTLW = 0xD600;

                }break;

                }

            }break;

            }

        }break;

        case 9600:
        {

            switch(uartClock)
            {
            case UART_CLOCK_ACLK:
                UCA1CTL1 |= UCSSEL__ACLK;

                UCA1BRW =3;
                UCA1MCTLW = 0x9200;

                break;
            case UART_CLOCK_SMCLK:
            {
                UCA1CTL1 |= UCSSEL__SMCLK;
                switch(uartClockFreq)
                {
                case UART_CLOCK_SOURC_FREQ_16MHz:
                {
                    UCA1BRW = 104;
                    UCA1MCTLW = 0xD621;

                }break;
                case UART_CLOCK_SOURC_FREQ_8MHz:
                {
                    UCA1BRW = 52;                              // 8000000/115200 = 69.444   UCA0BRW=69.44/16=4  when U016=1
                    UCA1MCTLW = 0x4911;

                }break;
                case UART_CLOCK_SOURC_FREQ_1MHz:
                {
                    UCA1BRW = 6;                              // 8000000/115200 = 69.444   UCA0BRW=69.44/16=4  when U016=1
                    UCA1MCTLW = 0x2081;

                }break;

                }
            }


            }

        }

        }
        UCA1CTL1 &= ~UCSWRST;
        UCA1IE |=UCRXIE;

        cbUart1App = funcptr;
        RxU1Buff = uartRxBuf;
        RxU1BuffLen = uartRxbuflen;


    }

    return 1;
}

bool uartReinit(uint8_t uartSelection,uint32_t baudrate)
{

    return 1;
}



void Tx_Uart0_Char(unsigned char Tx_Data)
{
    // Check for empty tx buffer Status
    while(!(UCA0IFG & UCTXIFG));
    // Write 8-bit data from string to Transmit Buffer Register:UCA0TXBUF
    UCA0TXBUF = Tx_Data;
}

/*TRANSMITING_STRING_FUCTION*/
//*****************************************************************************
//
//! \brief Transmits a string from the UART Module.
//!
//! This function will place the supplied data into UART transmit data register
//! to start transmission
//!
//!\param char *Str String to be transmitted from the UART module
//!
//! \return None
//
//*****************************************************************************
void Tx_Uart0_String(char * Str)
{
    while(*Str!= '\0')
    {
        Tx_Uart0_Char(*Str);
        Str++;
    }
}

/*TRANSMIT_DATA_CHAR_FUNCTION*/
//*****************************************************************************
//
//! \brief Transmits a byte from the UART Module.
//!
//! This function will place the supplied data into UART transmit data register
//! to start transmission
//!
//!\param char Tx_Data data to be transmitted from the UART module
//!
//! \return None
//
//*****************************************************************************
void Tx_Uart1_Char(unsigned char Tx_Data)
{
    // Check for empty tx buffer Status
    while(!(UCA1IFG & UCTXIFG));
    // Write 8-bit data from string to Transmit Buffer Register:UCA0TXBUF
    UCA1TXBUF = Tx_Data;
}

/*TRANSMITING_STRING_FUCTION*/
//*****************************************************************************
//
//! \brief Transmits a string from the UART Module.
//!
//! This function will place the supplied data into UART transmit data register
//! to start transmission
//!
//!\param char *Str String to be transmitted from the UART module
//!
//! \return None
//
//*****************************************************************************
void Tx_Uart1_String(char * Str)
{
    while(*Str!= '\0')
    {
        Tx_Uart1_Char(*Str);
        Str++;
    }
}




void uartWrite(int8_t uartSelection,int8_t *Buff,uint16_t Bufflen)
{
    switch(uartSelection)
         {
         case uart0:
             if(Bufflen == 1)
             {
                 Tx_Uart0_Char(*Buff);
             }
             else
             {
                 Tx_Uart0_String((char *)Buff);
             }

             break;
         case uart1:

             if(Bufflen == 1)
             {
                 Tx_Uart1_Char(*Buff);
             }
             else
             {
                 Tx_Uart1_String((char *)Buff);
             }

             break;
         }
}

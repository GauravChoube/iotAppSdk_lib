/*
 * cotrollerUartLL.h
 *
 *  Created on: May 6, 2020
 *      Author: gaurav
 */

#ifndef MAIN_CONTROLLERLL_CONTROLLERUARTLL_H_
#define MAIN_CONTROLLERLL_CONTROLLERUARTLL_H_

#include <msp430.h>
#include "driverlib.h"

#define UART_CLOCK_SMCLK        1
#define UART_CLOCK_ACLK         0

#define UART_CLOCK_SOURC_FREQ_16MHz   0x10
#define UART_CLOCK_SOURC_FREQ_8MHz    0x11
#define UART_CLOCK_SOURC_FREQ_1MHz   0x12
#define UART_CLOCK_SOURC_FREQ_32KHz  0x13



#define uart0          0

#define UART0_TX_PIN   BIT0
#define UART0_RX_PIN   BIT1

#define uart1          1

#define UART1_TX_PIN  BIT5
#define UART1_RX_PIN  BIT6


//#define uart2



void uart_event_task(void *pvParameters);
bool uartInit(uint8_t uartSelection,uint32_t baudRate,void (*funcptr)(int8_t *,uint16_t),int8_t *uartRxBuf,uint16_t uartRxbuflen);
bool uartReinit(uint8_t uartSelection,uint32_t baudrate);
void uartWrite(int8_t uartSelection,int8_t *Buff,uint16_t Bufflen);


#endif /* MAIN_CONTROLLERLL_CONTROLLERUARTLL_H_ */

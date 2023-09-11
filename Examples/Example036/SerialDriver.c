/**
 * MIT License
 *
 * Copyright (c) 2019 Brian Amos
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "SerialDriver.h"
#include "FreeRTOS.h"
#include <task.h>
#include <semphr.h>
#include "CMSDK_CM3.h"


/**
 * txBuffLen and rxBuffLen should be at least as large as the lengths defined in usbd_cdc_if.c
 * to avoid dropped data
 * if multiple transfers should be placed into vcom_rxStream before being read
 * by the application, the streamBuffers should be larger
 *
 * The way usbd_cdc_if.c is written, any newly received data not fitting into the stream
 * buffer is dropped
 **/
#define txBuffLen 1024
#define rxBuffLen 1024

uint8_t vcom_usbTxBuff[txBuffLen];
StreamBufferHandle_t rxStream = NULL;
StreamBufferHandle_t vcom_txStream = NULL;
TaskHandle_t vcom_usbTaskHandle = NULL;
SemaphoreHandle_t vcom_mutexPtr = NULL;

/********************************** PUBLIC *************************************/

/**
 * Initialize the USB peripheral and HAL-based USB stack.
 * A transmit task, responsible for pulling data out of the stream buffer and
 * pushing it into the USB peripheral is also created.
 * @param UsbStackSize	size (in FreeRTOS words) of the stack to be used for
 * 						the usbTxTask (256 is tested)
 * @param UsbTxPriority Priority with wich the USB task will be created
**/
void SerialInit( void )
{
	vcom_txStream = xStreamBufferCreate( txBuffLen, 1);
	rxStream  = xStreamBufferCreate( rxBuffLen, 1);
	configASSERT( vcom_txStream != NULL);
	configASSERT( rxStream != NULL);
}

/**
 * return the streamBuffer handle
 * This is wrapped in a function rather than exposed directly
 * so external modules aren't able to change where the handle points
 *
 * NOTE:	This return value will not be valid until VirtualCommInit has
 * 			been run
 */
StreamBufferHandle_t const* GetSerialRxStreamBuff( void )
{
	return &rxStream;
}
static uint8_t rxSendBuff[20];
static uint_fast16_t rxltr = 0;
/*-----------------------------------------------------------*/
uint32_t UART0_RX_IRQHANDLER(void)
{
	BaseType_t xHigherPriorityTaskWoken;
    const uint_fast16_t  xBytesToSend = 9;

	/* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
	it will get set to pdTRUE inside the interrupt safe API function if a
	context switch is required. */
	xHigherPriorityTaskWoken = pdFALSE;

	if(CMSDK_UART0->INTSTATUS & CMSDK_UART_INTSTATUS_RXIRQ_Msk) {

		//read the data register unconditionally to clear
		//the receive not empty interrupt if no reception is
		//in progress
		uint8_t tempVal = (uint8_t) CMSDK_UART0->DATA;

		rxSendBuff[rxltr++] = tempVal;
		printf("%d ", rxltr);

		if(rxltr >= xBytesToSend)
		{
			xStreamBufferSendFromISR(rxStream,
									( const void * ) rxSendBuff,
									xBytesToSend ,
									&xHigherPriorityTaskWoken);
			rxltr = 0;
		}


		/* Clear receive interrupt status register */
		CMSDK_UART0->INTCLEAR |= CMSDK_UART_INTSTATUS_RXIRQ_Msk;

	}
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

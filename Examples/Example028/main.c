/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* Standard includes. */
#include <stdbool.h>
#include <string.h>

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Demo includes. */
#include "supporting_functions.h"
#include "hardware_init.h"

/*-----------------------------------------------------------*/

/* A task to read the queue and print received characters */
static void uartPrintOutTask( void *pvParameters );

/*-----------------------------------------------------------*/

static SemaphoreHandle_t rxDone = NULL;

static bool rxInProgress = false;
static uint_fast16_t rxLen = 0;
static uint8_t* rxBuff = NULL;
static uint_fast16_t rxltr = 0;

/*-----------------------------------------------------------*/

int main( void )
{
	//ensure proper priority grouping for freeRTOS
	NVIC_SetPriorityGrouping(0);

	HwInit();

	/* A task to read the queue and print received characters */
	xTaskCreate( uartPrintOutTask, "Key poll", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );

	//setup tasks, making sure they have been properly created before moving on
	rxDone = xSemaphoreCreateBinary();
	configASSERT(rxDone != NULL);

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* As in previous examples, vTaskStartScheduler() shoudl not return so the
	following lines should not be reached. */
	for( ;; );
	return 0;
}
/*-----------------------------------------------------------*/

static void uartPrintOutTask( void *pvParameters )
{
	uint8_t rxData[20];
	uint8_t expectedLen = 16;
	rxBuff = rxData;
	for( ;; )
	{
		rxInProgress = true;
		rxLen = expectedLen;
		memset((void*)rxData, 0, 20);
		rxltr = 0;
		printf("\nWait for 16 character typed within 10 seconds...\n");
		if(xSemaphoreTake(rxDone, pdMS_TO_TICKS(10000)) == pdPASS)
		{
			printf("Received: %s", (char*) rxData);
		}
		else
		{
			printf("E-The sent bytes are not equal with expected length.");
		}
	}
}


/*-----------------------------------------------------------*/
uint32_t UART0_RX_IRQHANDLER(void)
{
	BaseType_t xHigherPriorityTaskWoken;

	/* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
	it will get set to pdTRUE inside the interrupt safe API function if a
	context switch is required. */
	xHigherPriorityTaskWoken = pdFALSE;

	if(CMSDK_UART0->INTSTATUS & CMSDK_UART_INTSTATUS_RXIRQ_Msk) {

		//read the data register unconditionally to clear
		//the receive not empty interrupt if no reception is
		//in progress
		uint8_t tempVal = (uint8_t) CMSDK_UART0->DATA;

		if(rxInProgress)
		{
			rxBuff[rxltr++] = tempVal;
			if(rxltr >= rxLen)
			{
				rxInProgress = false;
				xSemaphoreGiveFromISR(rxDone, &xHigherPriorityTaskWoken);
			}
		}
		/* Clear receive interrupt status register */
		CMSDK_UART0->INTCLEAR |= CMSDK_UART_INTSTATUS_RXIRQ_Msk;

	}
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

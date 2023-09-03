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

/*********************************************
 * A simple demonstration of creating queues
 * using static variables, instad of the FreeRTOS
 * heap
 *********************************************/

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"
#include <queue.h>

/* Demo includes. */
#include "supporting_functions.h"
#include "hardware_init.h"


// some common variables to use for each task
// 128 * 4 = 512 bytes
//(recommended min stack size per task)
#define STACK_SIZE 128

/**
 * setup a simple enum to command the state of a single LED
 *
 * NOTE: only 1 LED can be commanded at a time using this method
 * THE NUMBERIC VALUES ARE IMPORTANT! These will be used by
 * sendingTask to increment the LED states
 */
typedef enum
{
	ALL_OFF	= 	0,
	RED_ON	= 	1,
	RED_OFF = 	2,
	BLUE_ON = 	3,
	BLUE_OFF= 	4,
	GREEN_ON = 	5,
	GREEN_OFF = 6,
	ALL_ON	=	7

}LED_CMDS;

void recvTask( void* NotUsed );
void sendingTask( void* NotUsed );

//this is a handle for the queue that will be used by
//recvTask and sendingTask
static QueueHandle_t ledCmdQueue = NULL;

/**
 * The queue structure is defined here, as a static variable
 * so it can be passed into xQueueCreateStatic.  This additional
 * variable is required because there is no dynamic allocation
 * in xQueueCreateStatic
 */
static StaticQueue_t queueStructure;

/**
 *	define the number of elements in the queue.  This
 *	definition exists to ensure we allocate the same number
 *	of elements in the queueStorage array as we pass into
 *	xQueueCreateStatic
 */
#define LED_CMD_QUEUE_LEN 2

/**
 * storage for the actual queue is also required to be passed
 * into xQueueCreate.  This static variable will exist
 * the entire duration of the program
 */
static uint8_t queueStorage[LED_CMD_QUEUE_LEN];

int main(void)
{
	HwInit();
	BaseType_t retVal;

	//setup tasks, making sure they have been properly created before moving on
	retVal = xTaskCreate(recvTask, "recvTask", STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	configASSERT(retVal == pdPASS);
	retVal = xTaskCreate(sendingTask, "sendingTask", STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
	configASSERT(retVal == pdPASS);

	//create a queue that can store 2 uint8_t's
	//using ledCmdQueue to point to it
	ledCmdQueue = xQueueCreateStatic(LED_CMD_QUEUE_LEN, sizeof(uint8_t), queueStorage, &queueStructure );
	configASSERT(ledCmdQueue != NULL);

	//start the scheduler - shouldn't return unless there's a problem
	vTaskStartScheduler();

	//if you've wound up here, there is likely an issue with overrunning the freeRTOS heap
	while(1)
	{
	}
}

/**
 * This receive task watches a queue for a new ledCmd to be added to it
 */
void recvTask( void* NotUsed )
{
	uint8_t nextCmd = 0;

	while(1)
	{
		if(xQueueReceive(ledCmdQueue, &nextCmd, portMAX_DELAY) == pdTRUE)
		{
			switch(nextCmd)
			{
				case ALL_OFF:
					printf("All LEDS are OFF\n");
				break;
				case GREEN_ON:
					printf("GREEN ON\n");
				break;
				case GREEN_OFF:
					printf("GREEN OFF\n");
				break;
				case RED_ON:
					printf("RED ON\n");
				break;
				case RED_OFF:
					printf("RED OFF\n");
				break;
				case BLUE_ON:
					printf("BLUE ON\n");
				break;
				case BLUE_OFF:
					printf("BLUE OFF\n");
				break;
				case ALL_ON:
					printf("All LEDS are ON\n");
				break;
			}
		}
	}
}

/**
 * sendingTask loops through
 */
void sendingTask( void* NotUsed )
{
	while(1)
	{
		for(int i = 0; i < 8; i++)
		{
			uint8_t ledCmd = (LED_CMDS) i;
			xQueueSend(ledCmdQueue, &ledCmd, portMAX_DELAY);
			vTaskDelay(200/portTICK_PERIOD_MS);
		}
	}
}

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

	/* Pass out a pointer to the StaticTask_t structure in which the Idle task's
	state will be stored. */
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	/* Pass out the array that will be used as the Idle task's stack. */
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	/* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

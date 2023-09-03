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


/*
 * This demo uses heap_1 for dynamic memory allocation, but also includes
 * a "statically" allocated task.  It illustrates
 * that it is possible to vTaskDelete to remove a task from the scheduler
 * that was created using xTaskCreateStatic.
 *
 */

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo includes. */
#include "supporting_functions.h"
#include "hardware_init.h"

/**
 * 	function prototypes
 */
void vTask1(void *argument);
void vTask2(void *argument);


// some common variables to use for each task
// 128 * 4 = 512 bytes
//(recommended min stack size per task)
#define STACK_SIZE 128

StackType_t GreenTaskStack[STACK_SIZE];
StaticTask_t GreenTaskTCB;

int main(void)
{
	uint32_t* mySpace = malloc(sizeof(uint32_t));
	HwInit();

	// use retVal to track the return value of xTaskCreate, issue asserts along the way
	//to stop execution if things don't go as planned
	TaskHandle_t greenHandle = NULL;

	//using an inlined if statement with an infinite while loop to stop in case
	//the task wasn't created successfully
	greenHandle = xTaskCreateStatic(	vTask1, "GreenTask", STACK_SIZE,
										NULL, tskIDLE_PRIORITY + 2,
										GreenTaskStack, &GreenTaskTCB);
	configASSERT( greenHandle != NULL);

	BaseType_t retVal = xTaskCreate(	vTask2, "BlueTask", STACK_SIZE,
											NULL, tskIDLE_PRIORITY + 2, NULL);
	configASSERT( retVal == pdPASS );

	//start the scheduler - shouldn't return unless there's a problem
	vTaskStartScheduler();

	//if you've wound up here, there is likely an issue with overrunning the freeRTOS heap
	while(1)
	{
	}
}

void vTask1(void *argument)
{
	printf("Task 1 is running\r\n");
	vTaskDelay(1500/ portTICK_PERIOD_MS);
	printf("----Task 1 is done----- \r\n");

	/**
	 * in this example, the GreenTask has been created using
	 * "static" memory.  Calling vTaskDelete will remove
	 * the task from being executed by the scheduler, but will not
	 * have any affect on the memory that was allocated for the
	 * task's stack (GreenTaskStack) or TCB (GreenTaskTCB).
	 * After a call to vTaskDelete, that memory can be safety
	 * used for other purposes, it will not longer be accessed by
	 * FreeRTOS
	 **/
	vTaskDelete(NULL);

	//task never get's here
	printf("Task 1 never reach here.\r\n");
}

void vTask2( void* argument )
{
	while(1)
	{
		printf("Task 2 is running\r\n");
		vTaskDelay(200 / portTICK_PERIOD_MS);
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

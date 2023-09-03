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
 * This demo uses heap_1 for dynamic memory allocation.  With heap_1,
 * memory may only be allocated and never freed ( a hang will occur)
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

//define stack and task control block (TCB) for the red task
//static StackType_t RedTaskStack[STACK_SIZE];
//static StaticTask_t RedTaskTCB;

int main(void)
{
	HwInit();

	// use retval to track the return value of xTaskCreate, issue asserts along the way
	//to stop execution if things don't go as planned
	BaseType_t retVal = pdPASS;

	//using an inlined if statement with an infinite while loop to stop in case
	//the task wasn't created successfully
	retVal = xTaskCreate(vTask1, "GreenTask", STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	configASSERT( retVal == pdPASS );

	retVal = xTaskCreate(vTask2, "BlueTask", STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
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
	 * attempting to call vTaskDelete (or any other *Delete function
	 * i.e. vQueueDelete, vEventGroupDelete, vSemaphoreDelete, vMutexDelete, etc)
	 * will result in a hang.
	 * To observe this, uncomment the following line - you'll notice that the
	 * blue LED stops blinking.  Using a debugger, you'll also notice that
	 * the BlueTask and Software Timer task are both stopped in portYIELD_WITHIN_API
	 *
	 * If using heap_1.c  - it is best to adjust the FreeRTOSConfig.h to disable code
	 * for vTaskDelete as follows:
	 * #define INCLUDE_vTaskDelete 0
	 *
	 * This allows the linker to issue an undefined reference error when
	 * vTaskDelete used.  This allows the issue to be found at link time, rather
	 * than run time.
	 **/
//	vTaskDelete(NULL);

	// if a task should not longer run when using heap_1 an infinite
	// delay can be used instead of vTaskDelete
	while(1)
	{
		vTaskDelay(portMAX_DELAY);
	}

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

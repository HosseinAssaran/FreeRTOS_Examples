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

/**
 * This file only references RTOS-implementation independent headers.
 * Any CMSIS-RTOS v2 compliant RTOS should be able to be used
 * in the place of FreeRTOS
 */
/* CMSIS RTOS related include */
#include <cmsis_os2.h>
#include <RTOS_Dependencies.h>

/* Demo includes. */
#include "supporting_functions.h"
#include "hardware_init.h"

/**
 * 	function prototypes
 */
void vTask1(void *argument);
void vTask2(void *argument);
void lookBusy( void );

// some common variables to use for each task
// 512 bytes
//(recommended min stack size per task)
#define STACK_SIZE 512

//define stack the red task
static uint8_t Task2_Stack[STACK_SIZE];

/**
 * since we don't want this code to have direct dependencies on FreeRTOS.h,
 * we can't use sizeof(StaticTask_t) directly - instead TCB_SIZE is declared in
 * RTOS_Dependencies.h
**/
uint8_t Task2_TCB[TCB_SIZE];

int main(void)
{
	void* greenTaskThreadID = NULL;
	void* redTaskThreadID = NULL;
	osStatus_t status;
	HwInit();

	//initialize the kernel - this will setup any required
	//heap regions (if using FreeRTOS heap5)
	status = osKernelInitialize();
	assert(status == osOK);

	//start the green task
	osThreadAttr_t task1ThreadAtrribs = {	.name = "Task1",
											.attr_bits = osThreadDetached,
											.cb_mem = NULL,
											.cb_size = 0,
											.stack_mem = NULL,
											.stack_size = STACK_SIZE,
											.priority = osPriorityNormal,
											.tz_module = 0,
											.reserved = 0};
	greenTaskThreadID = osThreadNew( vTask1, NULL, &task1ThreadAtrribs);
	assert(greenTaskThreadID != NULL);


	//start the red task
	//this shows how to use static allocation for the stack and task control block
	osThreadAttr_t task2ThreadAtrribs =  {	.name = "Task2",
											.attr_bits = osThreadDetached,
											.cb_mem = Task2_TCB,
											.cb_size = TCB_SIZE,
											.stack_mem = Task2_Stack,
											.stack_size = STACK_SIZE,
											.priority = osPriorityNormal,
											.tz_module = 0,
											.reserved = 0};
	redTaskThreadID = osThreadNew( vTask2, NULL, &task2ThreadAtrribs);
	assert(redTaskThreadID != NULL);

	status = osKernelStart();
	assert(status == osOK);

	//if you've wound up here, there is likely an issue with over running the freeRTOS heap
	while(1)
	{
	}
}

void vTask1(void *argument)
{
	( void )argument;
	while(1)
	{
		printf("Task 1 is running\r\n");
		osDelay(200);
	}
}

void vTask2( void* argument )
{
	( void )argument;
	while(1)
	{
		lookBusy();
		printf("Task 2 is running\r\n");
		osDelay(500);
	}
}

void lookBusy( void )
{
	__attribute__ ((unused)) volatile uint32_t dontCare;
	for(int i = 0; i < 50E3; i++)
	{
		dontCare = i % 4;
	}
}


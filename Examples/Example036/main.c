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

///* Standard includes. */
//#include <stdbool.h>
//#include <string.h>
//
///* FreeRTOS.org includes. */
//#include "FreeRTOS.h"
//#include "task.h"
//#include "semphr.h"
//#include "stream_buffer.h"

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <string.h>
#include <stdio.h>
#include <pwmImplementation.h>
#include <ledCmdExecutor.h>
#include <CRC32.h>
#include <SerialDriver.h>

/* Demo includes. */
#include "supporting_functions.h"
#include "hardware_init.h"

static void vKeyHitTask( void *pvParameters );


// some common variables to use for each task
// 128 * 4 = 512 bytes
//(recommended min stack size per task)
#define STACK_SIZE 128

/**
 * the protocolTask is responsible for watching the stream buffer
 * receiving data from the PC over USB
 * It validates incoming data, populates the
 * data structure used by the LED command executor
 * and pushes commands into the command executor's queue
 */
void frameDecoder( void* NotUsed);

/**
 * the ledCmdQueue is used to pass data from the protocol decoding task to the
 * LedCmdExecutor
 */
QueueHandle_t ledCmdQueue = NULL;

int main(void)
{
	HwInit();
	PWMInit();
	SerialInit();
	//HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);	//ensure proper priority grouping for freeRTOS

	//create a queue for LedCmd structs, capable of holding up to 4 commands
	ledCmdQueue = xQueueCreate(4, sizeof(LedCmd));
	configASSERT(ledCmdQueue != NULL);

	/**
	 * create a variable that will store the arguments
	 * passed to the LED command executor
	 * this allows the logic in ledCmdExecutor to be completely
	 * independent of the underlying implementations
	 * this is a static variable so it isn't placed on the stack.  We're
	 * using aggregate initialization to guarantee that the iPWM* members won't
	 * change over time
	 */
	static CmdExecArgs ledTaskArgs;
	ledTaskArgs.ledCmdQueue = ledCmdQueue;
	ledTaskArgs.redPWM = &RedPWM;
	ledTaskArgs.greenPWM = &GreenPWM;
	ledTaskArgs.bluePWM = &BluePWM;

	//setup tasks, making sure they have been properly created before moving on
	configASSERT(xTaskCreate(frameDecoder, "frameDecoder", 256, NULL, configMAX_PRIORITIES-2, NULL) == pdPASS);
	configASSERT(xTaskCreate(LedCmdExecution, "cmdExec", 256, &ledTaskArgs, configMAX_PRIORITIES-2, NULL) == pdPASS);
	//xTaskCreate( vKeyHitTask, "Key poll", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

	//start the scheduler - shouldn't return unless there's a problem
	vTaskStartScheduler();

	//if you've wound up here, there is likely an issue with overrunning the freeRTOS heap
	while(1)
	{
	}
}


//static void vKeyHitTask( void *pvParameters )
//{
//const TickType_t xShortDelay = pdMS_TO_TICKS( 100 );
//
//	/* A real application, running on a real target, would probably read button
//	pushes in an interrupt.  That allows the application to be event driven, and
//	prevents CPU time being wasted by polling for key presses when no keys have
//	been pressed.  It is not practical to use real interrupts when using the
//	FreeRTOS Windows port, so this task is created to instead provide the key
//	reading functionality by simply polling the keyboard. */
//	for( ;; )
//	{
//		/* Has a key been pressed? */
//		if( _kbhit() != 0 )
//		{
//			/* Read and discard the key that was pressed. */
//			printf("%x \n", _getch());
//		}
//		/* Don't poll too quickly. */
//		vTaskDelay( xShortDelay );
//	}
//}

/**
 * this task monitors the UBS port, decodes complete frames from the USB Rx StreamBuffer,
 * populates an LedCmd and pushes thecommand into a queue for the LedCmdExecution task to consume
 *
 * The frame consists of a delimiter byte with value 0x02 and then 4 bytes
 * representing the command number, red duty cycle , green duty cycle, and blue duty cycle,
 * To ensure the frame has been correctly detected and
 * received, a 32 bit CRC must be validated before pushing the RGB values into
 * the queue (it comes across the wire little endian)
 *
 * <STX> <Cmd> <red> <green> <blue> <CRC LSB> <CRC> <CRC> <CRC MSB>
 *
 * STX is ASCII start of text (0x02)
 * some useful commnad for test:
 * \x02\x02\x29\x00\x00\x12\x5c\x8f\xcc
 * \x02\x01\xff\xff\xff\x5f\x6f\xa9\x34
 * \x02\x00\x00\x00\x00\x97\x17\x4d\xb8
 */
void frameDecoder( void* NotUsed)
{
	LedCmd incomingCmd;

#define FRAME_LEN 9
	uint8_t frame[FRAME_LEN];

	while(1)
	{
		memset(frame, 0, FRAME_LEN);

		//since this is the only task receiving from the streamBuffer, we don't
		//need to acquire a mutex before accessing it
		//if more than one task was to be receiving, vcom_rxStream would require
		//protection from a mutex

		//first, ensure we received STX (0x02)
		while(frame[0] != 0x02)
		{
			xStreamBufferReceive(	*GetSerialRxStreamBuff(),
									frame,
									1,
									portMAX_DELAY);
			printf("\n get %x", frame[0]);
		}

		//now receive the rest of the frame (hopefully)
		//this won't return until the remainder has bLEDeen received
		xStreamBufferReceive(	*GetSerialRxStreamBuff(),
								&frame[1],
								FRAME_LEN-1,
								portMAX_DELAY);
		for(int i = 1 ; i < FRAME_LEN; i++)
			printf(" %x ", frame[i]);
		printf("\n");

		//if the frame is intact, store it into the command
		//and send it to the ledCmdQueue
		if(CheckCRC(frame, FRAME_LEN))
		{
			//populate the command with current values
			incomingCmd.cmdNum = frame[1];
			incomingCmd.red = frame[2]/255.0 * 100;
			incomingCmd.green = frame[3]/255.0 * 100;
			incomingCmd.blue = frame[4]/255.0 * 100;

			//push the command to the queue
			//wait up to 100 ticks and then drop it if not added...
			xQueueSend(ledCmdQueue, &incomingCmd, 100);
		}
	}
}



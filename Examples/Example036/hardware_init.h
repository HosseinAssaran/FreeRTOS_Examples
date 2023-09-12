
#ifndef _WIN32
#include "CMSDK_CM3.h"
#include "core_cm3.h"
#include "FreeRTOSConfig.h"

#define _getch() uart0_getchar()
/*
 * Printf() output is sent to the serial port.  Initialise the serial hardware.
 */
static void prvUARTInit( void );
static void HwInit(void);
static void enable_uart0_interrupt(void);

static void HwInit(void){
	prvUARTInit();
	//enable_uart0_interrupt();
}
static void prvUARTInit( void )
{
	CMSDK_UART0->BAUDDIV = 16;
    CMSDK_UART0->CTRL |= CMSDK_UART_CTRL_TXEN_Msk | CMSDK_UART_CTRL_RXEN_Msk ;
}

static int _kbhit(void){
    return CMSDK_UART0->STATE & CMSDK_UART_STATE_RXBF_Msk;
}
// Function to receive a character from UART0
static char uart0_getchar() {
    return (char)(CMSDK_UART0->DATA & CMSDK_UART_DATA_Msk); // Read the received character from DATA register
}
 void test_getchar()
 {
	char ch = getchar();
	printf("char : %c\n", ch);
 }

static void enable_uart0_interrupt( void )
 {
	CMSDK_UART0->CTRL |= CMSDK_UART_CTRL_RXIRQEN_Msk | CMSDK_UART_CTRL_RXORIRQEN_Msk; //Enable Interrupt
 	//all 4 bits are for preemption priority -
 	NVIC_SetPriority(UARTRX0_IRQn, configMAX_PRIORITIES - 1);
 	NVIC_EnableIRQ(UARTRX0_IRQn);
 }

#else
static void HwInit(void){}
#endif /* _WIN32 */

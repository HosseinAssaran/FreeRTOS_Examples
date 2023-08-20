
#ifndef _WIN32
#include "SMM_MPS2.h"

/* printf() output uses the UART.  These constants define the addresses of the
required UART registers. */
#define UART0_ADDRESS   ( 0x40004000UL )
#define UART0_DATA              ( * ( ( ( volatile uint32_t * )( UART0_ADDRESS + 0UL ) ) ) )
#define UART0_STATE             ( * ( ( ( volatile uint32_t * )( UART0_ADDRESS + 4UL ) ) ) )
#define UART0_CTRL              ( * ( ( ( volatile uint32_t * )( UART0_ADDRESS + 8UL ) ) ) )
#define UART0_BAUDDIV   ( * ( ( ( volatile uint32_t * )( UART0_ADDRESS + 16UL ) ) ) )
#define TX_BUFFER_MASK  ( 1UL )
#define UARTn_RX_FULL   (0x00000002UL)
#define UARTn_TX_ENABLE (0x00000001UL)
#define UARTn_RX_ENABLE (0x00000002UL)
#define _getch() uart0_getchar()
#define rand() custom_rand()
/*
 * Printf() output is sent to the serial port.  Initialise the serial hardware.
 */
static void prvUARTInit( void );
static void HwInit(void);

static void HwInit(void){
	prvUARTInit();
}
static void prvUARTInit( void )
{
    UART0_BAUDDIV = 16;
	UART0_CTRL = UARTn_TX_ENABLE | UARTn_RX_ENABLE;
}

static int _kbhit(void){
    return UART0_STATE & UARTn_RX_FULL;
}
// Function to receive a character from UART0
static char uart0_getchar() {
    return (char)(UART0_DATA & 0xFF); // Read the received character from DATA register
}
 void test_getchar()
 {
	char ch = getchar();
	printf("char : %c\n", ch);
 }

 int custom_rand(void) {
   /* The rand() function returns a pseudo-random number between 0 and RAND_MAX. */
   static unsigned int seed = 1;
   seed = (seed * 1103515245 + 12345) & RAND_MAX;
   return seed;
 }

void vPortSetInterruptHandler( uint32_t ulInterruptNumber,
	 __attribute__((unused)) uint32_t (*pvHandler)( void ) ) {
	/* No need to add pvHandler dynamically because it is hard coded
	 in isr_vector in startup_gcc. It use in MSVC implementation.*/

	//ensure proper priority grouping for freeRTOS
	NVIC_SetPriorityGrouping(0);

	//Set interrupt priority and enable
	NVIC_SetPriority(ulInterruptNumber, 1);
	NVIC_EnableIRQ(ulInterruptNumber);

	uint32_t IinterruptPriority = NVIC_GetPriority(ulInterruptNumber);
	vPrintStringAndNumber("Priority of Interrupt: ", IinterruptPriority);
}

void vPortGenerateSimulatedInterrupt( uint32_t ulInterruptNumber) {
	 //NVIC_SetPendingIRQ(ulInterruptNumber);
	 SCB->SCR |= SCB_SCR_SEVONPEND_Msk;
	 NVIC->STIR |= ulInterruptNumber;
}

#else
static void HwInit(void){}
#endif /* _WIN32 */

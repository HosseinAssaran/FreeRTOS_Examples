
#ifndef _WIN32
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
//	const TickType_t xShortDelay = pdMS_TO_TICKS( 50 );
//    while (!(UART0_STATE & UARTn_RX_FULL)) {
//        // Wait until Receive FIFO is not empty
//		vTaskDelay( xShortDelay );
//    }
    return (char)(UART0_DATA & 0xFF); // Read the received character from DATA register
}
 void test_getchar()
 {
	char ch = getchar();
	printf("char : %c\n", ch);
 }

#else
static void HwInit(void){}
#endif /* _WIN32 */

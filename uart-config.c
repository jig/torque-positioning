#include <uart.h>

void configUart1(long fcy, long baudrate) {
	unsigned int val_U1BRG = 0;
	unsigned int val_U1MODE = 0;
	unsigned int val_U1STA = 0;

	// reset UART1
	CloseUART1();

	// Pin mapping:
	
	// Map output U1TX to RP0
	/*RPOR0bits.RP0R = 3;
	TRISB &= 0b0111111111111111;
	
	// Max input U1RX to RP1
	RPINR18bits.U1RXR = 1;
	TRISB |= 0b0100000000000000;
*/
	/*
	// Map output U1TX to RP6
	RPOR3bits.RP6R = 3;
	TRISB &= 0b1111111110111111;
	*/
	// Map output U1TX to RP6
	RPOR4bits.RP8R = 3;
	TRISB &= 0b1111111011111111;

	// Max input U1RX to RP7
	RPINR18bits.U1RXR = 7;
	TRISB |= 0b0000000010000000;

	// val_U1BRG = (fcy/baudrate/16)-1;
	val_U1BRG = 0;
	//val_U1BRG = baudrate;

	val_U1MODE =/* UART_IDLE_CON & 	
				UART_DIS_WAKE & 
				UART_EN_LOOPBACK  & */
				UART_EN &
				UART_IDLE_CON &
				UART_DIS_WAKE &
				UART_IrDA_DISABLE &
				UART_DIS_LOOPBACK &
				UART_DIS_ABAUD &
				UART_NO_PAR_8BIT  &
				UART_MODE_SIMPLEX &
				UART_UEN_00 &
				UART_BRGH_FOUR &
				// UART_BRGH_SIXTEEN &
				UART_1STOPBIT &
				UART_UXRX_IDLE_ONE;

	val_U1STA =	UART_INT_TX_LAST_CH  &  
				UART_IrDA_POL_INV_ONE &
				UART_SYNC_BREAK_DISABLED &
				UART_TX_ENABLE & 
				UART_INT_RX_CHAR & 
				// UART_INT_RX_3_4_FUL &
				UART_ADR_DETECT_DIS &
				UART_RX_OVERRUN_CLEAR;

	OpenUART1(val_U1MODE, val_U1STA, val_U1BRG);

	// Former flags leave some erroneus 1 on following bits
	U1STAbits.URXDA = 0;
	U1STAbits.PERR = 0;
	U1STAbits.OERR = 0;
	U1STAbits.FERR = 0;
	U1STAbits.TRMT = 0; //1 
	U1STAbits.UTXBF = 0;
	U1STAbits.UTXINV = 0;
	U1MODEbits.URXINV = 0;

	// Configure uart1 receive by interrupt
	// 	(remember not to transmit by interrupt at this moment, since a null interrupt will
	//  be generated). Enable TX interrupts when data to transmit will be available
	/*ConfigIntUART1(	UART_RX_INT_EN & 
					UART_RX_INT_PR6 & 
                   	UART_TX_INT_EN & 
					UART_TX_INT_PR2 );*/
}

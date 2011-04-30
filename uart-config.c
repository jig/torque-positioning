/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#include <uart.h>

void configUart1(long fcy, long baudrate) {
	U1BRG = (fcy/baudrate/4)-1;

/* U1MODE */
	U1MODEbits.UARTEN = 1;
	U1MODEbits.USIDL = 0;
	U1MODEbits.IREN = 0;
	U1MODEbits.RTSMD = 1;
	U1MODEbits.UEN = 0b00;
	U1MODEbits.WAKE = 0;
	U1MODEbits.LPBACK = 0;
	U1MODEbits.ABAUD = 0;
	U1MODEbits.URXINV = 0;
	U1MODEbits.STSEL = 0;
	U1MODEbits.PDSEL = 0b00;
	U1MODEbits.BRGH = 1;

/* U1STA */
	U1STAbits.ADDEN = 0;
	U1STAbits.URXISEL = 0b00;
	U1STAbits.UTXEN = 1;
	U1STAbits.UTXBRK = 0;
	U1STAbits.UTXINV = 0;
	// RO U1STAbits.URXDA
	// RO U1STAbits.OERR
	// RO U1STAbits.FERR
	// RO U1STAbits.PERR
	// RO U1STAbits.RIDLE
	// RO U1STAbits.TRMT
	// RO U1STAbits.UTXBF

	// Map output U1TX to RP7
	RPOR3bits.RP7R = 3;
	TRISB &= 0b1111111101111111;

	// Map output U1TX to RP8
	// RPOR4bits.RP8R = 3;
	// TRISB &= 0b1111111011111111;

	// Max input U1RX to RP7
	// RPINR18bits.U1RXR = 7;
	// TRISB |= 0b0000000010000000;
}

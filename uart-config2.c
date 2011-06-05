/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#include <uart.h>

void configUart2(long fcy, long baudrate) {
	U2BRG = (fcy/baudrate/4)-1;

/* U2MODE */
	U2MODEbits.UARTEN = 1;
	U2MODEbits.USIDL = 0;
	U2MODEbits.IREN = 0;
	U2MODEbits.RTSMD = 1;
	U2MODEbits.UEN = 0b00;
	U2MODEbits.WAKE = 0;
	U2MODEbits.LPBACK = 0;
	U2MODEbits.ABAUD = 0;
	U2MODEbits.URXINV = 0;
	U2MODEbits.STSEL = 0;
	U2MODEbits.PDSEL = 0b00;
	U2MODEbits.BRGH = 1;

/* U2STA */
	U2STAbits.ADDEN = 0;
	U2STAbits.URXISEL = 0b00;
	U2STAbits.UTXEN = 1;
	U2STAbits.UTXBRK = 0;
	U2STAbits.UTXINV = 0;
	// Read-only bits:
	// RO U2STAbits.URXDA
	// RO U2STAbits.OERR
	// RO U2STAbits.FERR
	// RO U2STAbits.PERR
	// RO U2STAbits.RIDLE
	// RO U2STAbits.TRMT
	// RO U2STAbits.UTXBF

	// Map output U2TX to RP8 and RP9
	// RPOR4bits.RP8R = 3;
	// RPOR4bits.RP9R = 3;
	// TRISB &= 0b1111110011111111;

	RPOR2bits.RP4R = 5;
	TRISB &= 0b1111111111101111;

	// Max input U2RX to RP7
	// RPINR18bits.U2RXR = 7;
	// TRISB |= 0b0000000010000000;
}

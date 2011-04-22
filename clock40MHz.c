/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#include <p33Fxxxx.h>

void configClock40MHz() {
	/* Configure Oscillator to operate device at 40MHz
	Fosc = (Fin x M) / (N1 x N2),  Fcy = Fosc / 2
	Fosc= 7.37 x (43) / (2 x 2) = ~80MHz for Fosc, Fcy = 40MHz */
	/* Configure PLL prescaler, PLL postscaler, PLL divisor */
	PLLFBD = 41; /*  M = PLLFBD + 2 */
	CLKDIVbits.PLLPOST = 0;   /*  N1 = 2 */
	CLKDIVbits.PLLPRE = 0;     /*  N2 = 2 */
	__builtin_write_OSCCONH(0x01); /*  New Oscillator FRC w/ PLL */
	__builtin_write_OSCCONL(0x01);  /*  Enable Switch */
	while(OSCCONbits.COSC != 0b001); /*  Wait for new Oscillator to become FRC w/ PLL */  
	while(OSCCONbits.LOCK != 1); /*  Wait for PLL to Lock */
}

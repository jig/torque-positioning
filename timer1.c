#include <timer.h>
#include "timer1.h"

// int speedT1;

void __attribute__((__interrupt__, auto_psv)) 
_T1Interrupt (void)
{
	IFS0bits.T1IF = 0;   // Clear timer 1 interrupt flag
}

void __attribute__((__interrupt__, auto_psv)) 
_T3Interrupt (void)
{
	IFS0bits.T3IF = 0;   // Clear timer 3 interrupt flag
}

void configTimer1(int pr1) {
	TMR1  = 0;      	/* Reset Timer1 to 0x0000 */
	//PR1   = 0xFFFF;		/* assigning Period to Timer period register */
	PR1   = pr1;		/* assigning Period to Timer period register */
    T1CON = 			/* Configure timer control reg */
		T1_ON & 
		T1_GATE_OFF & 
		T1_IDLE_STOP &
		T1_PS_1_1 & 
		T1_SYNC_EXT_OFF &
		T1_SOURCE_INT;
}

void configTimer3(int pr3) {
	TMR3 = 0x0000;
	PR3 = pr3;

	IFS0bits.T3IF = 0;
	IEC0bits.T3IE = 0;
	
	//Start Timer 3
	T3CONbits.TON = 1;
}
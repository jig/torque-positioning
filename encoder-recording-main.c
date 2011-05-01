/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#include <p33Fxxxx.h>

_FOSCSEL(FNOSC_FRC)
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE)
_FWDT(FWDTEN_OFF)
_FICD(ICS_PGD3 & JTAGEN_OFF)
_FPOR(ALTI2C_ON)

#define FCY 40000000UL

#include <libpic30.h>

#include "clock40MHz.h"
#include "timer1.h"
#include "adc.h"
#include "uart-config1.h"

#define PWM_PERIOD 1000
#define PWM_MAXDUTY (PWM_PERIOD*2)

int a[4000], b[4000];


unsigned int* _AN0 = &BufferAdc[AN0];
unsigned int* _AN1 = &BufferAdc[AN1];

int main() {
	configClock40MHz();
	configTimer1(0xFFFF);
	configAdc();
	configUart1(FCY, 10000000);
	
	int k, i, INC=4;

	while(1) {
		for(i=0; i<4000; ++i) {
			TMR1 = 0;
			a[i] = *_AN0;
			b[i] = *_AN1;		
			U1TXREG = 0xFF;
			for(k=0;k<INC;++k) {
				__builtin_nop();
			}
		}
		TMR1 = 0;
		for(k=0;k<100;++k) {
			for(i=0;i<4400;++i) {
				__builtin_nop();
			}
		}
	} 
}

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

	float meanA = 511.5;
	float meanB = 511.5;

	while(1) {
		int maxA=0, minA=1023;
		int maxB=0, minB=1023;

		for(i=0; i<4000; ++i) {
			TMR1 = 0;
			a[i] = *_AN0;
			b[i] = *_AN1;		
			// U1TXREG = 0xFF;
			for(k=0;k<INC;++k) {
				__builtin_nop();
			}
		}
		for(i=0; i<4000; ++i) {
			meanA = meanA*0.999 + a[i]*0.001;
			meanB = meanB*0.999 + b[i]*0.001;
			minA = (minA>a[i]?a[i]:minA);
			minB = (minB>b[i]?b[i]:minB);
			maxA = (maxA<a[i]?a[i]:maxA);
			maxB = (maxB<b[i]?b[i]:maxB);
		}
		int SEMIAXE_A = (maxA-minA)/2;
		int SEMIAXE_B = (maxB-minB)/2;
		int MEAN_A = ((maxA+minA)/2 + (int)meanA)/2;
		int MEAN_B = ((maxB+minB)/2 + (int)meanB)/2;
		TMR1 = 0;
		for(k=0;k<100;++k) {
			for(i=0;i<4400;++i) {
				__builtin_nop();
			}
		}
	} 
}

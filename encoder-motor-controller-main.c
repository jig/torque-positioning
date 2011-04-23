/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#include <p33Fxxxx.h>

_FOSCSEL(FNOSC_FRC)
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON  & POSCMD_NONE)
_FWDT(FWDTEN_OFF)
_FICD(ICS_PGD3 & JTAGEN_OFF)

#define FCY 40000000UL

#include <libpic30.h>

#include "clock40MHz.h"
#include "timer1.h"
#include "encoder-multiplier.h"
#include "encoder-fingerprint.h"
#include "pwm.h"
#include "trigo.h"
#include "uart-config.h"
#include "pid.h"
#include "delay.h"

#define PWM_PERIOD 1000
#define PWM_MAXDUTY (PWM_PERIOD*2-1)

int main() {
	configClock40MHz();
	configTimer1(0xFFFF);
	configEncoder();
	configPwm(PWM_PERIOD);
	configField();
	configUart1(FCY, 1000000);
	
	long encPos = 0;
	long brakeAt = 0;

	setField(0, PWM_MAXDUTY);
	// 2 s is enough for my system to stabilize from a near position, 3 s from a far position
	delay_s(3); 
	setField(450, PWM_MAXDUTY);
	delay_s(3); 
	setField(900, PWM_MAXDUTY);
	delay_s(3); 
	setField(1350, PWM_MAXDUTY);
	delay_s(3); 
	setField(1800, PWM_MAXDUTY);
	delay_s(3); 
	setField(2250, PWM_MAXDUTY);
	delay_s(3); 
	setField(2700, PWM_MAXDUTY);
	delay_s(3); 
	setField(3150, PWM_MAXDUTY);
	delay_s(3);
	// now reset the encoder to zero (coarse, approx)
	setField(0, 0);

	resetEncoderPosition(0);
	int decaAngle = 0;
	unsigned int power = 0;

	while(1) {
			TMR1 = 0;
		coarseEncoderPosition(&encPos);
			// U1TXREG = 0b11111101;
		pid_Action(encPos - brakeAt, &decaAngle, &power);
			// U1TXREG = 0b11110101;
		setField(decaAngle, power);
			U1TXREG = 0b11111111;
	}
}

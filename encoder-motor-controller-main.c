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

#define PWM_PERIOD 1000
#define PWM_MAXDUTY (PWM_PERIOD*2-1)

int time=0;

void resetTick() {
	time = TMR1;
	TMR1 = 0;
	U1TXREG = 0xFF;
}

void tick() {
	U1TXREG = 0xFF;
	time = TMR1;
	time = TMR1;
}

int main() {
	configClock40MHz();
	configTimer1(0xFFFF);
	configEncoder();
	configPwm(PWM_PERIOD);
	configField();
	configUart1(FCY, 1000000);
	
	long encPos = 0;
	long brakeAt = 0;

	float angle = 0;
	float power = 990;

	setAngle(angle);
	setPower(power);

	while(1) {
			resetTick();
		encPos = encoderPosition();
			tick();
		pid_Action(encPos - brakeAt, &angle, &power);
			tick();
		setField();
			tick();
	}
}

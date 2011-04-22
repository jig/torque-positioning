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

#define PWM_PERIOD 10000
#define PWM_MAXDUTY (PWM_PERIOD*2-1)

unsigned int time=0;

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
	// now reset the encoder to zero (coarse, approx)
	resetEncoderPosition(0);

	float angle = 0;
	float power = 0;

	while(1) {
			U1TXREG = 0xFF;
			TMR1 = 0;
		encoderPosition(&encPos);
			U1TXREG = 0xFF;
			time = TMR1;
		pid_Action(encPos - brakeAt, &angle, &power);
			U1TXREG = 0xFF;
			time = TMR1;	
		setField(angle, power);
			U1TXREG = 0xFF;
			time = TMR1;
	}
}

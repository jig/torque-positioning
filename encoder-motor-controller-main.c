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
#include "encoder-multiplier.h"
#include "encoder-fingerprint.h"
#include "encoder-angle-mapping.h"
#include "pwm.h"
#include "trigo.h"
#include "uart-config1.h"
#include "uart-config2.h"
#include "pid.h"
#include "delay.h"

#define PWM_PERIOD 1000
#define PWM_MAXDUTY (PWM_PERIOD*2)

int main() {
	configClock40MHz();
	configTimer1(0xFFFF);
	configEncoder();
	configPwm(PWM_PERIOD);
	configField();
	// configPid(200, PWM_MAXDUTY, 500);
	configPid(0, PWM_MAXDUTY, 500);
	configUart1(FCY, 10000000);	
	configUart2(FCY, 10000000);	

	long encPos = 0;
	long brakeAt = 0;
	/*
	setPwm(1, PWM_MAXDUTY);
	delay_s(2);
	TMR1 = 0;
	setPwm(1, 0);
	setPwm(2, PWM_MAXDUTY);
	delay_s(2);
	TMR1 = 0;
	setPwm(2, 0);
	setPwm(3, PWM_MAXDUTY);
	delay_s(2);
	TMR1 = 0;
	setPwm(3, 0);
	setPwm(1, -PWM_MAXDUTY);
	delay_s(2);
	TMR1 = 0;
	setPwm(1, 0);
	setPwm(2, -PWM_MAXDUTY);
	delay_s(2);
	TMR1 = 0;
	setPwm(2, 0);
	setPwm(3, -PWM_MAXDUTY);
	delay_s(2);
	TMR1 = 0;
	setPwm(3, 0);
	delay_s(2);
	*/
	int L;
	for(L=700;L<780;++L) {
		setField(L, PWM_MAXDUTY);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
	}
	for(;L>=0;--L) {
		setField(L, PWM_MAXDUTY);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
	}
	resetEncoderPosition();
	for(;L<780;++L) {
		setField(L, PWM_MAXDUTY);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
		encoderPosition(&encPos);
		delay_ms(1);
	}
	delay_ms(300);
	encoderPosition(&encPos);
	TMR1 = 0;
/*	int xx = 0;

	for(L=0;L<925;++L) {
		setField(L, PWM_MAXDUTY);
		for(xx=0;xx<10;++xx) {
			encoderPosition(&encPos);
			delay_ms(1);
		}
		TMR1 = 0;
	}
	encoderPosition(&encPos);
	TMR1 = 0;
	for(L=925;L>0;--L) {
		setField(L, PWM_MAXDUTY);
		for(xx=0;xx<10;++xx) {
			encoderPosition(&encPos);
			delay_ms(1);
		}
		TMR1 = 0;
	}
	encoderPosition(&encPos);
	TMR1 = 0;
	for(L=0;L>-2400;--L) {
		setField(L, PWM_MAXDUTY);
		for(xx=0;xx<10;++xx) {
			encoderPosition(&encPos);
			delay_ms(1);
		}
		TMR1 = 0;
	}
	encoderPosition(&encPos);
	TMR1 = 0;
	for(L=-2400;L<0;++L) {
		setField(L, PWM_MAXDUTY);
		for(xx=0;xx<10;++xx) {
			encoderPosition(&encPos);
			delay_ms(1);
		}
		TMR1 = 0;
	}
	encoderPosition(&encPos);
	TMR1 = 0;
	// now reset the encoder to zero (coarse, approx)
*/

	int i;
	int incBrakeAt = 32; //64;
	int incI = 1; 
	long minTrack = -410000L;
	long maxTrack = 1370000L;
	long midTrack = (maxTrack + minTrack) / 2;

	long trackAtom(long*, long);

	for(brakeAt = _inverseGetBestAngle(L); brakeAt<maxTrack;brakeAt+=incBrakeAt) {
		U1TXREG = 0b11111111;
		trackAtom(&encPos, brakeAt);
	}

	long j;

	while(1) {
		for(; brakeAt>midTrack;brakeAt-=incBrakeAt) {
			U1TXREG = 0b11111111;
			trackAtom(&encPos, brakeAt);
		}
		for(j=0; j<20000;++j) {
			U1TXREG = 0b11111111;
			trackAtom(&encPos, brakeAt);
		}
		for(; brakeAt>minTrack;brakeAt-=incBrakeAt) {
			U1TXREG = 0b11111111;
			trackAtom(&encPos, brakeAt);
		}
		for(j=0; j<20000;++j) {
			U1TXREG = 0b11111111;
			trackAtom(&encPos, brakeAt);
		}
		for(; brakeAt<midTrack;brakeAt+=incBrakeAt) {
			U1TXREG = 0b11111111;
			trackAtom(&encPos, brakeAt);
		}
		for(j=0; j<20000;++j) {
			U1TXREG = 0b11111111;
			trackAtom(&encPos, brakeAt);
		}

		extern unsigned int pid_attr_i;
		extern long pid_meanError;
		pid_meanError = 0;
		pid_attr_i = 1;
		incBrakeAt = 1;

		for(; brakeAt<maxTrack;brakeAt+=incBrakeAt) {
			U1TXREG = 0b11111111;
			trackAtom(&encPos, brakeAt);
		}
		for(j=0; j<20000;++j) {
			U1TXREG = 0b11111111;
			trackAtom(&encPos, brakeAt);
		}
		pid_attr_i = 0;
		incBrakeAt = 64;
	}
}

long trackAtom(long *encPos, long brakeAt) {
	TMR1 = 0;
	
	int fieldTargetDecaAngle = getBestAngle(brakeAt);
	encoderPosition(encPos);

	long error = *encPos - brakeAt;
	int decaAngle;
	unsigned int power;
	pid_Action(error, fieldTargetDecaAngle, &decaAngle, &power);
	setField(decaAngle, power);
	return error;
}
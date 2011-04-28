/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#include <p33Fxxxx.h>

_FOSCSEL(FNOSC_FRC)
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF  & POSCMD_NONE)
_FWDT(FWDTEN_OFF)
_FICD(ICS_PGD3 & JTAGEN_OFF)
_FPOR(ALTI2C_ON)

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
#define PWM_MAXDUTY (PWM_PERIOD*2)

int main() {
	configClock40MHz();
	configTimer1(0xFFFF);
	configEncoder();
	configPwm(PWM_PERIOD);
	configField();
	configPid(200, PWM_MAXDUTY, 500);
	configUart1(FCY, 10000000);	

	long encPos = 0;
	long brakeAt = 0;

	// now reset the encoder to zero (coarse, approx)
	setField(0, PWM_MAXDUTY);
	// 2 s is enough for my system to stabilize from a near position, 3 s from a far position
	delay_s(3); 
	/*int p1=1, p2=1, p3=1;
	while(p1!=13) {
		setPwmAll(p1, p2, p3);
	}*/
	resetEncoderPosition();
	int decaAngle = 0;
	unsigned int power = 0;

	/*unsigned int i;
	for(i=0;i<40000;++i) {
			TMR1 = 0;
		coarseEncoderPosition(&encPos);
			// U1TXREG = 0b11111101;
		pid_Action(encPos - brakeAt, &decaAngle, &power);
			// U1TXREG = 0b11110101;
		setField(decaAngle, power);
			U1TXREG = 0b11111111;
	}
	while(1) {
		for(brakeAt=0; brakeAt<TRACK_LENGTH;brakeAt+=100) {
			for(i=0;i<4000;++i) {
					TMR1 = 0;
				encoderPosition(&encPos);
					// U1TXREG = 0b11111101;
				pid_Action(encPos - brakeAt, &decaAngle, &power);
					// U1TXREG = 0b11110101;
				setField(decaAngle, power);
					U1TXREG = 0b11111111;
			}
		}
		for(brakeAt=0; brakeAt<TRACK_LENGTH;brakeAt+=10) {
			for(i=0;i<400;++i) {
					TMR1 = 0;
				encoderPosition(&encPos);
					// U1TXREG = 0b11111101;
				pid_Action(encPos - brakeAt, &decaAngle, &power);
					// U1TXREG = 0b11110101;
				setField(decaAngle, power);
					U1TXREG = 0b11111111;
			}
		}
		for(brakeAt=0; brakeAt<TRACK_LENGTH;++brakeAt) {
			for(i=0;i<40;++i) {
					TMR1 = 0;
				encoderPosition(&encPos);
					// U1TXREG = 0b11111101;
				pid_Action(encPos - brakeAt, &decaAngle, &power);
					// U1TXREG = 0b11110101;
				setField(decaAngle, power);
					U1TXREG = 0b11111111;
			}
		}
	}*/
	int i;
	int incBrakeAt = 1;
	int incI = 100; 
	long minTrack = -100L * 3600L;
	long maxTrack = 200L * 3600L;
	int fieldTargetDecaAngle; 

	for(brakeAt=0; brakeAt<maxTrack;brakeAt+=incBrakeAt) {
		TMR1 = 0;
		U1TXREG = 0b11111111;
		for(i=0;i<incI;++i) {
			fieldTargetDecaAngle = getBestAngle(brakeAt);
			encoderPosition(&encPos);
			pid_Action(encPos - brakeAt, fieldTargetDecaAngle, &decaAngle, &power);
			setField(decaAngle, power);
		}
	}

	while(1) {
		for(brakeAt=maxTrack; brakeAt>minTrack;brakeAt-=incBrakeAt) {
			TMR1 = 0;
			U1TXREG = 0b11111111;
			for(i=0;i<incI;++i) {
				fieldTargetDecaAngle = getBestAngle(brakeAt);
				encoderPosition(&encPos);
				pid_Action(encPos - brakeAt, fieldTargetDecaAngle, &decaAngle, &power);
				setField(decaAngle, power);
			}
		}
		for(brakeAt=minTrack; brakeAt<maxTrack;brakeAt+=incBrakeAt) {
			TMR1 = 0;
			U1TXREG = 0b11111111;
			for(i=0;i<incI;++i) {
				fieldTargetDecaAngle = getBestAngle(brakeAt);
				encoderPosition(&encPos);
				pid_Action(encPos - brakeAt, fieldTargetDecaAngle, &decaAngle, &power);
				setField(decaAngle, power);
			}
		}
	}
}

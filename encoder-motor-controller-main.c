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

unsigned int poum = 1000;

int main() {
	configClock40MHz();
	configTimer1(0xFFFF);
	configEncoder();
	configPwm(PWM_PERIOD);
	configField();
	configPid(0, PWM_MAXDUTY, 600);
	configUart1(FCY, 10000000);	
	configUart2(FCY, 10000000);

	long encPos = 0;
	long brakeAt = 0;

#define END_OF_TRACK 780
	int L;
	for(L=END_OF_TRACK-30;L<END_OF_TRACK;++L) {
		setField(L, PWM_MAXDUTY);
		delay_ms(7);
	}
	delay_ms(1000); // stoping time
	resetEncoderPositionTo(_inverseGetBestAngle(L));
	TMR1 = 0;

	int i;
	int incBrakeAt = 40; //64;
	int incI = 1; 
	long minTrack = -380000L;
	long maxTrack = 1370000L;
	long midTrack = -200000L;
	// long midTrack = -200000;

	long trackAtom(long*, long);
	long trackAtomPoum(long *, long, unsigned int);

	for(brakeAt = _inverseGetBestAngle(L); brakeAt<midTrack;brakeAt+=incBrakeAt) {
		U1TXREG = 0b11111111;
		trackAtom(&encPos, brakeAt);
	}
	/* while(1) {
		U1TXREG = 0b11111111;
		trackAtom(&encPos, brakeAt);
	}*/

	long j;

	for(j=0; j<10000; ++j) {
		U1TXREG = 0b11111111;
		trackAtom(&encPos, brakeAt);
	}
	/*
	*errorLogPointer++ = brakeAt - encPos;
	setField(1800, 2000);
	delay_ms(1);
	*errorLogPointer++ = brakeAt - encPos;
	setField(0, 2000);
	delay_ms(1);
	*errorLogPointer++ = brakeAt - encPos;
	setField(0, 0);
	delay_ms(1);
	*errorLogPointer++ = brakeAt - encPos;
	
	long jlast = 2048L*(long)errorGapStop;

	for(j=0; j<jlast; ++j) {
		U1TXREG = 0b11111111;
		encoderPosition(&encPos);
		if(errorGap==0) {
			errorGap = errorGapStop;
			// *actionLogPointer++ = action;
			*errorLogPointer++ = brakeAt - encPos;
			if(errorLogPointer == errorLogLastPointer) {
				errorLogPointer = errorLog;
				// actionLogPointer = actionLog;
			}
		} else {	
			--errorGap;
		}
	}

	while(1) {
		encoderPosition(&encPos);
		U1TXREG = 0b11111111;
	}*/

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

		/*extern unsigned int pid_attr_i;
		extern long pid_meanError;
		pid_meanError = 0;
		pid_attr_i = 0;*/
		incBrakeAt = 1;

		for(; brakeAt<maxTrack;brakeAt+=incBrakeAt) {
			for(i=0;i<190;++i) {
				trackAtom(&encPos, brakeAt);
			}
			trackAtomPoum(&encPos, brakeAt, poum);
			for(i=0;i<50;++i) {
				trackAtom(&encPos, brakeAt);
			}
			trackAtomPoum(&encPos, brakeAt, poum);
			for(i=0;i<170;++i) {
				trackAtom(&encPos, brakeAt);
			}
			trackAtomPoum(&encPos, brakeAt, poum);
			for(i=0;i<130;++i) {
				trackAtom(&encPos, brakeAt);
			}
			trackAtomPoum(&encPos, brakeAt, poum);
			for(i=0;i<90;++i) {
				trackAtom(&encPos, brakeAt);
			}
			trackAtomPoum(&encPos, brakeAt, poum);
			for(i=0;i<30;++i) {
				trackAtom(&encPos, brakeAt);
			}
			trackAtomPoum(&encPos, brakeAt, poum);
		}
		for(j=0; j<20000;++j) {
			U1TXREG = 0b11111111;
			trackAtom(&encPos, brakeAt);
		}
		// pid_attr_i = 0;
		incBrakeAt = 64;
	}
}

// int period = 3450;
int period = 6900;

long trackAtom(long *encPos, long brakeAt) {
	TMR1 = 0;
	U1TXREG = 0;

	int fieldTargetDecaAngle = getBestAngle(brakeAt);
	encoderPosition(encPos);

	long error = *encPos - brakeAt;
	int decaAngle;
	unsigned int power;
	pid_Action(error, fieldTargetDecaAngle, &decaAngle, &power);
	setField(decaAngle, power);
	while(TMR1 < period);
	return error;
}

long trackAtomPoum(long *encPos, long brakeAt, unsigned int poum) {
	TMR1 = 0;
	U1TXREG = 0;

	int fieldTargetDecaAngle = getBestAngle(brakeAt);
	encoderPosition(encPos);

	long error = *encPos - brakeAt;
	int decaAngle;
	unsigned int power;
	pid_Action(error, fieldTargetDecaAngle, &decaAngle, &power);
	setField(decaAngle, poum+power);
	while(TMR1 < period);
	return error;
}

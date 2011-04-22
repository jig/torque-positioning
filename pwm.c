#include <p33Fxxxx.h>
#include <pwm12.h>
#include "pwm.h"

void __attribute__((__interrupt__, auto_psv)) 
_MPWM1Interrupt(void)
{
   IFS3bits.PWM1IF = 0;
}

void configPwm(unsigned period) 
{
	TRISB &= 0b0000001111111111;

 	ConfigIntMCPWM1( 
			PWM1_INT_DIS & 
			PWM1_FLTA_DIS_INT /*& 
			PWM1_INT_PR1 & 
			PWM1_FLTA_INT_PR0 */
			// PWM1_FLTB_DIS_INT & 
			// PWM1_FLTB_INT_PR0
		);
	
	unsigned int val_PTPER = period?period:0x7fff; //0x7fff;
	unsigned int val_SEVTCMP = 0x0; //sptime;

	unsigned int val_P1TCON = 	
				PWM1_EN & 
				/*PWM1_PTSIDL_DIS & */
				//PWM1_OP_SCALE16 &
				PWM1_OP_SCALE1 &
				//PWM1_IPCLK_SCALE16 &
				PWM1_IPCLK_SCALE1 &
				PWM1_MOD_UPDN;

    unsigned int val_PWM1CON1 =
				/*PWM1_MOD3_COMP &
				PWM1_MOD2_COMP &
				PWM1_MOD1_COMP & */
				PWM1_MOD3_IND &
				PWM1_MOD2_IND &
				PWM1_MOD1_IND &	
				/*PWM1_PDIS4H & 
				PWM1_PDIS3H &
				PWM1_PDIS2H & 
				PWM1_PEN1H & 
				PWM1_PDIS4L &
				PWM1_PDIS3L & 
				PWM1_PDIS2L & */
				PWM1_PEN1L;

    unsigned int val_PWM1CON2 =
				// PWM1_SEVOPS1 & 
				PWM1_OSYNC_TCY &
				// PWM1_OSYNC_PWM &  
				PWM1_UEN;

	OpenMCPWM1( 
			val_PTPER, 
			val_SEVTCMP, 
			val_P1TCON,
			val_PWM1CON1,
			val_PWM1CON2
		);

	PWM1CON2bits.OSYNC = 1;
	
	P1OVDCONbits.POVD1H = 1;
	P1OVDCONbits.POVD2H = 1;
	P1OVDCONbits.POVD3H = 1;
	P1OVDCONbits.POVD1L = 0;
	P1OVDCONbits.POVD2L = 0;
	P1OVDCONbits.POVD3L = 0;

	P1OVDCONbits.POUT1H = 0;
	P1OVDCONbits.POUT2H = 0;
	P1OVDCONbits.POUT3H = 0;
	P1OVDCONbits.POUT1L = 0;
	P1OVDCONbits.POUT2L = 0;
	P1OVDCONbits.POUT3L = 0;

	setPwm(1, 0);
	setPwm(2, 0);
	setPwm(3, 0);
}

void setPwmAll(long duty1, long duty2, long duty3) {
	PWM1CON2bits.UDIS = 0;
	
	P1OVDCONbits.POVD1H = duty1>0;
	P1OVDCONbits.POVD1L = !P1OVDCONbits.POVD1H;
	P1DC1 = P1OVDCONbits.POVD1H?duty1:-duty1;	

	P1OVDCONbits.POVD2H = duty2>0;
	P1OVDCONbits.POVD2L = !P1OVDCONbits.POVD2H;
	P1DC2 = P1OVDCONbits.POVD2H?duty2:-duty2;	

	P1OVDCONbits.POVD3H = duty3>0;
	P1OVDCONbits.POVD3L = !P1OVDCONbits.POVD3H;
	P1DC3 = P1OVDCONbits.POVD3H?duty3:-duty3;	
}

void setPwm(int channel, long dutycycle) {
	
	PWM1CON2bits.UDIS = 0;
	
	switch(channel) {
	default:
	case 1:
		P1OVDCONbits.POVD1H = dutycycle>0;
		P1OVDCONbits.POVD1L = !P1OVDCONbits.POVD1H;
		P1DC1 = P1OVDCONbits.POVD1H?dutycycle:-dutycycle;	
		break;
	case 2:
		P1OVDCONbits.POVD2H = dutycycle>0;
		P1OVDCONbits.POVD2L = !P1OVDCONbits.POVD2H;
		P1DC2 = P1OVDCONbits.POVD2H?dutycycle:-dutycycle;	
		break;
	case 3:
		P1OVDCONbits.POVD3H = dutycycle>0;
		P1OVDCONbits.POVD3L = !P1OVDCONbits.POVD3H;
		P1DC3 = P1OVDCONbits.POVD3H?dutycycle:-dutycycle;	
		break;
	}
}

long getPwm(int channel) {
	long dutycycle;
	switch(channel) {
	default:
	case 1:
		dutycycle = P1DC1;
		return P1OVDCONbits.POVD1H?dutycycle:-dutycycle;
	case 2:
		dutycycle = P1DC2;
		return P1OVDCONbits.POVD2H?dutycycle:-dutycycle;
	case 3:
		dutycycle = P1DC3;
		return P1OVDCONbits.POVD3H?dutycycle:-dutycycle;
	}
}

unsigned int getPwmPeriod()
{
	return PTPER;
}

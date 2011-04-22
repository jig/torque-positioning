/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#ifndef _TRIGO_APPROX_H
#endif _TRIGO_APPROX_H
#	include <p33Fxxxx.h>
#	define FCY 7372800UL/2

#	include <libpic30.h>

#	include <math.h>
#	include "trigo.h"
#	include "pwm.h"

#	define PI 3.1415926535897932384626433832795
#	define DOUBLEPI (PI*2.0)
#	define HALFPI (PI/2.0)
#	define TORADIANS (PI/180.0)
#	define THIRDSPI (PI/3.0)
#	define DOUBLETHIRDSPI (DOUBLEPI/3.0)

	double field_angle = defaultAngle;
	double field_power = 0;
	
	// in 90 degrees:
	#define SIN_RESOLUTION 900
	unsigned int sinVal[SIN_RESOLUTION+1];
	
	long sinAprox(double angle) {
		// unsigned int integerAngle = (angle/90.0)*((double)SIN_RESOLUTION);
		unsigned int integerAngle = angle*(SIN_RESOLUTION/90.0);
	
		integerAngle = integerAngle%(SIN_RESOLUTION * 4);
	
		if(integerAngle<SIN_RESOLUTION) {
			return (long)sinVal[integerAngle];
		}
		if(integerAngle<SIN_RESOLUTION*2) {
			unsigned int idx = (SIN_RESOLUTION*2)-integerAngle;
			return (long)sinVal[idx];
		}
		if(integerAngle<SIN_RESOLUTION*3) {
			unsigned int idx = integerAngle-(SIN_RESOLUTION*2);
			return -(long)sinVal[idx];
		}
		return -(long)sinVal[(SIN_RESOLUTION*4)-integerAngle];
	}
	
	void configField() {
		int i;
		for(i=0; i<=SIN_RESOLUTION; ++i) {
			double sinValue = sin( ((double)i) * (HALFPI/SIN_RESOLUTION) );
			sinVal[i] = (long)(65535.4 * sinValue);
		}
	}
	
	extern unsigned int time;
	void setField(long angle, long power) {
		time = TMR1;
		long ph1 = field_power * sinAprox(field_angle+90.0 + 60.0);
		time = TMR1;
		long ph2 = field_power * sinAprox(field_angle+90.0 );
		time = TMR1;
		long ph3 = field_power * sinAprox(field_angle+90.0 - 60.0);
		time = TMR1;
	
		setPwmAll(ph1, ph2, ph3);
	}
	
	void setAngle(double a) {
		field_angle = a;
	}
	
	void setPower(double p) {
		field_power = p;
	}
	
	double getPower() {
		return field_power;
	}
	
	double getAngle() {
		return field_angle;
	}
#endif
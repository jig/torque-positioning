/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#include <p33Fxxxx.h>
#define FCY 7372800UL/2

#include <libpic30.h>

#include <math.h>
#include "trigo.h"
#include "pwm.h"

#define PI 3.1415926535897932384626433832795
#define DOUBLEPI (PI*2.0)
#define HALFPI (PI/2.0)
#define TORADIANS (PI/180.0)
#define THIRDSPI (PI/3.0)
#define DOUBLETHIRDSPI (DOUBLEPI/3.0)

// This functions are based on "deca-degree" resolution: so a whole circunference has 3600 deca-degrees
// Deca-degrees offer resolution enough to shape the magnetic field needed, an can be easily stored in RAM
// FFS: store the deca-degree shape on ROM
// FFS2: use of fractional Q-something dsPIC types

// in 90 degrees:
#define SIN_RESOLUTION 900
int sinVal[SIN_RESOLUTION+1];

int sinAprox(int decaAngle) {
	decaAngle = decaAngle % (SIN_RESOLUTION * 4);

	if(decaAngle <= SIN_RESOLUTION) {
		return sinVal[decaAngle];
	}
	if(decaAngle <= SIN_RESOLUTION*2) {
		int idx = (SIN_RESOLUTION*2) - decaAngle;
		return sinVal[idx];
	}
	if(decaAngle <= SIN_RESOLUTION*3) {
		int idx = decaAngle - (SIN_RESOLUTION*2);
		return -sinVal[idx];
	}
	return -sinVal[(SIN_RESOLUTION*4) - decaAngle];
}

void configField() {
	int i;
	for(i=0; i<=SIN_RESOLUTION; ++i) {
		double sinValue = sin(i * (HALFPI/SIN_RESOLUTION));
		sinVal[i] = (int)(32767.49 * sinValue);
	}
}

void setField(int decAngle, unsigned int power) {
	long ph1 = (long)power * sinAprox(decAngle+900 + 600);
	long ph2 = (long)power * sinAprox(decAngle+900);
	long ph3 = (long)power * sinAprox(decAngle+900 - 600);

	ph1 >>= 15;
	ph2 >>= 15;
	ph3 >>= 15;
	setPwmAll(ph1, ph2, ph3);
}

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

double field_angle;
double field_power;

void configField() {}

void setField() {
	double _angle = field_angle * TORADIANS;
	long ph1 = field_power * cos(_angle+THIRDSPI);
	long ph2 = field_power * cos(_angle);
	long ph3 = field_power * cos(_angle-THIRDSPI);

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

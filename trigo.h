/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#ifndef _TRIGO_H
#	define _TRIGO_H
#	define defaultAngle 0

	extern double angle;
	extern double power;
	
	void configField();
	void setField();
	
	void setAngle(double a);
	void setPower(double p);
	double getAngle();
	double getPower();
#endif

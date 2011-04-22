/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#ifndef _PWM_H
#	define _PWM_H
	void configPwm(unsigned int period);
	
	void setPwm1(unsigned int dutycycle, char updatedisable);
	void setPwm2(unsigned int dutycycle, char updatedisable);
	void setPwm3(unsigned int dutycycle, char updatedisable);
	
	void setPwm(int channel, long dutycycle);
	long getPwm(int channel);
	void setPwmAll(long duty1, long duty2, long duty3);
	
	unsigned int getPwmPeriod();
#endif


/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#ifndef _PID_H
#	define _PID_H
	void configPid(unsigned int dutymin, unsigned int dutymax, unsigned meanErrorMax);
	void pid_Action(long posError, int targetDecaAngle, int *decaAngle, unsigned int *power);
#endif

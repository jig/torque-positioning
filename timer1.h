/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#ifndef _TIMER1_H
#	define _TIMER1_H
#	define t1() (TMR1)
#	define resetT1() TMR1 = 0

	void configTimer1(int pr1);
#endif
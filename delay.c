/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#include "delay.h"

/*
	Note: this is only valid for a dsPIC33 at 40 MHz FCY as with:
		#define FCY 40000000UL
*/

void delay_s(unsigned int seconds) {
	int i, j, k;
	for(k=0;k<seconds;++k) {
		for(j=0;j<1000;++j) {
			for(i=0;i<4400;++i) {
				__builtin_nop();
			}
		}
	}
}

void delay_ms(unsigned int milliseconds) {
	int i, k;
	for(k=0;k<milliseconds;++k) {
		for(i=0;i<4400;++i) {
			__builtin_nop();
		}
	}
}

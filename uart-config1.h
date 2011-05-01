/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#ifndef _UART_CONFIG1_H
#	define _UART_CONFIG1_H
#	include <uart.h>

#	define NL "\r\n"
// #define NL 	"\x1B[1E"

	void configUart1(long fcy, long baudvalue);
#endif

/****************************************************************/
/** Author: Jordi ��igo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#ifndef _UART_CONFIG_H
#	define _UART_CONFIG_H
#	include <uart.h>

#	define NL "\r\n"
// #define NL 	"\x1B[1E"

	void configUart2(long fcy, long baudvalue);
#endif

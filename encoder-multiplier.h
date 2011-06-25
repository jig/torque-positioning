/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#ifndef _ENCODER_MULTIPLIER_H
#	define _ENCODER_MULTIPLIER_H
#	include "encoder-fingerprint.h"
	
	void configEncoder();
	void coarseEncoderPosition(long *_encPos);
	void encoderPosition(long *encPos);
	void deltaEncoderPosition(long *_encPos);
	void logEncoderPosition(long *encPos);
	void resetEncoderPosition();
	void resetEncoderPositionTo(long pos);
#endif _ENCODER_MULTIPLIER_H
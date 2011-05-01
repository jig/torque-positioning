/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#ifndef ENCODER_FINGERPRINT_H
#	define ENCODER_FINGERPRINT_H
	struct encoder_point {int a; int b;};
	
	extern const unsigned int track_A[];
	extern const unsigned int track_B[];
	extern const unsigned int coarseInc;
	extern const unsigned int inc1;
	extern const unsigned int inc2;
	extern const unsigned int TRACK_LENGTH;
	extern const unsigned int LAST_COARSE1_TRACK_POSITION;
	extern const unsigned int LAST_COARSE2_TRACK_POSITION;
	extern const unsigned int LAP_SCAN_THRESHOLD_LOW;
	extern const unsigned int LAP_SCAN_THRESHOLD_HIGH;
	extern int short_a[];
	extern int short_b[];
	extern int short_pos[];
	extern const unsigned int SHORT_TRACK_LENGTH;
#endif


/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#ifndef ENCODER_FINGERPRINT_H
#	define ENCODER_FINGERPRINT_H
	struct encoder_point {int a; int b;};
	
	extern const struct encoder_point track[];
	extern const int inc1;
	extern const int inc2;
	extern const unsigned int TRACK_LENGTH;
	extern const unsigned int LAST_COARSE1_TRACK_POSITION;
	extern const unsigned int LAST_COARSE2_TRACK_POSITION;
	extern const unsigned int LAP_SCAN_THRESHOLD_LOW;
	extern const unsigned int LAP_SCAN_THRESHOLD_HIGH;
#endif


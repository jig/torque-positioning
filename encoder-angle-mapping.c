/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

// defines steps per (decaAngle) degree:
// (this encoder has 1 million (approx) steps per rev:
#define ANGLE_ENCODER_RATIO (1790186L/3250L)
// #define ANGLE_ENCODER_RATIO 360

// Use -/+ sign to adjust encoder and motor directions
#define INVERSE_ENCODER

int getBestAngle(long encPos) { 
#ifdef INVERSE_ENCODER
	return - encPos / ANGLE_ENCODER_RATIO;
#else
	return encPos / ANGLE_ENCODER_RATIO;
#endif
}

long _inverseGetBestAngle(int decaAngle) { 
#ifdef INVERSE_ENCODER
	return - (long)decaAngle * ANGLE_ENCODER_RATIO;
#else
	return (long)decaAngle * ANGLE_ENCODER_RATIO;
#endif
}

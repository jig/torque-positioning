/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

// defines steps per (decaAngle) degree:
// (this encoder has 1 million (approx) steps per rev:
#define ANGLE_ENCODER_RATIO (1080000L/3600L)

int getBestAngle(long encPos) { 
	return encPos / ANGLE_ENCODER_RATIO;
}
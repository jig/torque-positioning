/****************************************************************/
/** Author: Jordi ��igo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

// defines steps per (decaAngle) degree:
// (this encoder has 1 million (approx) steps per rev:
#define ANGLE_ENCODER_RATIO (900000L/1800L)


// Use -/+ sign to adjust encoder and motor directions
int getBestAngle(long encPos) { 
	return encPos / ANGLE_ENCODER_RATIO;
	// return - encPos / ANGLE_ENCODER_RATIO;
}

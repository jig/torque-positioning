/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

/*
	- This version of rotary/lineal encoder-multiplier decoder uses a elliptic generated track that
	must be callibrated before use with real track.
	- This track is generated during system load on RAM
	- Afterwards, mesured a,b points are distance mesured against 
*/

#include <p33Fxxxx.h> // TMR1
#include <math.h>

#include "adc.h"
#include "encoder-fingerprint.h"
#include "encoder-multiplier.h"

unsigned int* _AN0 = &BufferAdc[AN0];
unsigned int* _AN1 = &BufferAdc[AN1];
int i;

int scan;
int nearest;
int a, b;
unsigned int dist;
unsigned int d_aux;
long long_dist;
long long_d_aux;

long encoderAbsolutePosition = 0;
long laps = 0;
int last_nearest = -1;
int lap_warning = 0;
int lap_count_headroom = 0;

// 181 max (181^2 = 2^15)
#define _BOUNDS 181
int bounds = _BOUNDS; 
int bounds_minus = -_BOUNDS;
int absolute_command = 0;

void configEncoder()
{
	configAdc();
	configTrack();
	configShortTrack();
}

void resetEncoderPositionTo(long pos) {
	laps = pos/TRACK_LENGTH;
}

void resetEncoderPosition() {
	// coarse reset
	laps = 0L;
}

void encoderPosition(long *_encPos)
{
	a = *_AN0 - MEAN_A;
	b = *_AN1 - MEAN_B;

	float nf = atan2f( (float)a, (float)b ) * 162.97466172610082382733697369345;
	nearest = 512 + (int)nf;

	if(last_nearest>LAP_SCAN_THRESHOLD_HIGH) {
		if(nearest<LAP_SCAN_THRESHOLD_LOW) {
			++laps;
		}
	} else if(last_nearest<LAP_SCAN_THRESHOLD_LOW) {
		if(nearest>LAP_SCAN_THRESHOLD_HIGH) {
			--laps;
		}
	} 

	last_nearest = nearest;
	encoderAbsolutePosition = (laps * TRACK_LENGTH) + nearest;
	*_encPos = encoderAbsolutePosition;
}


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
	a = *_AN0;
	b = *_AN1;
	int sa = a>>3;
	int sb = b>>3;

	TMR1 = 0;
	// init search variables
	dist = 0x7FFF;
	nearest = -1;

	// Coarse position approximation: track is search in inc1 steps 
	for(scan=0;scan<SHORT_TRACK_LENGTH;++scan) {
		int error_a = sa-short_a[scan];
		int error_b = sb-short_b[scan];
		d_aux = error_a*error_a + error_b*error_b;
		if(d_aux < dist) {
			dist = d_aux;
			nearest = short_pos[scan];
		}
	}
	++dist;
	dist <<= 6; // multiply by 8 and square it: 64 => 2^6

	// from coarse search, track is sectioned 
	int section_a1;
	int section_a2;
	int section_b1;
	int section_b2;

	if (nearest==0) {
		// Special case: nearest is first
		section_a1 = LAST_COARSE1_TRACK_POSITION;
		section_a2 = TRACK_LENGTH;
		section_b1 = 0;
		section_b2 = inc1;
	} else if (nearest==LAST_COARSE1_TRACK_POSITION) {
		// Special case: nearest is last
		section_a1 = LAST_COARSE1_TRACK_POSITION-inc1;
		section_a2 = LAST_COARSE1_TRACK_POSITION;
		section_b1 = LAST_COARSE1_TRACK_POSITION;
		section_b2 = TRACK_LENGTH;
	} else {
		//  General case:
		section_a1 = nearest-inc1;
		section_a2 = nearest;
		section_b1 = nearest;
		section_b2 = nearest+inc1;
	}

	// search again on target now coarsely narrowed
	int ontarget = 0;
	long_dist = dist;
	for(scan=section_a1;scan<section_a2;scan+=inc2) {
		int error_a = a-track_A[scan];
		int error_b = b-track_B[scan];
		long_d_aux = __builtin_mulss(error_a, error_a) + __builtin_mulss(error_b, error_b);
		if(long_d_aux < long_dist) {
			long_dist = long_d_aux;
			nearest = scan;
		}
	}
	for(scan=section_b1;scan<section_b2;scan+=inc2) {
		int error_a = a-track_A[scan];
		int error_b = b-track_B[scan];
		long_d_aux = __builtin_mulss(error_a, error_a) + __builtin_mulss(error_b, error_b);
		if(long_d_aux < long_dist) {
			long_dist = long_d_aux;
			nearest = scan;
		}
	}

	// from second coarse approach, track is sectioned for final nailing
	if (nearest==0) {
		// Special case: nearest is first
		section_a1 = LAST_COARSE2_TRACK_POSITION;
		section_a2 = TRACK_LENGTH;
		section_b1 = 0;
		section_b2 = inc2;
	} else if (nearest==LAST_COARSE2_TRACK_POSITION) {
		// Special case: nearest is last
		section_a1 = LAST_COARSE2_TRACK_POSITION-inc2;
		section_a2 = LAST_COARSE2_TRACK_POSITION;
		section_b1 = LAST_COARSE2_TRACK_POSITION;
		section_b2 = TRACK_LENGTH;
	} else {
		//  General case:
		section_a1 = nearest-inc2;
		section_a2 = nearest;
		section_b1 = nearest;
		section_b2 = nearest+inc2;
	}
	ontarget = 0;
	for(scan=section_a1;scan<section_a2;++scan) {
		int error_a = a-track_A[scan];
		int error_b = b-track_B[scan];
		long_d_aux = __builtin_mulss(error_a, error_a) + __builtin_mulss(error_b, error_b);
		if(long_d_aux < long_dist) {
			long_dist = long_d_aux;
			nearest = scan;
		}
	}

	// fine search in step-by-step traverse
	for(scan=section_a1;scan<section_a2;++scan) {
		int error_a = a-track_A[scan];
		int error_b = b-track_B[scan];
		long_d_aux = __builtin_mulss(error_a, error_a) + __builtin_mulss(error_b, error_b);
		if(long_d_aux < long_dist) {
			long_dist = long_d_aux;
			nearest = scan;
		}
	}

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


/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

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
	int i;
	for(i=0; i < SHORT_TRACK_LENGTH; ++i) {
		int pos = TRACK_LENGTH * i / SHORT_TRACK_LENGTH;
		short_a[i] = track_A[pos] >> 3;
		short_b[i] = track_B[pos] >> 3;
		short_pos[i] = pos;
	}
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

void logEncoderPosition(long *_encPos)
{
	a = *_AN0;
	b = *_AN1;

	TMR1 = 0;
	// init search variables
	long_dist = 0x7FFFFFFF;
	nearest = -1;

	// Coarse position approximation: track is search in inc1 steps 
	for(scan=0;scan<TRACK_LENGTH;scan+=( TRACK_LENGTH/8 +1 )) {
		int error_a = a-track_A[scan];
		int error_b = b-track_B[scan];
		long_d_aux = __builtin_mulss(error_a, error_a) + __builtin_mulss(error_b, error_b);
		if(long_d_aux < long_dist) {
			long_dist = long_d_aux;
			nearest = scan;
		}
	}

	// assert(TRACK_LENGTH/8>256);
	long int long_d_x;
	long int long_d_y;

	int inc = 256;

	int nearest_less_inc = nearest-inc;
	if(nearest_less_inc<0) {
		nearest_less_inc += TRACK_LENGTH;
	}

	int nearest_plus_inc = nearest+inc;
	if(nearest_plus_inc>TRACK_LENGTH) {
		nearest_plus_inc -= TRACK_LENGTH;
	}

	while(1) {
		unsigned int xa;
		unsigned int xb;
		unsigned int ya;
		unsigned int yb;
		xa = a-track_A[nearest_less_inc];
		xb = b-track_B[nearest_less_inc];
		ya = a-track_A[nearest_plus_inc];
		yb = b-track_B[nearest_plus_inc];
		long_d_x = __builtin_mulss(xa,xa) + __builtin_mulss(xb,xb);
		long_d_y = __builtin_mulss(ya,ya) + __builtin_mulss(yb,yb);

		inc >>= 1;
		if(inc) {
			if(long_d_x < long_d_y) {
				nearest_plus_inc = nearest;
				nearest -= inc;
				if(nearest < 0) {
					nearest += TRACK_LENGTH;
				}
			} else {
				nearest_less_inc = nearest;
				nearest += inc;
				if(nearest >= TRACK_LENGTH) {
					nearest -= TRACK_LENGTH;
				}
			}
		} else {
			unsigned int za = a-track_A[nearest];
			unsigned int zb = b-track_B[nearest];
			long_dist = __builtin_mulss(za,za) + __builtin_mulss(zb,zb);
			if(long_d_x < long_dist) {
				nearest = nearest_less_inc;
			} else if(long_d_y < long_dist) {
				nearest = nearest_plus_inc;
			} /* else {
				nearest = nearest;
			} */
			break;
		}
	}

	if(last_nearest>LAP_SCAN_THRESHOLD_HIGH) {
		if(nearest<LAP_SCAN_THRESHOLD_LOW) {
			// --laps;	// for encoder-motor alternate direction
			++laps;  // for encoder-motor same direction
		}
	} else if(last_nearest<LAP_SCAN_THRESHOLD_LOW) {
		if(nearest>LAP_SCAN_THRESHOLD_HIGH) {
			// ++laps;	// for encoder-motor alternate direction
			--laps;  // for encoder-motor same direction
		}
	} 

	last_nearest = nearest;
	encoderAbsolutePosition = (laps * TRACK_LENGTH) + nearest;
	*_encPos = encoderAbsolutePosition;
}
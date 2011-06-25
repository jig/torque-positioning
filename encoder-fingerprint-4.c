/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

#include <math.h>
#include "encoder-fingerprint.h"

unsigned int track_A[1024];
unsigned int track_B[1024];

#define _SHORT_TRACK_LENGTH 10

#define COARSEINC 100

#define _TRACK_LENGTH (sizeof track_A/sizeof track_A[0])
#define INC1 (_TRACK_LENGTH / _SHORT_TRACK_LENGTH)
#define INC2 10
#define _LAST_COARSE1_TRACK_POSITION ((_TRACK_LENGTH/INC1)*INC1)
#define _LAST_COARSE2_TRACK_POSITION ((_TRACK_LENGTH/INC2)*INC2)

#define _LAP_SCAN_THRESHOLD_LOW (_TRACK_LENGTH/3)
#define _LAP_SCAN_THRESHOLD_HIGH (_TRACK_LENGTH*2/3)

const unsigned int coarseInc = COARSEINC;
const unsigned int inc1 = INC1;
const unsigned int inc2 = INC2;
const unsigned int TRACK_LENGTH = _TRACK_LENGTH;
const unsigned int SHORT_TRACK_LENGTH = _SHORT_TRACK_LENGTH;
const unsigned int LAST_COARSE1_TRACK_POSITION = _LAST_COARSE1_TRACK_POSITION;
const unsigned int LAST_COARSE2_TRACK_POSITION = _LAST_COARSE2_TRACK_POSITION;
const unsigned int LAP_SCAN_THRESHOLD_LOW = _LAP_SCAN_THRESHOLD_LOW;
const unsigned int LAP_SCAN_THRESHOLD_HIGH = _LAP_SCAN_THRESHOLD_HIGH;

int short_a[_SHORT_TRACK_LENGTH];
int short_b[_SHORT_TRACK_LENGTH];
int short_pos[_SHORT_TRACK_LENGTH];

// calculated values for track

const int MEAN_A = (961+254)/2; //683; //660; //619;
const int MEAN_B = (739+167)/2; //524; //512; //495;
const int SEMIAXE_A = (961-254)/2; //246; //240; //360;
const int SEMIAXE_B = (739-167)/2; //210; //305;
/*
const int MEAN_A = (1012+232)/2; //683; //660; //619;
const int MEAN_B = (756+176)/2; //524; //512; //495;
const int SEMIAXE_A = (1012-232)/2; //246; //240; //360;
const int SEMIAXE_B = (756-176)/2; //210; //305;
*/

#define PI 3.14159265358979323846

void configTrack() {
	int i;

	for(i=0;i<_TRACK_LENGTH;++i) {
		track_A[i] = MEAN_A + SEMIAXE_A * sin(2 * PI * ((double)i)/1024.0);
		track_B[i] = MEAN_B + SEMIAXE_B * cos(2 * PI * ((double)i)/1024.0);
	}
}

void configShortTrack() {
	int i;
	for(i=0; i < SHORT_TRACK_LENGTH; ++i) {
		int pos = TRACK_LENGTH * i / SHORT_TRACK_LENGTH;
		short_a[i] = track_A[pos] >> 3;
		short_b[i] = track_B[pos] >> 3;
		short_pos[i] = pos;
	}
}

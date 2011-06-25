/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

long pid_deltaError = 0;
long pid_lastPosError = 0;
long pid_integralError = 0;

#define SHIFT_DECIMALS 18
// Ku=20000
// Tu=4000
long pid_attr_d = 500000;
long pid_attr_p = 30000;
long pid_attr_i = 1;//2;
long pid_attr_m = 1;

long pid_attr_dutymin;
long pid_attr_dutymax;
long pid_attr_meanErrorMax; 

void configPid(unsigned int dutymin, unsigned int dutymax, unsigned meanErrorMax) {
	pid_attr_dutymin = dutymin;
	pid_attr_dutymax = dutymax;
	pid_attr_meanErrorMax = meanErrorMax;
}

int errorGap = 0;
int errorGapStop = 5;
int errorLog[1024];
int* errorLogPointer = errorLog;
int* errorLogLastPointer = errorLog + (sizeof errorLog/sizeof errorLog[0]);
int actionLog[1024];
int* actionLogPointer = actionLog;
int* actionLogLastPointer = actionLog + (sizeof actionLog/sizeof actionLog[0]);

void pid_Action(long posError, int targetDecaAngle, int *decaAngle, unsigned int *power) {
#define SHIFT_DECIMALS_HALF_PERIOD (1L<<(SHIFT_DECIMALS-1))
	// long lastDeltaError = pid_deltaError;

	pid_deltaError = posError-pid_lastPosError;
	pid_lastPosError = posError;

	long action;
	action = 
		posError * pid_attr_p +
		pid_deltaError * pid_attr_d +
		pid_integralError * pid_attr_i;

	pid_integralError = pid_integralError + posError;

	action+=SHIFT_DECIMALS_HALF_PERIOD;
	action>>=SHIFT_DECIMALS;

	if(errorGap==0) {
		errorGap = errorGapStop;
		*actionLogPointer++ = action;
		*errorLogPointer++ = posError;
		if(errorLogPointer == errorLogLastPointer) {
			errorLogPointer = errorLog;
			actionLogPointer = actionLog;
		}
	} else {	
		--errorGap;
	}
	if(action > 0) {
		*decaAngle = targetDecaAngle	+ 900; // inverted encoder
		// *decaAngle = targetDecaAngle - 900; // direct encoder
	} else if(action < 0) {
		action = -action;
		*decaAngle = targetDecaAngle - 900;  // inverted encoder
		// *decaAngle = targetDecaAngle + 900; // direct encoder
	} else {
		*decaAngle = targetDecaAngle;
	}

	if(action > pid_attr_dutymax) {
		*power = pid_attr_dutymax;
	} else if(action < pid_attr_dutymin) {
		if(action == 0) {
			*power = 0;
		} else {
			*power = action + pid_attr_dutymin;
		}
	} else {
		*power = action + pid_attr_dutymin;
	}
}

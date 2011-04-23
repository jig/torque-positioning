/****************************************************************/
/** Author: Jordi Íñigo Griera                                 **/
/** Mail: a s t r o j i g on g m a i l dot c o m               **/
/** GNU Public Licence v3                                      **/
/** Doc: see http://code.google.com/p/jig-torque-positioning   **/
/****************************************************************/

long pid_deltaError = 0;
long pid_lastPosError = 0;
long pid_meanError = 0;

/*float pid_attr_p = 10.0;
float pid_attr_i = 0.0;
float pid_attr_d = 100.0;
float pid_attr_m = 0.0;*/
int pid_attr_p = 1;
int pid_attr_i = 1;
int pid_attr_d = 100;
int pid_attr_m = 0;

int pid_attr_dutymin = 0;
int pid_attr_dutymax = 20000;
int targetDecaAngle;

void pid_Action(long posError, int *decaAngle, unsigned int *power) {
	// long lastDeltaError = pid_deltaError;

	pid_deltaError = posError-pid_lastPosError;
	pid_lastPosError = posError;

	long action;
	action = 
		posError * pid_attr_p +
		pid_deltaError * pid_attr_d +
		+ pid_meanError * pid_attr_i;
	pid_meanError = pid_meanError * pid_attr_m + posError;

	// 5000 ~ PWM_MAXDUTY / attr_i
	if(pid_meanError > 10000) {
		pid_meanError = 10000;
	} else if(pid_meanError < -10000) {
		pid_meanError = -10000;
	}

	// doblo la derivada
	/*float action = 
		(lastDeltaError + pid_deltaError) * pid_attr_d 
		+ posError * pid_attr_p 
		+ pid_meanError * pid_attr_i;
	pid_meanError = pid_meanError * pid_attr_m + posError;

	// 5000 ~ PWM_MAXDUTY / attr_i
	if(pid_meanError>10000.0) {
		pid_meanError=10000.0;
	} else if(pid_meanError<-10000.0) {
		pid_meanError=-10000.0;
	}

	if(action>0.0) {
		// *decaAngle = targetDecaAngle	+ 900; // inverted encoder
		*decaAngle = targetDecaAngle - 900; // direct encoder
	} else if(action<0.0) {
		action = -action;
		// *decaAngle = targetDecaAngle - 900;  // inverted encoder
		*decaAngle = targetDecaAngle + 900; // direct encoder
	} else {
		*decaAngle = targetDecaAngle;
	}*/

	if(action > 0) {
		// *decaAngle = targetDecaAngle	+ 900; // inverted encoder
		*decaAngle = targetDecaAngle - 900; // direct encoder
	} else if(action < 0) {
		action = -action;
		// *decaAngle = targetDecaAngle - 900;  // inverted encoder
		*decaAngle = targetDecaAngle + 900; // direct encoder
	} else {
		*decaAngle = targetDecaAngle;
	}

	if(action > pid_attr_dutymax) {
		*power = pid_attr_dutymax;
	} else if(action < pid_attr_dutymin) {
		*power = pid_attr_dutymin;
	} else {
		*power = action;
	}
}

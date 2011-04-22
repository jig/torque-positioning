long pid_deltaError = 0;
long pid_lastPosError = 0;
long pid_meanError = 0;

float pid_attr_p = 0.1;
float pid_attr_i = 0.0;
float pid_attr_d = 0.0;
float pid_attr_m = 0.0;
int pid_attr_dutymin = 0;
int pid_attr_dutymax = 20000;
float targetAngle;

void pid_Action(long posError, float *angle, float *power) {
	long lastDeltaError = pid_deltaError;

	pid_deltaError = posError-pid_lastPosError;
	pid_lastPosError = posError;

	// doblo la derivada
	float action = 
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
		// *angle = targetAngle	+ 90.0; // inverted encoder
		*angle = targetAngle - 90.0; // direct encoder
	} else if(action<0.0) {
		action = -action;
		// *angle = targetAngle - 90.0;  // inverted encoder
		*angle = targetAngle + 90.0; // direct encoder
	} else {
		*angle = targetAngle;
	}

	/*if( (deltaError>=0 && posError>0) || (deltaError<=0 && posError<0) ) {
		power = PWM_MAXDUTY;
	} else*/ 
	{
		if((long)action<pid_attr_dutymax) {
			*power = action;
		} else {
			*power = pid_attr_dutymax;
		}

		if(*power>0 && *power<pid_attr_dutymin) {
			*power = pid_attr_dutymin;
		}	
	}
}
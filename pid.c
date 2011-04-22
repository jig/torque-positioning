long pid_deltaError = 0;
long pid_lastPosError = 0;
long pid_meanError = 0;

float pid_attr_p;
float pid_attr_i;
float pid_attr_d;
float pid_attr_m;
int pid_attr_dutymin;
int pid_attr_dutymax;
float targetAngle;

void pid_Action(long posError, float *angle, float *power) {
	int lastDeltaError = pid_deltaError;

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
	} else if(action<0) {
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
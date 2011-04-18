void configPwm(unsigned int period);

void setPwm1(unsigned int dutycycle, char updatedisable);
void setPwm2(unsigned int dutycycle, char updatedisable);
void setPwm3(unsigned int dutycycle, char updatedisable);

void setPwm(int channel, long dutycycle);
long getPwm(int channel);
void setPwmAll(long duty1, long duty2, long duty3);

unsigned int getPwmPeriod();


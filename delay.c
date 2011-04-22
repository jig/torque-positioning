#include "delay.h"

void delay_s(unsigned int seconds) {
	int i, j, k;
	for(k=0;k<seconds;++k) {
		for(j=0;j<1000;++j) {
			for(i=0;i<4400;++i) {
				__builtin_nop();
			}
		}
	}
}

void delay_ms(unsigned int milliseconds) {
	int i, k;
	for(k=0;k<milliseconds;++k) {
		for(i=0;i<4400;++i) {
			__builtin_nop();
		}
	}
}

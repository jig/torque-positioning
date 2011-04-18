#include <p33Fxxxx.h>
#define    FCY 40000000UL
#include <libpic30.h>
#include <assert.h>
#include "adc.h"

// Number of locations for ADC buffer = 14 (AN0 to AN13) x 8 = 112 words
// Align the buffer to 128 words or 256 bytes. This is needed for peripheral indirect mode
// unsigned int BufferA[MAX_CHNUM][SAMP_BUFF_SIZE] __attribute__((space(dma),aligned(256)));
// unsigned int BufferB[MAX_CHNUM][SAMP_BUFF_SIZE] __attribute__((space(dma),aligned(256)));

unsigned int BufferAdc[MAX_CHNUM] __attribute__((space(dma),aligned(64)));

void configDma0(void);

void adc_start() {
	DMA0CONbits.CHEN=1; // Enable DMA
}

void adc_stop() {
	AD1CON1bits.ADON = 0;           // Turn off the A/D converter   
}

/*
	configAdc uses simultaneous sampling of CH0 and CH1; 
	connects AN0 to CH1 and AN1 to CH0 and
	disables or other analog inputs. Yes, 0 to 1 and 1 to 0, it is not possible
	to use a (more understandable) mapping AN0->CH0 and AN1->CH1...
*/
void configAdc() {
	AD1CON1bits.FORM   = 0;		// Data Output Format: 0 Unsigned Integer; 0b10 Fractional
	AD1CON1bits.SSRC   = 0b111;	// Sample Clock Source: internal timer (auto-convert)
	AD1CON1bits.ASAM   = 1;		// ADC Sample Control: Sampling begins immediately after conversion
	AD1CON1bits.AD12B  = 0;		// 10-bit ADC operation
	AD1CON1bits.SIMSAM = 1;		// Simultaneous sampling
	AD1CON1bits.ADDMABM = 0;	// DMA buffers are built in scatter/gather mode
	
	AD1CON2bits.CSCNA = 0;		// NO Scan Input Selections for CH0+ during Sample A bit
	AD1CON2bits.CHPS  = 1;		// Converts CH0 and CH1 (ep! do not confuse with AN0 and AN1 (see note)
	// AD1CON2bits.VCFG  = 0b000;	// use AVDD and AGND
	AD1CON2bits.BUFM  = 0;		// fill from the begining
	AD1CON2bits.ALTS  = 0;		// only sample MUX A is used
	AD1CON2bits.SMPI  = 0;		// no alternate mode and no scan mode, then SMPI = 0
	
	AD1CON3bits.ADRC = 0;		// ADC Clock is derived from Systems Clock (0; 1=internal clock)
	AD1CON3bits.SAMC = 1;		// 1 TAD; auto sample time bits
	AD1CON3bits.ADCS = 2; //3; //63;// 3=>TAD = 100 ns > 75 ns
	
	AD1CON4bits.DMABL = LOG_SAMP_BUFF_SIZE;	// Each buffer contains 1 words (0=log(SAMP_BUFF_SIZE))

	AD1PCFGL=0xFFFF;

#ifdef DEFAULT_VREF
	// In this case, we use AN1 and AN0 as feed for CH0 and CH1 respectively
	// and we use Vdd and Vss as feed for VrefH and VrefL respectively
	AD1CON2bits.VCFG  = 0b000;	// use AVDD and AGND
	AD1PCFGLbits.PCFG0 = 0;         // AN0 as analog input
	AD1PCFGLbits.PCFG1 = 0;			// AN1 as analog input
	TRISA |= 0b11;					// RA0 and RA1 as pin input

	// Channel 0: Aternate Sample A
	AD1CHS0bits.CH0NB = 0;		// negative input is VREFL
	AD1CHS0bits.CH0SB = 1;		// CH0 is AN1
	// Channel 0: Aternate Sample B (not used here but...)
	AD1CHS0bits.CH0NA = 0;		// negative input is VREFL
	AD1CHS0bits.CH0SA = 1;		// CH0 is AN1

	// Channel 1, 2, 3: Aternate Sample A
	AD1CHS123bits.CH123NA = 0;	// negative input is VREFL
	AD1CHS123bits.CH123SA = 0;	// positive input for CH1 is AN0 (and for CH2 is AN1, for CH3 is AN2)

	// Channel 1, 2, 3: Aternate Sample B (not used here but...)
	AD1CHS123bits.CH123NB = 0;	// negative input is VREFL
	AD1CHS123bits.CH123SB = 0;	// positive input for CH1 is AN0 (and for CH2 is AN1, for CH3 is AN2)
#else
	// In this case, we use AN2 and AN3 as feed for CH0 and CH1 respectively
	// and we use AN0 and AN1 as feed for VrefH and VrefL respectively
	AD1CON2bits.VCFG  = 0b011;	// use VrefH and VrefL
	AD1PCFGLbits.PCFG0 = 0;         // AN0 as analog input
	AD1PCFGLbits.PCFG1 = 0;			// AN1 as analog input
	AD1PCFGLbits.PCFG2 = 0;			// AN2 as analog input
	AD1PCFGLbits.PCFG3 = 0;			// AN3 as analog input
	TRISA |= 0b11;					// RA0 and RA1 as pin inputs
	TRISB |= 0b11;					// RA2 and RA3 as pin inputs

	// Channel 0: Aternate Sample A
	AD1CHS0bits.CH0NB = 0;		// negative input is VREFL
	AD1CHS0bits.CH0SB = 2;		// CH0 is AN2
	// Channel 0: Aternate Sample B (not used here but...)
	AD1CHS0bits.CH0NA = 0;		// negative input is VREFL
	AD1CHS0bits.CH0SA = 2;		// CH0 is AN2

	// Channel 1, 2, 3: Aternate Sample A
	AD1CHS123bits.CH123NA = 0;	// negative input is VREFL
	AD1CHS123bits.CH123SA = 1;	// 1 = CH1 positive input is AN3, CH2 positive input is AN4, CH3 positive input is AN5

	// Channel 1, 2, 3: Aternate Sample B (not used here but...)
	AD1CHS123bits.CH123NB = 0;	// negative input is VREFL
	AD1CHS123bits.CH123SB = 1;	// 1 = CH1 positive input is AN3, CH2 positive input is AN4, CH3 positive input is AN5
#endif
	//AD1CSSH/AD1CSSL: A/D Input Scan Selection Register
# 	ifdef AD1CSSH 
	AD1CSSH = 0x0000;                       
#endif
	AD1CSSL = 0x0000; // do no scan please
		
	IFS0bits.AD1IF   = 0;           // Clear the A/D interrupt flag bit
	IEC0bits.AD1IE   = 0;           // Do Not Enable A/D interrupt 
	
	configDma0();
	AD1CON1bits.ADON = 1;
	
	// adc_start();
}

void configDma0(void) {
		DMA0CONbits.SIZE = 0; // 0: word size transfer
		DMA0CONbits.DIR = 0; // 0: from I/O device (e.g. ADC) to RAM
		DMA0CONbits.HALF = 0; // 0: interrupt after all transfer done... but don't using interrupts here
		DMA0CONbits.NULLW = 0; // 0: normal operation (not wanting to pass zeroes over
        DMA0CONbits.AMODE = 2; // Configure DMA for Peripheral indirect mode: ADC governs addressing
        // DMA0CONbits.MODE  = 1; // 01 = One-Shot, Ping-Pong mode disabled
		DMA0CONbits.MODE  = 0; // 00 = continuous, Ping-Pong mode disabled

        DMA0PAD=(volatile unsigned int)&ADC1BUF0; // the address of what we transfer

        DMA0CNT = SAMP_BUFF_SIZE-1;                                      
        DMA0REQ = 13; // Select ADC1 as DMA Request source

        // DMA0STA = __builtin_dmaoffset(BufferA);         
        // DMA0STB = __builtin_dmaoffset(BufferB);
		DMA0STA = __builtin_dmaoffset(BufferAdc);         
		DMA0STB = 0; // not used

        IFS0bits.DMA0IF = 0; //Clear the DMA interrupt flag bit
    	IEC0bits.DMA0IE = 0; //Set the DMA interrupt enable bit

        DMA0CONbits.CHEN=1; // Disable DMA
}


void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
        // adc_stop();
        IFS0bits.DMA0IF = 0;            // Clear the DMA0 Interrupt Flag
}


unsigned int adc_get_channel(int i)
{
	return BufferAdc[i];
}


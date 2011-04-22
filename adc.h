// Define Message Buffer Length for ECAN1/ECAN2
#define  MAX_CHNUM 4 // Highest Analog input number in Channel Scan (AN3 => 4)
#define  LOG_SAMP_BUFF_SIZE 0
#define  SAMP_BUFF_SIZE (1<<LOG_SAMP_BUFF_SIZE)               // Size of the input buffer per analog input

extern unsigned int BufferAdc[MAX_CHNUM];

void configAdc();

// #define getAdc() (ADC1BUF0)

unsigned int adc_get_channel(int i);

// unsigned int getAdc();

void adc_start();

// #define adc_get_channel(i) (BufferAdc[i][0])

// #define DEFAULT_VREF
#ifdef DEFAULT_VREF
#	define AN0 0
#	define AN1 1
#else
#	define AN0 2
#	define AN1 3
#endif

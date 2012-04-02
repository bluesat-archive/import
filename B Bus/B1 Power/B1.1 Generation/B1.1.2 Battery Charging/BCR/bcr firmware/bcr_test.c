#include "bcr_test.h"

uint8_t main (void)
{
	uart_init (9600);
	setDAC (DAC_ALL, 0x3FF);
	testADC (void);

	return 0;
}

// Reads ADC values and transmits data over UART
void testADC (void)
{
	uint16_t adcVals[8];
	uint8_t i;
	uint8_t outputString[50];

	// Get ADC values
	for (i=0; i < 8; i++)
	{
		adcVals[i] = readADC (ADC_A, i);
		sprintf (outputString, "ADC_A channel %d = %d\n", i, adcVals[i];
		uart_puts (outputString);
	}
	for (i=0; i < 8; i++)
	{
		adcVals[i] = readADC (ADC_B, i);
		sprintf (outputString, "ADC_B channel %d = %d\n", i, adcVals[i];
		uart_puts (outputString);
	}
	return;
}

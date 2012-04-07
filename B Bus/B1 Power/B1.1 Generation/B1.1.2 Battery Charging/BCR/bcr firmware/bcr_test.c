#include <stdio.h>
#include <avr/io.h>

#include "bcr.h"
#include "bcr_test.h"

void testADC (void)
{
	uint16_t adcVals[8];
	uint8_t i;
	uint8_t outputString[50];

	uart_init (9600);

	// Get ADC values
	for (i=0; i < 8; i++)
	{
		adcVals[i] = readADC (ADC_A, i);
		sprintf (outputString, "ADC_A channel %d = %d\n", i, adcVals[i]);
		uart_puts (outputString);
	}
	for (i=0; i < 8; i++)
	{
		adcVals[i] = readADC (ADC_B, i);
		sprintf (outputString, "ADC_B channel %d = %d\n", i, adcVals[i]);
		uart_puts (outputString);
	}
	return;
}

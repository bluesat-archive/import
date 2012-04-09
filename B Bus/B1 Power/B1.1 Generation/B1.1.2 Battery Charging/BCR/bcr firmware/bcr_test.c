#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "bcr.h"
#include "bcr_test.h"

#define MAXSTRLEN 50

// Define mystdout
static FILE mystdout = FDEV_SETUP_STREAM (uartPutChar, NULL, _FDEV_SETUP_WRITE);

void bcrTest (void)
{

	uartInit (MYUBRR);
	//while (1)
	//{
		printf ("Hello, world\n");
	//	_delay_ms (1000);
	//}
}

void testADC (void)
{
	uint16_t adcVals[8];
	uint8_t i;

	// Get ADC values
	for (i=0; i < 8; i++)
	{
		adcVals[i] = readADC (ADC_A, i);
		printf ("ADC_A channel %d = %d\n", i, adcVals[i]);
	}
	for (i=0; i < 8; i++)
	{
		adcVals[i] = readADC (ADC_B, i);
		printf ("ADC_B channel %d = %d\n", i, adcVals[i]);
	}
}

void uartInit (uint16_t ubrr)
{
	// Set baud rate
	UBRR0H = (uint8_t) (ubrr >> 8);
	UBRR0H = (uint8_t) ubrr;
	// Enable rx and tx
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	
	// Set frame format: 8 data, 1 stop bit
  UCSR0C = (0 << USBS0) | (3 << UCSZ00);

	// stdout required for printf
	stdout = &mystdout;
}

int uartPutChar (char c, FILE *stream)
{
	if (c == '\n')
		uartPutChar ('\r', stream);

	while ( !(UCSR0A & (1 << UDRE0) ))
			;

	UDR0 = c;
	return 0;
}

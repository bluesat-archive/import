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

	_delay_ms(1);

	testADC ();
}

void testADC (void)
{
	uint16_t adcVal;
	uint8_t i;

	// Print ADC values to uart
	printf("Printing ADC values:\n\n");
	printf("ADC_A:\n");
	for (i=0; i < 8; i++)
	{
		adcVal = readADC (ADC_A, i);
		printf ("  Ch %d = %d mV\n", i, adcVal/2);
	}
	uartPutChar ('\n', stdout);
	printf("ADC_B:\n");
	for (i=0; i < 8; i++)
	{
		adcVal = readADC (ADC_B, i);
		printf ("  Ch %d = %d mV\n", i, adcVal/2);
	}
	uartPutChar ('\n', stdout);
}

void uartInit (uint16_t ubrr)
{
	// Set baud rate
	UBRR0H = ubrr >> 8;
	UBRR0L = ubrr;
	// Enable rx and tx
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	
	// Set frame format: 8 data, 1 stop bit
  UCSR0C = (3 << UCSZ00);

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

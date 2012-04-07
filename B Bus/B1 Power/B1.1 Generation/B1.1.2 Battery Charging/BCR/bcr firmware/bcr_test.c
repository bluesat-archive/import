#include <stdio.h>
#include <avr/io.h>

#include "bcr.h"
#include "bcr_test.h"

#define MAXSTRLEN 50

void testADC (void)
{
	uint16_t adcVals[8];
	uint8_t i;
	char outputString[MAXSTRLEN];

	uartInit (UART_BAUD);

	// Get ADC values
	for (i=0; i < 8; i++)
	{
		adcVals[i] = readADC (ADC_A, i);
		sprintf (outputString, "ADC_A channel %d = %d\n", i, adcVals[i]);
		uartPuts (outputString);
	}
	for (i=0; i < 8; i++)
	{
		adcVals[i] = readADC (ADC_B, i);
		sprintf (outputString, "ADC_B channel %d = %d\n", i, adcVals[i]);
		uartPuts (outputString);
	}
	return;
}

void uartInit (uint16_t baudRate)
{
	// Set baud rate
	UBRR0H = (uint8_t) (baudRate >> 8);
	UBRR0H = (uint8_t) baudRate;
	// Enable rx and tx
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Set frame format: 8 data, 2 stop bits
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void uartTransmit (uint8_t data)
{
	while ( !(UCSR0A & (1 << UDRE0) ))
			;

	// Put data in buffer
	UDR0 = data;
}

void uartPuts (char* string)
{
	uint8_t i;
	for(i=0; i < MAXSTRLEN && string[i]!='\0'; i++)
		uartTransmit ((uint8_t)string[i]);
}

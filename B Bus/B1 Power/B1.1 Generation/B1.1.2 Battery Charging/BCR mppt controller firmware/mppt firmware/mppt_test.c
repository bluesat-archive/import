//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 mppt_test.c : Code necessary for testing and debugging
//

void uartInit (void)
{
	// Set baud rate
	USART
	
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
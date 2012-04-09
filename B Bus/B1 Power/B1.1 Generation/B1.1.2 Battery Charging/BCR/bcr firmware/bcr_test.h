//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 For AVR ATMEGA328P microcontroller
//
//	 bcr_test.h : Allows testing of BCR over UART serial interface
//

#include <stdio.h>

#define UART_BAUD 9600
#define MYUBRR 51  //(((FOSC * 10)/(16L * UART_BAUD) + 5)/10 - 1)

void bcrTest (void);

void testADC (void);

void uartInit (uint16_t ubrr);

int uartPutChar (char c, FILE *stream);

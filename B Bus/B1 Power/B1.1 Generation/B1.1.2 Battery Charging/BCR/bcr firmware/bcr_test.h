//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 For AVR ATMEGA328P microcontroller
//
//	 bcr_test.h : Allows testing of BCR over UART serial interface
//

#define UART_BAUD 9600
#define MYUBRR 51			// MYUBRR = F_CPU/(16*UART_BAUD) - 1

void testADC (void);

void uartInit (uint16_t ubrr);

void uartTransmit (uint8_t data);

void uartPuts (char* string);

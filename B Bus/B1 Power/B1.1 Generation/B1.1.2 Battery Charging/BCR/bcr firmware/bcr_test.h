//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 For AVR ATMEGA328P microcontroller
//
//	 bcr_test.h : Allows testing of BCR over UART serial interface
//

#define UART_BAUD 416

void testADC (void);

void uartInit (uint16_t baudRate);

void uartTransmit (uint8_t data);

void uartPuts (char* string);

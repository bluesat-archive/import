//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 mppt_test.h : Code necessary for testing and debugging
//

#define UART_BAUD 9600
#define MYBSEL (((F_CPU * 10)/(16L * UART_BAUD) + 5)/10 - 1)

void uartInit (void);


int uartPutChar (char c, FILE *stream);
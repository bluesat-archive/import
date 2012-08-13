/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <stdio.h>
#include "core.h"

int main (void)
{
	sysclk_init();
	initGPIOdir();
	//ioport_set_pin_high(GPIO_SHDN_YN);
	uartInit(9600); // TODO: usart baudrate selection
	while (1)
	{
		printf("Harrow\n");
	}
}

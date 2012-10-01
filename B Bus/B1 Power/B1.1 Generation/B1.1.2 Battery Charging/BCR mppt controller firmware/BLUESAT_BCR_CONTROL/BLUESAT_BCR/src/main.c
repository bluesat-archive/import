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
#include "mppt.h"

status_code_t usart_serial_write_string(usart_if usart, char *str);
void print_adc(void);

int main (void)
{
	bcr_init(); //Sets up BCR
	
	/* BCRs default to enabled, uncomment lines below to disable */
	ioport_set_pin_high(SHDN_XP); //Disable BCR XP
	ioport_set_pin_high(SHDN_YP); //Disable BCR YP
	ioport_set_pin_high(SHDN_ZP); //Disable BCR ZP
	ioport_set_pin_high(SHDN_XM); //Disable BCR XM
	ioport_set_pin_high(SHDN_YM); //Disable BCR YM
	ioport_set_pin_high(SHDN_ZM); //Disable BCR ZM
	//ioport_set_pin_low(SYNC_EN);
	
	bcr_dac.ISET_ZM = 1023;
	bcr_dac_update();
	//print_adc();
	
	while (1)
	{		
		//mppt_cycle();
		print_adc();			
		delay_ms(500);
	}	
}

// Test DAC and ADC
void print_adc (void)
{
	char msg[40];
		
	sprintf(msg, "VREF=%umV\r\n", bcr_adc.VREF>>1);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "ISENSE_XP=%umA\r\n", bcr_adc.ISENSE_XP>>1);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "VSENSE_XP=%umV\r\n", (bcr_adc.VSENSE_XP>>1)*4);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "ISENSE_YP=%umA\r\n", bcr_adc.ISENSE_YP>>1);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "VSENSE_YP=%umV\r\n", (bcr_adc.VSENSE_YP>>1)*4);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "ISENSE_ZP=%umA\r\n", bcr_adc.ISENSE_ZP>>1);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "VSENSE_ZP=%umV\r\n", (bcr_adc.VSENSE_ZP>>1)*4);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "ISENSE_XM=%umA\r\n", bcr_adc.ISENSE_XM>>1);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "VSENSE_XM=%umV\r\n", (bcr_adc.VSENSE_XM>>1)*4);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "ISENSE_YM=%umA\r\n", bcr_adc.ISENSE_YM>>1);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "VSENSE_YM=%umV\r\n", (bcr_adc.VSENSE_YM>>1)*4);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "ISENSE_ZM=%umA\r\n", bcr_adc.ISENSE_ZM>>1);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "VGNDREF=%umV\r\n", bcr_adc.VGNDREF>>1);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "ADCB5=%umA\r\n", bcr_adc.ADCB5>>1);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "VSENSE_BAT=%umV\r\n", (bcr_adc.VSENSE_BAT>>1)*16);
	usart_serial_write_string(USART_SERIAL, msg);
	sprintf(msg, "ISENSE_LOAD=%umA\r\n", bcr_adc.ISENSE_LOAD>>1);
	usart_serial_write_string(USART_SERIAL, msg);
}

//Neat function to write a null terminating string
status_code_t usart_serial_write_string(usart_if usart, char *str)
{
	while (*str != '\0') {
		usart_serial_putchar(usart, *str);
		str++;
	}
	return STATUS_OK;
}
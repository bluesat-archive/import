/*
 * bcr.c
 *
 * Created: 4/09/2012 11:45:18 PM
 *  Author: Mitch
 */ 
#include <asf.h>

static void uart_init(void);

void bcr_init(void) {
	//Setup clock (8Mhz XOSC) and power control
	sysclk_init();
	pmic_init();
	
	//Start ADC sampling
	bcr_adc_init();
	
	//Setup DAC SPI
	bcr_dac_init();
	
	//Default ISETs to 512
	bcr_dac.ISET_ZM = 512;
	bcr_dac.ISET_YM = 512;
	bcr_dac.ISET_XM = 512;
	bcr_dac.ISET_ZP = 512;
	bcr_dac.ISET_YP = 512;
	bcr_dac.ISET_XP = 512;
	bcr_dac_update();
	
	//Setup shutdown pins, default to enabled.
	ioport_configure_pin(SHDN_XP, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	ioport_configure_pin(SHDN_YP, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	ioport_configure_pin(SHDN_ZP, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	ioport_configure_pin(SHDN_XM, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	ioport_configure_pin(SHDN_YM, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	ioport_configure_pin(SHDN_ZM, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	
	//Setup TimerBlox enable pin, default to enabled.
	ioport_configure_pin(SYNC_EN, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	
	//I2C stuff??
	//....
	
	//Enable FTDI UART
	#if defined(UART_ENABLE)
	uart_init();
	#endif
}



//Sets up UART broken out on FTDI (USARTE0)
static void uart_init(void) {
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTE, 3), IOPORT_DIR_OUTPUT
	| IOPORT_INIT_HIGH);
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTE, 2), IOPORT_DIR_INPUT);
	
	static usart_serial_options_t usart_options = {
		.baudrate = USART_SERIAL_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
	};

	usart_serial_init(USART_SERIAL, &usart_options);
}
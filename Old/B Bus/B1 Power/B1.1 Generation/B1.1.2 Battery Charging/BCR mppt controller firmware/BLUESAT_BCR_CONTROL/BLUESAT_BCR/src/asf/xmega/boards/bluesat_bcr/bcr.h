/*
 * bcr.h
 *
 * Created: 4/09/2012 11:42:31 PM
 *  Author: Mitch
 */ 

#ifndef BCR_H_
#define BCR_H_

#define DEBUG

//Comms functions
#include "bcr_comms.h"

//Main BCR init function (all you should need to call!)
void bcr_init(void);

void bcr_adc_init(void);
void bcr_dac_init(void);

//DAC update function (call after modifying bcr_dac)
void bcr_dac_update(void);

//ADC pins
struct bcr_adc_pins {
	uint16_t VREF;			//ADCA0
	uint16_t ISENSE_XP;		//ADCA1
	uint16_t VSENSE_XP;		//ADCA2
	uint16_t ISENSE_YP;		//ADCA3
	uint16_t VSENSE_YP;		//ADCA4
	uint16_t ISENSE_ZP;		//ADCA5
	uint16_t VSENSE_ZP;		//ADCA6
	uint16_t ISENSE_XM;		//ADCA7
	uint16_t VSENSE_XM;		//ADCB0
	uint16_t ISENSE_YM;		//ADCB1
	uint16_t VSENSE_YM;		//ADCB2
	uint16_t ISENSE_ZM;		//ADCB3
	uint16_t VSENSE_ZM;		//ADCB4
	uint16_t ADCB5;     	//ADCB5
	uint16_t VSENSE_BAT;	//ADCB6
	uint16_t ISENSE_LOAD;	//ADCB7
};
struct bcr_adc_pins bcr_adc;

//DAC output pins
struct bcr_dac_pins {
	uint16_t ISET_XP;	//V_OUTA
	uint16_t ISET_YP;	//V_OUTB
	uint16_t ISET_ZP;	//V_OUTC
	uint16_t NC1;		//V_OUTD
	uint16_t NC2;		//V_OUTE
	uint16_t ISET_XM;	//V_OUTF
	uint16_t ISET_YM;	//V_OUTG
	uint16_t ISET_ZM;	//V_OUTH
};
struct bcr_dac_pins bcr_dac;

//DAC spi pins and baud
#define DAC_SS IOPORT_CREATE_PIN(PORTC, 4)
#define DAC_MOSI IOPORT_CREATE_PIN(PORTC, 5)
#define DAC_MISO IOPORT_CREATE_PIN(PORTC,6)
#define DAC_SCK IOPORT_CREATE_PIN(PORTC, 7)
#define DAC_BAUD 1000000

//Shutdown pins
#define SHDN_XP IOPORT_CREATE_PIN(PORTF, 0)
#define SHDN_YP IOPORT_CREATE_PIN(PORTF, 1)
#define SHDN_ZP IOPORT_CREATE_PIN(PORTF, 2)
#define SHDN_XM IOPORT_CREATE_PIN(PORTF, 3)
#define SHDN_YM IOPORT_CREATE_PIN(PORTF, 4)
#define SHDN_ZM IOPORT_CREATE_PIN(PORTF, 5)
#define SYNC_EN IOPORT_CREATE_PIN(PORTF, 6)

//Enable FTDI UART or not?
#define UART_ENABLE

//External oscillator
#define BOARD_XOSC_HZ          8000000
#define BOARD_XOSC_TYPE        0x0B
#define BOARD_XOSC_STARTUP_US  500000

#define ADC_OFFSET 62

#endif /* BCR_H_ */
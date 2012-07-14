//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 mppt_core.c : Contains low level hardware specific functions
//
//	 For AVR ATxmega64A3
//

#include <avr/io.h>
#include "mppt_core.h"

// Set GPIO directions (for BCR shutdown pins and frequency synchronization enable pin)
void initGPIOdir (void)
{
	// Set BCR shutdown pins as outputs
	PORTF.DIR |= SHDN_XP_bm | SHDN_YP_bm | SHDN_ZP_bm | SHDN_XN_bm | SHDN_YN_bm | SHDN_ZN_bm;
	
	// Set frequency synchronization enable pin as output
	PORTF.DIR |= FSYNC_EN_bm;
}

// Initialize SPI bus for LTC1660 D/A converter
void initDAC (void)
{
	// Set SS, MOSI and SCK as outputs, MISO as input
	PORTC_DIR |=  DAC_SS_bm | DAC_MOSI_bm | DAC_SCK_bm;
	PORTC_DIR &= ~MISO_bm;

	// Enable SPI as master, set clock rate F_clk/16
	SPIC_CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESCALER0_bm;
}

// Returns DAC address for given panel designator
uint8_t dacAddress (uint8_t panelNo)
{
	switch (panelNo)
	{
		case PANEL_XP :
			return DAC_XP;
			break;
		case PANEL_YP :
				return DAC_YP;
			break;
		case PANEL_ZP :
			return DAC_ZP;
			break;
		case PANEL_XN :
			return DAC_XN;
			break;
		case PANEL_YN :
			return DAC_YN;
			break;
		case PANEL_ZN :
			return DAC_ZN;
			break;
		default :
			return 0;
		break;
	}
}

// Send 10 bit data value to addressed channel of LTC1660 D/A converter
void setDAC (uint8_t address, uint16_t value)
{
	uint16_t dacWord = (value << 2) | (((uint16_t) address) << 12);

	// Pull chip select low
	PORTC &= ~(DAC_SS_bm);
	_delay_us (1);

	// Send word	
	SPIC_DATA = (uint8_t) (dacWord >> 8);
	while (!(SPIC_STATUS & SPI_IF_bm));
	SPIC_DATA = (uint8_t) dacWord;
	while (!(SPIC_STATUS & SPI_IF_bm);

	// Pull chip select high
	PORTC |= DAC_SS_bm;
}
s
// Initialize internal ADC
void initADC (void)
{
	PORTA.DIR = 0x00;										// Configure PORTA as inputs
	PORTB.DIR = 0x00;										// Configure PORTB as inputs
	
	ADCA.CTRLA = ADC_ENABLE_bm;
	ADCB.CTRLA = ADC_ENABLE_bm;
	
	ADCA.CTRLB = ADC_RESOLUTION_12BIT_gc;
	ADCB.CTRLB = ADC_RESOLUTION_12BIT_gc;
	
	ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc				// Peripheral clk/512
	ADCB.PRESCALER = ADC_PRESCALER_DIV512_gc				// Peripheral clk/512
	
	ADCA.REFCTRL = ADC_REFSEL_AREFA_gc;
	ADCB.REFCTRL = ADC_REFSEL_AREFA_gc;
	
	ADCA.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;		// Configure ADCA and ADCb in single ended input mode
	ADCB.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;		// Only using CH0, no pipelining
}

// Reads 12 bit data value from addressed channel of internal ADC
// Channel addresses are numbered from 0 to 15 (ADCA is 0 to 7 and ADCB is 8 to 15)
uint16_t readADC (uint8_t channel)
{
	uint16_t result = 0;
	
	if (channel <= 7)
	{
		ADCA.CH0.MUXCTRL = (channel << 3);
		while(!ADCA.CH0.INTFLAGS);
		Result = ADCA.CH0RES;
	}
	else
	{
		ADCB.CH0.MUXCTRL = ( (channel - 7) << 3);
		while(!ADCB.CH0.INTFLAGS);
		Result = ADCB.CH0RES;
	}
	
	return result;	
}

// Enable/disable 700 kHz frequency synchronization
// syncEnable = 1 to enable, 0 to disable
void freqSync (uint8_t syncEnable)
{
	if (syncEnable)
		PORTF.OUTSET |=  FSYNC_EN_bm;
	else
		PORTF.OUTSET &= ~FSYNC_EN_bm;
}
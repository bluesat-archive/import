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
	// Initialize to disabled
	PORTF.DIR    |= SHDN_XP_bm | SHDN_YP_bm | SHDN_ZP_bm | SHDN_XN_bm | SHDN_YN_bm | SHDN_ZN_bm;
	PORTF.OUTSET |= SHDN_XP_bm | SHDN_YP_bm | SHDN_ZP_bm | SHDN_XN_bm | SHDN_YN_bm | SHDN_ZN_bm;
	
	// Set frequency synchronization enable pin as output
	PORTF.DIR |= FSYNC_EN_bm;
}

// Initialize SPI bus for LTC1660 D/A converter
void initDAC (void)
{
	// Set SS, MOSI and SCK as outputs, MISO as input
	PORTC_DIR |=  DAC_SS_bm | DAC_MOSI_bm | DAC_SCK_bm;
	PORTC_DIR &= ~DAC_MISO_bm;

	// Enable SPI as master, set clock rate F_clk/16
	SPIC_CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESCALER0_bm;
}

// Returns DAC address for given panel designator
uint8_t dacChannel (uint8_t panelNo)
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

// Returns ADC voltage channel for given panel designator
uint8_t adcVoltageChannel (uint8_t panelNo)
{
	switch (panelNo)
	{
		case PANEL_XP :
			return PANEL_XP_VOLTAGE;
			break;
		case PANEL_YP :
			return PANEL_YP_VOLTAGE;
			break;
		case PANEL_ZP :
			return PANEL_ZP_VOLTAGE;
			break;
		case PANEL_XN :
			return PANEL_XN_VOLTAGE;
			break;
		case PANEL_YN :
			return PANEL_YN_VOLTAGE;
			break;
		case PANEL_ZN :
			return PANEL_ZN_VOLTAGE;
			break;
		default :
			return 0;
		break;
	}	
}

// Returns ADC current channel for given panel designator
uint8_t adcCurrentChannel (uint8_t panelNo)
{	switch (panelNo)
	{
		case PANEL_XP :
			return PANEL_XP_CURRENT;
			break;
		case PANEL_YP :
			return PANEL_YP_CURRENT;
			break;
		case PANEL_ZP :
			return PANEL_ZP_CURRENT;
			break;
		case PANEL_XN :
			return PANEL_XN_CURRENT;
			break;
		case PANEL_YN :
			return PANEL_YN_CURRENT;
			break;
		case PANEL_ZN :
			return PANEL_ZN_CURRENT;
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
	PORTC.OUTSET &= ~(DAC_SS_bm);
	_delay_us (1);

	// Send word
	SPIC.DATA = (uint8_t) (dacWord >> 8);
	while (!(SPIC.STATUS & SPI_IF_bm));
	SPIC.DATA = (uint8_t) dacWord;
	while (!(SPIC.STATUS & SPI_IF_bm));

	// Pull chip select high
	PORTC.OUTSET |= DAC_SS_bm;
}


// Initialize internal ADC
// Must be called before readADC can be used
void initADC (void)
{
	/*
	ADCA.CALL = ADCACAL0;									// Load calibration registers with stored
	ADCA.CALH = ADCACAL1;
	ADCB.CALL = ADCBCAL0;
	ADCB.CALH = ADCBCAL1;
	
	PORTA.DIR = 0x00;										// Configure PORTA as inputs
	PORTB.DIR = 0x00;										// Configure PORTB as inputs
	
	ADCA.CTRLA = ADC_ENABLE_bm;								// Enable ADCA
	ADCB.CTRLA = ADC_ENABLE_bm;								// Enable ADCB
	
	ADCA.CTRLB = ADC_RESOLUTION_12BIT_gc;
	ADCB.CTRLB = ADC_RESOLUTION_12BIT_gc;
	
	ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc;				// Peripheral clk/512
	ADCB.PRESCALER = ADC_PRESCALER_DIV512_gc;				// Peripheral clk/512
	
	ADCA.REFCTRL = ADC_REFSEL_AREFA_gc;
	ADCB.REFCTRL = ADC_REFSEL_AREFA_gc;
	
	ADCA.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;		// Configure ADCA and ADCb in single ended input mode
	ADCB.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;		// Only using CH0, no pipelining
	*/
}


// Reads 12 bit data value from addressed channel of internal ADC
// Channel addresses are numbered from 0 to 15 (ADCA is 0 to 7 and ADCB is 8 to 15)
// initADC must be called before function can be used
uint16_t readADC (uint8_t channel)
{
	uint16_t result = 0;
	
	if (channel <= 7)
	{
		ADCA.CH0.MUXCTRL = (channel << 3);
		while(!ADCA.CH0.INTFLAGS);
		result = ADCA.CH0RES;
	}
	else
	{
		ADCB.CH0.MUXCTRL = ( (channel - 7) << 3);
		while(!ADCB.CH0.INTFLAGS);
		result = ADCB.CH0RES;
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

// enable or disable panel
// 1 to enable, 0 to disable
void enablePanel (uint8_t panelNo, uint8_t enable)
{
	if (ENABLE)
	{
		switch (panelNo)
		{
			case PANEL_XP :
				PORTF.OUTSET &= ~SHDN_XP_bm;
				break;
			case PANEL_YP :
				PORTF.OUTSET &= ~SHDN_YP_bm;
				break;
			case PANEL_ZP :
				PORTF.OUTSET &= ~SHDN_ZP_bm;
				break;
			case PANEL_XN :
				PORTF.OUTSET &= ~SHDN_XN_bm;
				break;
			case PANEL_YN :
				PORTF.OUTSET &= ~SHDN_YN_bm;
				break;
			case PANEL_ZN :
				PORTF.OUTSET &= ~SHDN_ZN_bm;
				break;
			default :
				break;
		}
	}
	else
	{
		switch (panelNo)
		{
			case PANEL_XP :
				PORTF.OUTSET |= SHDN_XP_bm;
				break;
			case PANEL_YP :
				PORTF.OUTSET |= SHDN_YP_bm;
				break;
			case PANEL_ZP :
				PORTF.OUTSET |= SHDN_ZP_bm;
				break;
			case PANEL_XN :
				PORTF.OUTSET |= SHDN_XN_bm;
				break;
			case PANEL_YN :
				PORTF.OUTSET |= SHDN_YN_bm;
				break;
			case PANEL_ZN :
				PORTF.OUTSET |= SHDN_ZN_bm;
				break;
			default :
				break;
		}
	}
}
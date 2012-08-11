//
//	 BLUEsat Battery Charge Regulator firmware
//
//	 core.c : Contains low level hardware specific functions
//
//	 For AVR ATxmega64A3
//

#include <avr/io.h>
#include <util/delay.h>
#include "core.h"

// Set GPIO directions (for BCR shutdown pins and frequency synchronization enable pin)
void initGPIOdir (void)
{
	// Set BCR shutdown pins as outputs
	// Initialize to disabled
	ioport_configure_pin(GPIO_SHDN_XP, IOPORT_DIR_OUTPUT | BCR_DISABLE);
	ioport_configure_pin(GPIO_SHDN_YP, IOPORT_DIR_OUTPUT | BCR_DISABLE);
	ioport_configure_pin(GPIO_SHDN_ZP, IOPORT_DIR_OUTPUT | BCR_DISABLE);
	ioport_configure_pin(GPIO_SHDN_XN, IOPORT_DIR_OUTPUT | BCR_DISABLE);
	ioport_configure_pin(GPIO_SHDN_YN, IOPORT_DIR_OUTPUT | BCR_DISABLE);
	ioport_configure_pin(GPIO_SHDN_ZN, IOPORT_DIR_OUTPUT | BCR_DISABLE);

	// Set frequency synchronization enable pin as output
	ioport_configure_pin(FSYNC_EN_bm, IOPORT_DIR_OUTPUT | FSYNC_DISABLE);
}

// Initialize SPI bus for LTC1660 D/A converter
void initDAC (void)
{	
	spi_enable_master_mode(SPIC);
	spi_xmega_set_baud_div(SPIC, DAC_SPI_BAUD, 16);
	ioport_configure_pin(GPIO_DAC_SS, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
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

// Send 10 bit data value to addressed channel of LTC1660 D/A converter
void dacSet (uint8_t address, uint16_t value)
{
	uint16_t dacWord = (value << 2) | (((uint16_t) address) << 12);

	// Pull chip select low
	ioport_set_pin_low(GPIO_DAC_SS);
	_delay_us(1);

	// Send word
	spi_put(DAC_SPI, (uint8_t) (dacWord >> 8));
	spi_put(DAC_SPI, (uint8_t) dacWord);

	// Pull chip select high
	ioport_set_pin_high(GPIO_DAC_SS);
}

/*
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
{	
	switch (panelNo)
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

// Initialize internal ADC
// Must be called before readADC can be used
void adcInit (void)
{
	struct adc_config         adc_conf;
	struct adc_channel_config adcch_conf;
	
	// Clear the configuration structures.
	memset(&adc_conf, 0, sizeof(struct adc_config));
	memset(&adcch_conf, 0, sizeof(struct adc_channel_config));
	
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_ON, ADC_RES_12, ADC_REF_AREFA);
	adc_set_clock_rate(&adc_conf, 200000UL);
	//adc_set_conversion_trigger(&adc_conf, ADC_TRIG_EVENT_SWEEP, 1, 0);
	adc_set_current_limit(&adc_conf, ADC_CURRENT_LIMIT_HIGH);
	adc_set_gain_impedance_mode(&adc_conf, ADC_GAIN_HIGHIMPEDANCE);

	adc_write_configuration(&ADCA, &adc_conf);
	adc_set_callback(&ADCA, &adc_handler);
	
	
	
	
	
	
	
	if (!adc_is_enabled())
		adc_enable();
		
	adc_get_calibration_data(ADC_CAL_ADCA);
	adc_get_calibration_data(ADC_CAL_ADCB);
	
	
	
	
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
	
}

// Reads 12 bit data value from addressed channel of internal ADC
// Channel addresses are numbered from 0 to 15 (ADCA is 0 to 7 and ADCB is 8 to 15)
// initADC must be called before function can be used
uint16_t adcRead (ADC_t adc, uint8_t channel)
{
	uint16_t result;
	adc_start_conversion(adc, (1 << channel));
	result = 
	return result;	
}
*/


// Enable/disable 700 kHz frequency synchronization
// syncEnable = 1 to enable, 0 to disable
void freqSync (uint8_t syncEnable)
{
	if (syncEnable)
		ioport_set_pin_high(GPIO_FSYNC_EN);
	else
		ioport_set_pin_low(GPIO_FSYNC_EN);
}

// enable or disable panel
// 1 to enable, 0 to disable
void enablePanel (uint8_t panelNo, uint8_t enable)
{
	if (enable)
		ioport_set_group_high(BCR_SHDN_PORT, (1 << panelNo));
	else
		ioport_set_group_low(BCR_SHDN_PORT, (1 << panelNo));
}
/*
 * adc.c
 *
 * Created: 4/09/2012 6:11:16 PM
 *  Author: Mitch
 */ 

#include <asf.h>

//Interrupt handler for ADC samples
//Reads result into bcr_adc and starts sample on next pin on ADC.
//(We can't use DMA because we have to change input multiplexers on each sample)
static void adc_handler(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{
	struct adc_channel_config adcch_conf;
	uint8_t muxpos;
	uint16_t* result_store;
	
	//All ADC samples should be done on CH0 (This loses pipelining, but ADC speed isn't a worry)
	if ( ch_mask != ADC_CH0 )
		return;
		
	//Read sampled muxpos
	adcch_read_configuration(adc, ADC_CH0, &adcch_conf);
	muxpos = (adcch_conf.muxctrl&ADC_CH_MUXPOS_gm) >> ADC_CH_MUXPOS_gp;
	
	//Use pointer arithm to store result into bcr_adc
	if (adc == &ADCA) //ADCA0-ADCA7
		result_store = ((uint16_t*)&bcr_adc) + muxpos;
	else //ADCB0-ADCB7
		result_store = ((uint16_t*)&bcr_adc) + muxpos + 8;
	*result_store = result;
	
	//Increment and store muxpos into CH0 config
	muxpos = (muxpos+1)%8;
	adcch_conf.muxctrl = (adcch_conf.muxctrl & ~ADC_CH_MUXPOS_gm) | (muxpos<<ADC_CH_MUXPOS_gp);
	adcch_write_configuration(adc, ADC_CH0, &adcch_conf);
	
	//Start conversion on the next pin (will eventually trigger another adc_handler)
	adc_start_conversion(adc, ADC_CH0);
}

//Sets up ADC in 12-bit mode, with AREFA reference, manually triggered and with adc_handler as the interrupt callback
//Sets up ADC's CH0 to sample Pin0 in single ended mode with interrupts. 
static void adc_setup(ADC_t* adc)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	
	adc_read_configuration(adc, &adc_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12, ADC_REF_AREFA);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adc_conf, 5000UL);
	adc_set_callback(adc, &adc_handler);
	adc_write_configuration(adc, &adc_conf);
	
	//Setup CH0
	adcch_read_configuration(adc, ADC_CH0, &adcch_conf);
	adcch_enable_interrupt(&adcch_conf);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN0, ADCCH_NEG_NONE, 1);
	adcch_write_configuration(adc, ADC_CH0, &adcch_conf);
}

void bcr_adc_init(void)
{
	//Setup ADCs
	adc_setup(&ADCA);
	adc_setup(&ADCB);
	
	//Ensure interrupts are enabled
	cpu_irq_enable();
	
	//Enable ADCs
	adc_enable(&ADCA);
	adc_enable(&ADCB);
	
	//Start first conversion
	adc_start_conversion(&ADCA, ADC_CH0);
	adc_start_conversion(&ADCB, ADC_CH0);
}